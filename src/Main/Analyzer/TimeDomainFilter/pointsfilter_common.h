#ifndef POINTSFILTER_COMMON_H
#define POINTSFILTER_COMMON_H

#include <vector>
#include <QPoint>

namespace az {

using Point = QPoint;
using Points = std::vector<Point>;
using FramePoints = std::vector<Points>;

} // namespace az

#endif // POINTSFILTER_COMMON_H
