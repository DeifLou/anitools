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

    info.insert("id", "anitools.imagefilter.grayscale");
    info.insert("version", "0.1.0");
    info.insert("name", QObject::tr("Grayscale"));
    info.insert("description", QObject::tr("Convert the image to grayscale"));
    info.insert("tags", QObject::tr("Color"));
    info.insert("author", QObject::tr("Deif Lou"));
    info.insert("copyright", QObject::tr(""));
    info.insert("url", QObject::tr(""));
    return info;
}

extern "C" ANITOOLS_EXPORT ImageFilter * getImageFilterInstance()
{
    return new Filter();
}