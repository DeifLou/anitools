#include "filter.h"
#include "filterwidget.h"
#include "../imgproc/types.h"

Filter::Filter() :
    mRGBMode(0)
{
    for (int i = 0; i < 5; i++)
    {
        mAffectedChannel[i] = false;
        mThreshold[i] = 128;
        memset(&mLUT[i][0], 0, 128);
        memset(&mLUT[i][128], 255, 128);
    }
    for (int i = 0; i < 256; i++)
        mIdLUT[i] = i;
}

Filter::~Filter()
{

}

ImageFilter *Filter::clone()
{
    Filter * f = new Filter();
    f->mRGBMode = mRGBMode;
    for (int i = 0; i < 5; i++)
    {
        f->mAffectedChannel[i] = mAffectedChannel[i];
        f->mThreshold[i] = mThreshold[i];
    }
    memcpy(f->mLUT, mLUT, sizeof(mLUT));
    return f;
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

    QImage i = inputImage.copy();
    register BGRA * bits = (BGRA *)i.bits();
    unsigned char * mLuts[5];
    register int totalPixels = i.width() * i.height();

    if (mRGBMode == 0)
    {
        if (!mAffectedChannel[0] && !mAffectedChannel[4])
            return i;
        mLuts[4] = mAffectedChannel[4] ? mLUT[4] : mIdLUT;
        if (mAffectedChannel[0])
        {
            register int gray;
            register unsigned short gR = .2126 * 0x10000;
            register unsigned short gG = .7152 * 0x10000;
            register unsigned short gB = .0722 * 0x10000;
            while (totalPixels--)
            {
                gray = (bits->r * gR >> 16) + (bits->g * gG >> 16) + (bits->b * gB >> 16);
                bits->r = bits->g = bits->b = mLUT[0][gray];
                bits->a = mLuts[4][bits->a];
                bits++;
            }
        }
        else
        {
            while (totalPixels--)
            {
                bits->a = mLuts[4][bits->a];
                bits++;
            }
        }

    }
    else
    {
        if (!mAffectedChannel[1] && !mAffectedChannel[2] &&
            !mAffectedChannel[3] && !mAffectedChannel[4])
            return i;
        mLuts[1] = mAffectedChannel[1] ? mLUT[1] : mIdLUT;
        mLuts[2] = mAffectedChannel[2] ? mLUT[2] : mIdLUT;
        mLuts[3] = mAffectedChannel[3] ? mLUT[3] : mIdLUT;
        mLuts[4] = mAffectedChannel[4] ? mLUT[4] : mIdLUT;
        while (totalPixels--)
        {
            bits->r = mLuts[1][bits->r];
            bits->g = mLuts[2][bits->g];
            bits->b = mLuts[3][bits->b];
            bits->a = mLuts[4][bits->a];
            bits++;
        }
    }

    return i;
}

