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

#ifndef IBP_WIDGETS_ROTATIONGAUGE_H
#define IBP_WIDGETS_ROTATIONGAUGE_H

#include <QWidget>

namespace ibp {
namespace widgets {

class RotationGauge : public QWidget
{
    Q_OBJECT

private:
    double mAngle;
    double mAltitude;
    bool mAltitudeEnabled;

    bool mMouseButtonPressed;

public:
    explicit RotationGauge(QWidget *parent = 0);
    
    double angle();
    double altitude();
    bool isAltitudeEnabled();

public slots:
    void setAngle(double v);
    void setAltitude(double v);
    void enableAltitude(bool v);

protected:
    void paintEvent(QPaintEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent * e);

signals:
    void angleChanged(double v);
    void altitudeChanged(double v);
    
};

}}
#endif // IBP_WIDGETS_ROTATIONGAUGE_H
