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

#include <QPainter>
#include <QStyle>
#include <QStyleOptionFrame>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>

#include "imageviewer.h"
#include "../misc/util.h"

namespace ibp {
namespace widgets {

ImageViewer::ImageViewer(QWidget *parent) :
    QAbstractScrollArea(parent),
    mZoomIndex(7),
    mZoom(1.0f),
    mImage(),
    mBackgroundImage(2, 2, QImage::Format_RGB888),
    mIsMoving(false),
    mLastPos()
{
}

bool ImageViewer::viewportEvent(QEvent *e)
{
    QWidget * vp = viewport();

    if (e->type() == QEvent::Resize)
    {
        updateScrollBars();
        emit viewportResized(vp->rect());
        return true;
    }
    else if (e->type() == QEvent::Paint)
    {
        QPainter p(vp);

        p.fillRect(vp->rect(), vp->palette().dark());

        QPoint center(vp->width() >> 1, vp->height() >> 1);
        if (!mImage.isNull())
        {

            QPoint pos;
            QSize scaledSize(mImage.width() * mZoom, mImage.height() * mZoom);

            if (scaledSize.width() > vp->width())
                pos.setX(-horizontalScrollBar()->value());
            else
                pos.setX((vp->width() - scaledSize.width()) / 2);

            if (scaledSize.height() > vp->height())
                pos.setY(-verticalScrollBar()->value());
            else
                pos.setY((vp->height() - scaledSize.height()) / 2);

            QRect r(pos, scaledSize);

            p.setBrushOrigin(pos);
            p.setRenderHint(QPainter::SmoothPixmapTransform);
            int checkerboardColor1 = palette().color(QPalette::Light).rgb();
            int checkerboardColor2 = palette().color(QPalette::Midlight).rgb();
            mBackgroundImage.setPixel(0, 0, checkerboardColor1);
            mBackgroundImage.setPixel(1, 1, checkerboardColor1);
            mBackgroundImage.setPixel(0, 1, checkerboardColor2);
            mBackgroundImage.setPixel(1, 0, checkerboardColor2);
            QBrush checkerboardBrush = QBrush(mBackgroundImage);
            checkerboardBrush.setTransform(QTransform(6, 0, 0, 0, 6, 0, 0, 0, 1));
            p.fillRect(r, checkerboardBrush);

            p.setRenderHint(QPainter::SmoothPixmapTransform, mZoom < 1.0);
            p.drawImage(r, mImage, mImage.rect());

            p.setPen(vp->palette().color(QPalette::Shadow));
            p.drawRect(r.adjusted(-1, -1, 0, 0));
        }
        else
        {
            p.setRenderHint(QPainter::Antialiasing);
            int radius = (qMin<int>(vp->width(), vp->height()) - 16) >> 1;
            int offset = (radius - radius / 6) * 0.7;

            p.setBrush(Qt::transparent);
            p.setPen(QPen(vp->palette().mid(), radius / 3, Qt::SolidLine, Qt::FlatCap));

            p.drawEllipse(center, radius - radius / 6, radius - radius / 6);
            p.drawLine(center.x() - offset, center.y() - offset, center.x() + offset, center.y() + offset);
        }

        return true;
    }
    else if (e->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent * mousePressEvent = (QMouseEvent *)e;
        if ((mousePressEvent->buttons() & Qt::LeftButton) && !mImage.isNull())
        {
            vp->setCursor(Qt::ClosedHandCursor);
            mLastPos = mousePressEvent->pos();
            mIsMoving = true;
            return true;
        }
    }
    else if (e->type() == QEvent::MouseButtonRelease)
    {
        QMouseEvent * mouseReleaseEvent = (QMouseEvent *)e;
        if ((mouseReleaseEvent->button() == Qt::LeftButton) && !mImage.isNull())
        {
            vp->setCursor(Qt::OpenHandCursor);
            mIsMoving = false;
            return true;
        }
    }
    else if (e->type() == QEvent::MouseMove)
    {
        QMouseEvent * mouseMoveEvent = (QMouseEvent *)e;
        if ((mouseMoveEvent->buttons() & Qt::LeftButton) && mIsMoving && !mImage.isNull())
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() -
                                            (mouseMoveEvent->pos().x() - mLastPos.x()));
            verticalScrollBar()->setValue(verticalScrollBar()->value() -
                                          (mouseMoveEvent->pos().y() - mLastPos.y()));
            mLastPos = mouseMoveEvent->pos();
            return true;
        }
    }
    else if (e->type() == QEvent::Wheel)
    {
        QWheelEvent * mouseWheelEvent = (QWheelEvent *)e;
        if (mImage.isNull()) return true;

        if (mouseWheelEvent->angleDelta().y() > 0)
            setZoomIndex(mZoomIndex + 1);
        else if (mouseWheelEvent->angleDelta().y() < 0)
            setZoomIndex(mZoomIndex - 1);

        return true;
    }

    return false;
}

void ImageViewer::setZoomIndex(int newZoomIndex)
{
    if (newZoomIndex < 0 || newZoomIndex > 13 || newZoomIndex == mZoomIndex)
        return;

    mZoomIndex = newZoomIndex;
    switch (newZoomIndex)
    {
    case 0: mZoom = 0.0625f; break;
    case 1: mZoom = 0.1250f; break;
    case 2: mZoom = 0.2500f; break;
    case 3: mZoom = 0.3333f; break;
    case 4: mZoom = 0.5000f; break;
    case 5: mZoom = 0.6666f; break;
    case 6: mZoom = 0.7500f; break;
    case 8: mZoom = 2.0000f; break;
    case 9: mZoom = 3.0000f; break;
    case 10: mZoom = 4.0000f; break;
    case 11: mZoom = 6.0000f; break;
    case 12: mZoom = 8.0000f; break;
    case 13: mZoom = 16.0000f; break;
    default: mZoom = 1.0000f; break;
    }

    updateScrollBars();

    emit zoomIndexChanged(mZoomIndex);
}
void ImageViewer::setImage(const QImage & newImage)
{
    mImage = newImage;
    if (newImage.isNull())
        viewport()->unsetCursor();
    else
        viewport()->setCursor(Qt::OpenHandCursor);

    updateScrollBars();
}

int ImageViewer::zoomIndex()
{
    return mZoomIndex;
}
QImage ImageViewer::image()
{
    return mImage;
}

void ImageViewer::updateScrollBars()
{
    if (mImage.isNull())
    {
        verticalScrollBar()->setRange(0, 0);
        horizontalScrollBar()->setRange(0, 0);
    }
    else
    {
        QSize scaledSize(mImage.width() * mZoom, mImage.height() * mZoom);
        verticalScrollBar()->setPageStep(viewport()->height());
        horizontalScrollBar()->setPageStep(viewport()->width());

        verticalScrollBar()->setRange(0, scaledSize.height() - viewport()->height());
        horizontalScrollBar()->setRange(0, scaledSize.width() - viewport()->width());

    }
    viewport()->update();
}

}}