bool Filter::loadParameters(QSettings &s)
{
    QString rgbModeStr, affectedChannelStr;
    int rgbMode;
    QStringList affectedChannelList;
    bool affectedChannel[5] =  { false };
    int threshold[5] = { 128 };
    bool ok;

    rgbModeStr = s.value("rgbmode", "tight").toString();
    if (rgbModeStr == "tight")
        rgbMode = 0;
    else if (rgbModeStr == "independent")
        rgbMode = 1;
    else
        return false;

    affectedChannelStr = s.value("affectedchannels", "").toString();
    affectedChannelList = affectedChannelStr.split(" ", QString::SkipEmptyParts);
    for (int i = 0; i < affectedChannelList.size(); i++)
    {
        affectedChannelStr = affectedChannelList.at(i);
        if (affectedChannelList.at(i) == "rgb")
            affectedChannel[0] = true;
        else if (affectedChannelList.at(i) == "red")
            affectedChannel[1] = true;
        else if (affectedChannelList.at(i) == "green")
            affectedChannel[2] = true;
        else if (affectedChannelList.at(i) == "blue")
            affectedChannel[3] = true;
        else if (affectedChannelList.at(i) == "alpha")
            affectedChannel[4] = true;
        else
            return false;
    }

    threshold[0] = s.value("rgbthreshold", 128).toInt(&ok);
    if (!ok || threshold[0] < 0 || threshold[0] > 255)
        return false;
    threshold[1] = s.value("redthreshold", 128).toInt(&ok);
    if (!ok || threshold[1] < 0 || threshold[1] > 255)
        return false;
    threshold[2] = s.value("greenthreshold", 128).toInt(&ok);
    if (!ok || threshold[2] < 0 || threshold[2] > 255)
        return false;
    threshold[3] = s.value("bluethreshold", 128).toInt(&ok);
    if (!ok || threshold[3] < 0 || threshold[3] > 255)
        return false;
    threshold[4] = s.value("alphathreshold", 128).toInt(&ok);
    if (!ok || threshold[4] < 0 || threshold[4] > 255)
        return false;

    setRGBMode(rgbMode);
    for (int i = 0; i < 5; i++)
    {
        setAffectedChannel(i, affectedChannel[i]);
        setThreshold(i, threshold[i]);
    }

    return true;
}

bool Filter::saveParameters(QSettings &s)
{
    s.setValue("rgbmode", mRGBMode == 0 ? "tight" : "independent");

    QStringList affectedChannelList;
    if (mAffectedChannel[0])
        affectedChannelList.append("rgb");
    if (mAffectedChannel[1])
        affectedChannelList.append("red");
    if (mAffectedChannel[2])
        affectedChannelList.append("green");
    if (mAffectedChannel[3])
        affectedChannelList.append("blue");
    if (mAffectedChannel[4])
        affectedChannelList.append("alpha");
    s.setValue("affectedchannels", affectedChannelList.join(" "));

    s.setValue("rgbthreshold", mThreshold[0]);
    s.setValue("redthreshold", mThreshold[1]);
    s.setValue("greenthreshold", mThreshold[2]);
    s.setValue("bluethreshold", mThreshold[3]);
    s.setValue("alphathreshold", mThreshold[4]);

    return true;
}

QWidget *Filter::widget(QWidget *parent)
{
    FilterWidget * fw = new FilterWidget(parent);
    fw->setRGBMode(mRGBMode);
    for (int i = 0; i < 5; i++)
    {
        fw->setAffectedChannel(i, mAffectedChannel[i]);
        fw->setThreshold(i, mThreshold[i]);
    }
    connect(this, SIGNAL(rgbModeChanged(int)), fw, SLOT(setRGBMode(int)));
    connect(this, SIGNAL(affectedChannelChanged(int,bool)), fw, SLOT(setAffectedChannel(int,bool)));
    connect(this, SIGNAL(thresholdChanged(int,int)), fw, SLOT(setThreshold(int,int)));
    connect(fw, SIGNAL(rgbModeChanged(int)), this, SLOT(setRGBMode(int)));
    connect(fw, SIGNAL(affectedChannelChanged(int,bool)), this, SLOT(setAffectedChannel(int,bool)));
    connect(fw, SIGNAL(thresholdChanged(int,int)), this, SLOT(setThreshold(int,int)));
    return fw;
}

void Filter::setRGBMode(int m)
{
    if (m == mRGBMode)
        return;
    mRGBMode = m;
    emit rgbModeChanged(m);
    emit parametersChanged();
}

void Filter::setAffectedChannel(int c, bool a)
{
    if (a == mAffectedChannel[c])
        return;
    mAffectedChannel[c] = a;
    emit affectedChannelChanged(c, a);
    emit parametersChanged();
}

void Filter::setThreshold(int c, int t)
{
    if (t == mThreshold[c])
        return;
    mThreshold[c] = t;
    for (int i = 0; i < 256; i++)
        mLUT[c][i] = i > t ? 255 : 0;
    emit thresholdChanged(c, t);
    emit parametersChanged();
}
