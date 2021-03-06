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

#ifndef IBP_WIDGETS_OUTPUTLEVELSWIDGET_H
#define IBP_WIDGETS_OUTPUTLEVELSWIDGET_H

#include <QWidget>

namespace ibp {
namespace widgets {

namespace Ui {
class OutputLevelsWidget;
}

class OutputLevelsWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::OutputLevelsWidget *ui;

    int mUnits;

public:
    explicit OutputLevelsWidget(QWidget *parent = 0);
    ~OutputLevelsWidget();

    double blackPoint();
    double whitePoint();

    int units();

signals:
    void blackPointChanged(double v);
    void whitePointChanged(double v);

public slots:
    void setBlackPoint(double v);
    void setWhitePoint(double v);
    void setValues(double b, double w);

    void setUnits(int u);

private slots:
    void on_mSliderOutputLevels_blackPointChanged(double v);
    void on_mSliderOutputLevels_whitePointChanged(double v);
    void on_mSpinBlackPoint_valueChanged(double v);
    void on_mSpinWhitePoint_valueChanged(double v);
};

}}
#endif // IBP_WIDGETS_OUTPUTLEVELSWIDGET_H
