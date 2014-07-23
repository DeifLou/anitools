/****************************************************************************
**
** Copyright (C) 2014 Deif Lou
**
** This file is part of Anitools
**
** Anitools is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <opencv2/imgproc.hpp>
#include "../../../eigen/Eigen/Dense"
#include <QDebug>
#include <limits>

#include "filter.h"
#include "../imgproc/lut.h"
#include "../imgproc/types.h"
#include "../imgproc/colorconversion.h"
#include "../misc/util.h"

#define MAX_IMAGE_SIZE 128
#define DEGREE 3
#define MIU .5
#define MIUSQR (MIU * MIU)
#define MATRIXCOLUMNS ((DEGREE + 1) * (DEGREE + 2) / 2 - 1)
#define BLURKERNELSIZE 5

Filter::Filter()
{
}

Filter::~Filter()
{
}

ImageFilter *Filter::clone()
{
    return new Filter();
}

extern "C" QHash<QString, QString> getAnitoolsPluginInfo();
QHash<QString, QString> Filter::info()
{
    return getAnitoolsPluginInfo();
}

QImage Filter::process(const QImage &inputImage)
{
    if (inputImage.isNull() || inputImage.format() != QImage::Format_ARGB32)
        return inputImage;

    register int x, y, w = inputImage.width(), h = inputImage.height(), mean = 0, sw, sh, i, j;
    register HSL * bitsHSL = (HSL *)malloc(w * h * sizeof(HSL)), * bitsHSLsl;
    cv::Mat mlchannel(h, w, CV_8UC1);
    register unsigned char * mbits8;
    register double * mbits321, * mbits322;
    register double weight;

    // Convert to HSL
    convertBGRToHSL(inputImage.bits(), (unsigned char *)bitsHSL, w * h);

    // Separate L channel
    for (y = 0; y < h; y++)
    {
        bitsHSLsl = bitsHSL + y * w;
        mbits8 = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            *mbits8 = bitsHSLsl->l;
            bitsHSLsl++;
            mbits8++;
        }
    }

    // Resample
    cv::Mat mInitial;
    if (w != MAX_IMAGE_SIZE || h != MAX_IMAGE_SIZE)
    {
        if (w > h)
        {
            sw = MAX_IMAGE_SIZE;
            sh = h * MAX_IMAGE_SIZE / w;
        }
        else
        {
            sh = MAX_IMAGE_SIZE;
            sw = w * MAX_IMAGE_SIZE / h;
        }
        cv::Mat mresized(sh, sw, CV_8UC1);
        cv::resize(mlchannel, mresized, cv::Size(sw, sh), 0, 0, cv::INTER_CUBIC);
        mInitial = mresized;
    }
    else
    {
        sw = w;
        sh = h;
        mInitial = mlchannel;
    }

    // Remove noise
    cv::Mat mBlurred(sh, sw, CV_8UC1);
    cv::GaussianBlur(mInitial, mBlurred, cv::Size(BLURKERNELSIZE, BLURKERNELSIZE), 0);

    // Convert to float
    cv::Mat mBlurredF(sh, sw, CV_64FC1);
    for (y = 0; y < sh; y++)
    {
        mbits8 = mBlurred.ptr(y);
        mbits321 = (double *)mBlurredF.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits321 = (*mbits8) / 255.;
            mbits8++;
            mbits321++;
        }
    }

    // Get the gradient of the blurred image
    cv::Mat mGradientX(sh, sw, CV_64FC1);
    cv::Mat mGradientY(sh, sw, CV_64FC1);
    cv::Sobel(mBlurredF, mGradientX, -1, 1, 0);
    cv::Sobel(mBlurredF, mGradientY, -1, 0, 1);

    // Apply weights to the gradient
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mGradientX.ptr(y);
        mbits322 = (double *)mGradientY.ptr(y);
        for (x = 0; x < sw; x++)
        {
            weight = exp(-sqrt((*mbits321) * (*mbits321) + (*mbits322) * (*mbits322)) / MIUSQR);
            *mbits321 *= weight;
            *mbits322 *= weight;
            mbits321++;
            mbits322++;
        }
    }

    // Surface fitting using eigen
    // First set up the matrix A and the vector b
    register int row = 0, column = 0, totalPixels = sw * sh;
    Eigen::MatrixXf ls_A(totalPixels * 2, MATRIXCOLUMNS);
    Eigen::VectorXf ls_b(totalPixels * 2);
    Eigen::VectorXf ls_x;

    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mGradientX.ptr(y);
        mbits322 = (double *)mGradientY.ptr(y);
        for (x = 0; x < sw; x++)
        {
            column = 0;
            // Fill matrix A with the monomial terms of the polynomial surface
            for (i = 1; i <= DEGREE; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    ls_A(row, column) = (i - j) * (x == 0 ? 1 : pow(x, i - j - 1)) * (y == 0 ? 1 : pow(y, j));
                    ls_A(row + totalPixels, column) = j * (x == 0 ? 1 : pow(x, i - j)) * (y == 0 ? 1 : pow(y, j - 1));

                    column++;
                }
            }
            // Fill vector b with the image gradient values
            ls_b(row) = *mbits321;
            ls_b(row + totalPixels) = *mbits322;

            mbits321++;
            mbits322++;
            row++;
        }
    }
    // Solve...
    ls_x = ls_A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(ls_b);

    // Create polynomial image
    cv::Mat mIIHf(sh, sw, CV_64FC1);
    register double value;
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mIIHf.ptr(y);
        for (x = 0; x < sw; x++)
        {
            value = 0.;
            row = 0;
            for (i = 1; i <= DEGREE; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    value += ls_x(row) * (x == 0 ? 1 : pow(x, i - j)) * (y == 0 ? 1 : pow(y, j));
                    row++;
                }
            }
            *mbits321 = value;

            mbits321++;
        }
    }

    // Remap due to the lack of the constant term in the previous step and get the mean
    cv::Mat mIIH(sh, sw, CV_8UC1);
    double min, max, minb, maxb;
    cv::minMaxLoc(mIIHf, &min, &max);
    cv::minMaxLoc(mBlurredF, &minb, &maxb);
    for (y = 0; y < sh; y++)
    {
        mbits321 = (double *)mIIHf.ptr(y);
        mbits8 = mIIH.ptr(y);
        for (x = 0; x < sw; x++)
        {
            *mbits8 = round((minb + ((*mbits321) - min) * (maxb - minb) / (max - min)) * 255.);
            mean += *mbits8;
            mbits8++;
            mbits321++;
        }
    }
    mean /= totalPixels;

    // Resample
    if (w != sw || h != sh)
        cv::resize(mIIH, mlchannel, cv::Size(w, h), 0, 0, cv::INTER_CUBIC);
    else
        mlchannel = mIIH;

    // Divide lightness channel
    for (y = 0; y < h; y++)
    {
        bitsHSLsl = bitsHSL + y * w;
        mbits8 = mlchannel.ptr(y);
        for (x = 0; x < w; x++)
        {
            bitsHSLsl->l = AT_clamp(0, lut02[bitsHSLsl->l][*mbits8] * mean / 255, 255);
            bitsHSLsl++;
            mbits8++;
        }
    }

    // Convert to RGB
    QImage finalImage = inputImage.copy();
    convertHSLToBGR((const unsigned char *)bitsHSL, finalImage.bits(), w * h);
    free(bitsHSL);

    return finalImage;
}

bool Filter::loadParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    Q_UNUSED(s)
    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    Q_UNUSED(parent)
    return(0);
}