#include "markedpoint.h"

az::MarkedPoint::MarkedPoint()
    : MarkedPoint(Point(), undefinedMark())
{
}

az::MarkedPoint::MarkedPoint(const az::Point &pointParam, int markParam)
    : mark(markParam)
    , point(pointParam)
{
}

az::MarkedPoint::MarkedPoint(const az::Point &pointParam)
    : MarkedPoint(pointParam, undefinedMark())
{
}

int az::MarkedPoint::undefinedMark()
{
    int mark = -1;
    return mark;
}
