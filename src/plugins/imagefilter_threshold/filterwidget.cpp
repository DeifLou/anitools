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

    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();

    ui->mContainerLuma->hide();
    ui->mContainerRed->hide();
    ui->mContainerGreen->hide();
    ui->mContainerBlue->hide();
    ui->mContainerAlpha->hide();

    mButtonAffectedChannel[0] = ui->mButtonAffectedChannelsLuma;
    mButtonAffectedChannel[1] = ui->mButtonAffectedChannelsRed;
    mButtonAffectedChannel[2] = ui->mButtonAffectedChannelsGreen;
    mButtonAffectedChannel[3] = ui->mButtonAffectedChannelsBlue;
    mButtonAffectedChannel[4] = ui->mButtonAffectedChannelsAlpha;

    mSpinThreshold[0] = ui->mSpinLuma;
    mSpinThreshold[1] = ui->mSpinRed;
    mSpinThreshold[2] = ui->mSpinGreen;
    mSpinThreshold[3] = ui->mSpinBlue;
    mSpinThreshold[4] = ui->mSpinAlpha;
}

FilterWidget::~FilterWidget()
{
    delete ui;
}

void FilterWidget::setColorMode(int m)
{
    if ((m == 0 && ui->mButtonColorModeLuma->isChecked()) ||
        (m == 1 && ui->mButtonColorModeRGB->isChecked()))
        return;

    mEmitSignals = false;
    if (m == 0)
        ui->mButtonColorModeLuma->setChecked(true);
    else
        ui->mButtonColorModeRGB->setChecked(true);
    mEmitSignals = true;

    emit colorModeChanged(m);
}

void FilterWidget::setAffectedChannel(int c, bool a)
{
    if (mButtonAffectedChannel[c]->isChecked() == a)
        return;

    mEmitSignals = false;
    mButtonAffectedChannel[c]->setChecked(a);
    mEmitSignals = true;

    emit affectedChannelChanged(c, a);
}

void FilterWidget::setThreshold(int c, int t)
{
    if (mSpinThreshold[c]->value() == t)
        return;

    mEmitSignals = false;
    mSpinThreshold[c]->setValue(t);
    mEmitSignals = true;

    emit thresholdChanged(c, t);
}

void FilterWidget::on_mButtonColorModeLuma_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsLuma->show();
    ui->mButtonAffectedChannelsRed->hide();
    ui->mButtonAffectedChannelsGreen->hide();
    ui->mButtonAffectedChannelsBlue->hide();
    ui->mContainerLuma->setVisible(ui->mButtonAffectedChannelsLuma->isChecked());
    ui->mContainerRed->hide();
    ui->mContainerGreen->hide();
    ui->mContainerBlue->hide();
    if (mEmitSignals)
        emit colorModeChanged(0);
}

void FilterWidget::on_mButtonColorModeRGB_toggled(bool checked)
{
    if (!checked)
        return;
    ui->mButtonAffectedChannelsLuma->hide();
    ui->mButtonAffectedChannelsRed->show();
    ui->mButtonAffectedChannelsGreen->show();
    ui->mButtonAffectedChannelsBlue->show();
    ui->mContainerLuma->hide();
    ui->mContainerRed->setVisible(ui->mButtonAffectedChannelsRed->isChecked());
    ui->mContainerGreen->setVisible(ui->mButtonAffectedChannelsGreen->isChecked());
    ui->mContainerBlue->setVisible(ui->mButtonAffectedChannelsBlue->isChecked());
    if (mEmitSignals)
        emit colorModeChanged(1);
}

void FilterWidget::on_mButtonAffectedChannelsLuma_toggled(bool checked)
{
    ui->mContainerLuma->setVisible(checked && ui->mButtonColorModeLuma->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(0, checked);
}

void FilterWidget::on_mButtonAffectedChannelsRed_toggled(bool checked)
{
    ui->mContainerRed->setVisible(checked && ui->mButtonColorModeRGB->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(1, checked);
}

void FilterWidget::on_mButtonAffectedChannelsGreen_toggled(bool checked)
{
    ui->mContainerGreen->setVisible(checked && ui->mButtonColorModeRGB->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(2, checked);
}

void FilterWidget::on_mButtonAffectedChannelsBlue_toggled(bool checked)
{
    ui->mContainerBlue->setVisible(checked && ui->mButtonColorModeRGB->isChecked());
    if (mEmitSignals)
        emit affectedChannelChanged(3, checked);
}

void FilterWidget::on_mButtonAffectedChannelsAlpha_toggled(bool checked)
{
    ui->mContainerAlpha->setVisible(checked);
    if (mEmitSignals)
        emit affectedChannelChanged(4, checked);
}

void FilterWidget::on_mSliderLuma_valueChanged(int value)
{
    ui->mSpinLuma->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(0, value);
}

void FilterWidget::on_mSpinLuma_valueChanged(int arg1)
{
    ui->mSliderLuma->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(0, arg1);
}

void FilterWidget::on_mSliderRed_valueChanged(int value)
{
    ui->mSpinRed->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(1, value);
}

void FilterWidget::on_mSpinRed_valueChanged(int arg1)
{
    ui->mSliderRed->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(1, arg1);
}

void FilterWidget::on_mSliderGreen_valueChanged(int value)
{
    ui->mSpinGreen->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(2, value);
}

void FilterWidget::on_mSpinGreen_valueChanged(int arg1)
{
    ui->mSliderGreen->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(2, arg1);
}

void FilterWidget::on_mSliderBlue_valueChanged(int value)
{
    ui->mSpinBlue->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(3, value);
}

void FilterWidget::on_mSpinBlue_valueChanged(int arg1)
{
    ui->mSliderBlue->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(3, arg1);
}

void FilterWidget::on_mSliderAlpha_valueChanged(int value)
{
    ui->mSpinAlpha->setValue(value);
    if (mEmitSignals)
        emit thresholdChanged(4, value);
}

void FilterWidget::on_mSpinAlpha_valueChanged(int arg1)
{
    ui->mSliderAlpha->setValue(arg1);
    if (mEmitSignals)
        emit thresholdChanged(4, arg1);
}
