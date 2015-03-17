/****************************************************************************
**
** Copyright (C) 2014 - 2015 Deif Lou
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

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QImage>
#include <QSettings>
#include <QWidget>

#include "../imgproc/imagefilter.h"
#include "../imgproc/types.h"

using namespace anitools::imgproc;

class Filter : public ImageFilter
{
    Q_OBJECT

public:
    enum Position
    {
        Front, Behind, Inside
    };

    Filter();
    ~Filter();
    ImageFilter * clone();
    QHash<QString, QString> info();
    QImage process(const QImage & inputImage);
    bool loadParameters(QSettings & s);
    bool saveParameters(QSettings & s);
    QWidget * widget(QWidget *parent = 0);

private:
    QImage mImage;
    Position mPosition;
    ColorCompositionMode mColorCompositionMode;
    int mOpacity;
    QList<AffineTransformation> mTransformations;
    QList<bool> mBypasses;

signals:
    void imageChanged(const QImage & i);
    void positionChanged(Filter::Position v);
    void colorCompositionModeChanged(ColorCompositionMode v);
    void opacityChanged(int v);
    void transformationsChanged(const QList<AffineTransformation> & t, const QList<bool> & b);

public slots:
    void setImage(const QImage & i);
    void setPosition(Filter::Position v);
    void setColorCompositionMode(ColorCompositionMode v);
    void setOpacity(int v);
    void setTransformations(const QList<AffineTransformation> & t, const QList<bool> & b);
};

#endif // FILTER_H
