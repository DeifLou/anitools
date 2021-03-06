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

void FilterWidget::setStrength(double s)
{
    if (ui->mSpinStrength->value() == s)
        return;
    mEmitSignals = false;
    ui->mSpinStrength->setValue(s);
    mEmitSignals = true;
    emit strengthChanged(s);
}

void FilterWidget::setIterations(int i)
{
    if (ui->mSpinIterations->value() == i)
        return;
    mEmitSignals = false;
    ui->mSpinIterations->setValue(i);
    mEmitSignals = true;
    emit iterationsChanged(i);
}

void FilterWidget::on_mSliderStrength_valueChanged(int v)
{
    ui->mSpinStrength->setValue(v / 100.);
    if (mEmitSignals)
        emit strengthChanged(v / 100.);
}

void FilterWidget::on_mSpinStrength_valueChanged(double v)
{
    ui->mSliderStrength->setValue((int)round(v * 100.));
    if (mEmitSignals)
        emit strengthChanged(v);
}

void FilterWidget::on_mSliderIterations_valueChanged(int v)
{
    ui->mSpinIterations->setValue(v);
    if (mEmitSignals)
        emit iterationsChanged(v);
}

void FilterWidget::on_mSpinIterations_valueChanged(int v)
{
    ui->mSliderIterations->setValue(v);
    if (mEmitSignals)
        emit iterationsChanged(v);
}
