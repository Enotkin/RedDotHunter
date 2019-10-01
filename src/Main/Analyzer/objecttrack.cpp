#include "objecttrack.h"

//ObjectTrack::ObjectTrack(QObject *parent) : QObject(parent)
//{

//}

ObjectTrack::ObjectTrack(const Contours &contours/*, QObject *parent*/)
{
    this->contours = contours;
}
