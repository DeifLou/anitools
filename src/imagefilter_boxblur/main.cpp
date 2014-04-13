#include <QHash>

#include "filter.h"
#include "../imgproc/imagefilter.h"

using namespace anitools::imgproc;

#ifdef Q_OS_WIN32
#define ANITOOLS_EXPORT __declspec(dllexport)
#else
#define ANITOOLS_EXPORT
#endif

extern "C" ANITOOLS_EXPORT QHash<QString, QString> getAnitoolsPluginInfo()
{
    QHash<QString, QString> info;

    info.insert("id", "anitools.imagefilter.boxblur");
    info.insert("version", "0.1.0");
    info.insert("name", QObject::tr("Box Blur (Mean Filter)"));
    info.insert("description", QObject::tr("Smooths the image using a normalized box filter"));
    info.insert("tags", QObject::tr("Smooth, Noise"));
    info.insert("author", QObject::tr("Deif Lou"));
    info.insert("copyright", QObject::tr(""));
    info.insert("url", QObject::tr(""));
    return info;
}

extern "C" ANITOOLS_EXPORT ImageFilter * getImageFilterInstance()
{
    return new Filter();
}