#ifndef OBJECTTRZCK_H
#define OBJECTTRZCK_H

//#include <QObject>

#include <QColor>

#include "contour.h"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

class ObjectTrack
{
public:
    ObjectTrack(const Contours &contours);

private:
    Contours contours;
    std::vector<QPoint> coordinates;
};

#endif // OBJECTTRZCK_H
