//
// MIT License
// 
// Copyright (c) Deif Lou
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <math.h>

#include "filterwidget.h"
#include "ui_filterwidget.h"

FilterWidget::FilterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterWidget),
    mEmitSignals(true)
{
    ui->setupUi(this);
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setFeatureSize(int fs)
{
    if (ui->mSpinFeatureSize->value() == fs)
        return;
    mEmitSignals = false;
    ui->mSpinFeatureSize->setValue(fs);
    mEmitSignals = true;
    emit featureSizeChanged(fs);
}

void FilterWidget::setOutputMode(Filter::OutputMode om)
{
    if ((om == Filter::CorrectedImageMode1 && ui->mButtonOutputModeCorrectedImageMode1->isChecked()) ||
        (om == Filter::CorrectedImageMode2 && ui->mButtonOutputModeCorrectedImageMode2->isChecked()) ||
        (om == Filter::IIHCorrectionModel && ui->mButtonOutputModeIIHCorrectionModel->isChecked()))
        return;
    mEmitSignals = false;
    if (om == Filter::CorrectedImageMode1)
        ui->mButtonOutputModeCorrectedImageMode1->setChecked(true);
    if (om == Filter::CorrectedImageMode2)
        ui->mButtonOutputModeCorrectedImageMode2->setChecked(true);
    else
        ui->mButtonOutputModeIIHCorrectionModel->setChecked(true);
    mEmitSignals = true;
    emit outputModeChanged(om);
}

void FilterWidget::on_mSliderFeatureSize_valueChanged(int value)
{
    ui->mSpinFeatureSize->setValue(value);
    if (mEmitSignals)
        emit featureSizeChanged(value);
}

void FilterWidget::on_mSpinFeatureSize_valueChanged(int arg1)
{
    ui->mSliderFeatureSize->setValue(arg1);
    if (mEmitSignals)
        emit featureSizeChanged(arg1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode1_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode1);
}

void FilterWidget::on_mButtonOutputModeCorrectedImageMode2_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::CorrectedImageMode2);
}

void FilterWidget::on_mButtonOutputModeIIHCorrectionModel_toggled(bool c)
{
    if (!c)
        return;
    if (mEmitSignals)
        emit outputModeChanged(Filter::IIHCorrectionModel);
}
