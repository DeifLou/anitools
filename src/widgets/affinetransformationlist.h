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

#ifndef ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H
#define ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H

#include <QWidget>

namespace anitools {
namespace widgets {

namespace Ui {
class AffineTransformationList;
}

class AffineTransformationList : public QWidget
{
    Q_OBJECT

public:
    enum TransformationType
    {
        Translation,
        Scaling,
        Rotation,
        Shearing
    };

    struct Transformation
    {
        TransformationType type;
        double v1, v2;
        bool bypass;
    };

public:
    explicit AffineTransformationList(QWidget *parent = 0);
    ~AffineTransformationList();

private:
    Ui::AffineTransformationList *ui;

    QList<Transformation> mTransformations;

signals:
    void transformationChanged(const QList<Transformation> & tList);

private slots:
    void On_mButtonAppend_menuActionClicked();
    void On_valueChanged(double v);
};

}}

#endif // ANITOOLS_WIDGETS_AFFINETRANSFORMATIONLIST_H