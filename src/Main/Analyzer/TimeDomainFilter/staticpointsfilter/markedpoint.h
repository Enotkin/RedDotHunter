#ifndef MARKEDPOINT_H
#define MARKEDPOINT_H

#include "Analyzer/TimeDomainFilter/pointsfilter_common.h"

namespace az {

struct MarkedPoint
{
    MarkedPoint();
    MarkedPoint(const Point &point, int mark);
    MarkedPoint(const Point &point);
    static int undefinedMark();

    int mark;
    Point point;
};

} // namespace az

#endif // MARKEDPOINT_H
