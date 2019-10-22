#include "staticpointsfilterprivate.h"

//1. Filter to only time-points.
//2. Filter by distance
//3. Filter 2/3/4 (heights)


//class NNN
//{
//public:
//    using Point = math::Point;
//    using Points = std::vector<Point>;

//public:
//    void analyze(const Points &points);

//    //const Points &originalPoints();
//    const Points &analyzedPoints() const;

//private:

//};

//class YYY
//{
//public:
//    using Point = math::Point;
//    using Points = std::vector<Point>;

//private:

//};

//1. Point
//2. check point with previous available points
//3.1. If found appropriate point -> push
//3.2. If not -> mark as new
//4. remove outdated traces (PointTrace)

//class PointTrace {
//public:
//    void setLast(const MarkedPoint &markedPoint);
//    const MarkedPoint &last() const;
//    const MarkedPoints &points() const;
//private:

//};

//class MarkedPoint: public Point {
//public:
//    int frame();
//};


//for (frame 0 .. frameCount) {
//    for (point) {
//        int appropriateTrace = findAppropriateTrace(markedPoint);
//        if (!= -1)
//            updateTrace(appropriateTrace, markedPoint);
//        else
//            newTrace(markedPoint);
//    }
//    removeFarTraces(frame);
//}
//distributeLeftTraces();


//Point:
//  int frame;


az::StaticPointsFilterPrivate::StaticPointsFilterPrivate() :
    m_lastPointTraceGeneratedId(0)
{
}

az::StaticPointsFilterPrivate::~StaticPointsFilterPrivate()
{
}

void az::StaticPointsFilterPrivate::run(const FramePoints &framePoints)
{
   clear();
   m_filteredPoints.resize(framePoints.size());

   for (int frame = 0; frame < framePoints.size(); ++frame)
   {
       const auto &points = framePoints.at(frame);
       filterPointsAtFrame(points, frame);
       removeFarTraces(frame);
   }

   distributeLeftTraces();
}

const az::FramePoints &az::StaticPointsFilterPrivate::filteredPoints() const
{
    return m_filteredPoints;
}

void az::StaticPointsFilterPrivate::clear()
{
    m_lastPointTraceGeneratedId = 0;
    m_pointTraces.clear();
    m_filteredPoints.clear();
}

const az::StaticPointsFilterSettings &az::StaticPointsFilterPrivate::settings() const
{
    return m_settings;
}

void az::StaticPointsFilterPrivate::setSettings(const StaticPointsFilterSettings &settings)
{
    m_settings = settings;
}

az::StaticPointsFilterPrivate::PointTraceId az::StaticPointsFilterPrivate::undefinedPointTraceId()
{
    PointTraceId id = -1;
    return id;
}

void az::StaticPointsFilterPrivate::filterPointsAtFrame(const Points &points, int frame)
{
    for (auto &point : points)
    {
        filterPointAtFrame(point, frame);
    }
}

void az::StaticPointsFilterPrivate::filterPointAtFrame(const Point &point, int frame)
{
    MarkedPoint markedPoint(point, frame);
    PointTraceId appropriateTraceId = findAppropriateTrace(markedPoint);
    if (isValidPointTraceId(appropriateTraceId))
    {
        updatePointTrace(appropriateTraceId, markedPoint);
    }
    else
    {
        createTrace(markedPoint);
    }
}

void az::StaticPointsFilterPrivate::removeFarTraces(int frame)
{
    auto it = m_pointTraces.begin();
    while (it != m_pointTraces.end())
    {
        const PointTrace &pointTrace = it->second;
        if (isFarPointTrace(pointTrace, frame))
        {
            if (isAppropriatePointTrace(pointTrace))
            {
                distributePointTrace(pointTrace);
            }

            it = m_pointTraces.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void az::StaticPointsFilterPrivate::distributeLeftTraces()
{
    for (auto it = m_pointTraces.begin(); it != m_pointTraces.end(); ++it)
    {
        const PointTrace &pointTrace = it->second;
        if (isAppropriatePointTrace(pointTrace))
        {
            distributePointTrace(pointTrace);
        }
    }

    m_pointTraces.clear();
}

az::StaticPointsFilterPrivate::PointTraceId az::StaticPointsFilterPrivate::generatePointTraceId()
{
    return m_lastPointTraceGeneratedId++;
}

void az::StaticPointsFilterPrivate::updatePointTrace(PointTraceId pointTraceId, const MarkedPoint &markedPoint)
{
    auto it = m_pointTraces.find(pointTraceId);
    PointTrace &pointTrace = it->second;
    pointTrace.push_back(markedPoint);
}

void az::StaticPointsFilterPrivate::createTrace(const MarkedPoint &markedPoint)
{
    PointTrace pointTrace(markedPoint);
    m_pointTraces.insert(std::make_pair(generatePointTraceId(), pointTrace));
}

az::StaticPointsFilterPrivate::PointTraceId az::StaticPointsFilterPrivate::findAppropriateTrace(const MarkedPoint &markedPoint) const
{
    PointTraceId id = undefinedPointTraceId();
    auto it = m_pointTraces.begin();
    const PointTrace *bestPointTraceMatch = nullptr;
    while (it != m_pointTraces.end())
    {
        const PointTrace &pointTrace = it->second;
        if (isAppropriatePoint(markedPoint, pointTrace))
        {
            if (bestPointTraceMatch == nullptr
                    || isBetterMatch(markedPoint, *bestPointTraceMatch, pointTrace))
            {
                bestPointTraceMatch = &pointTrace;
                id = it->first;
            }
        }
        ++it;
    }

    return id;
}

bool az::StaticPointsFilterPrivate::isValidPointTraceId(PointTraceId id) const
{
    return id != undefinedPointTraceId();
}

bool az::StaticPointsFilterPrivate::isAppropriatePoint(const MarkedPoint &markedPoint, const PointTrace &pointTrace) const
{
    const MarkedPoint &markedPointCheck = pointTrace.last();
    return markedPoint.mark != markedPointCheck.mark
            && isLocatedClosely(markedPoint, markedPointCheck);
}

bool az::StaticPointsFilterPrivate::isLocatedClosely(const MarkedPoint &markedPoint, const MarkedPoint &markedPointCheck) const
{
    return isLocatedClosely(markedPoint.point, markedPointCheck.point);
}

bool az::StaticPointsFilterPrivate::isLocatedClosely(const Point &point, const Point &pointCheck) const
{
    int xDifference = calculateXDifference(point, pointCheck);
    int yDifference = calculateYDifference(point, pointCheck);
    return std::abs(xDifference) <= settings().maxXDifference()
            && std::abs(yDifference) <= settings().maxYDifference();
}

bool az::StaticPointsFilterPrivate::isBetterMatch(const MarkedPoint &markedPoint, const PointTrace &pointTrace, const PointTrace &pointTraceCheck) const
{
    const Point &currentPoint = markedPoint.point;
    const Point &previousPoint = pointTrace.last().point;
    const Point &pointCheck = pointTraceCheck.last().point;
    double currentDistance = calculateDistance(currentPoint, previousPoint);
    double distanceCheck = calculateDistance(currentPoint, pointCheck);
    return distanceCheck < currentDistance;
}

bool az::StaticPointsFilterPrivate::isFarPointTrace(const PointTrace &pointTrace, int frame) const
{
    const MarkedPoint &markedPoint = pointTrace.last();
    return (frame - markedPoint.mark) > settings().maxPointTraceGap();
}

bool az::StaticPointsFilterPrivate::isAppropriatePointTrace(const PointTrace &pointTrace) const
{
    int pointsCount = static_cast<int>(pointTrace.points().size());
    return pointsCount >= settings().minPointTraceSize();
}

void az::StaticPointsFilterPrivate::distributePointTrace(const PointTrace &pointTrace)
{
    for (const MarkedPoint &point : pointTrace.points())
    {
        distributeMarkedPoint(point);
    }
}

void az::StaticPointsFilterPrivate::distributeMarkedPoint(const MarkedPoint &markedPoint)
{
    m_filteredPoints[markedPoint.mark].push_back(markedPoint.point);
}

double az::StaticPointsFilterPrivate::calculateDistance(const Point &point1, const Point &point2) const
{
    int xDifference = calculateXDifference(point1, point2);
    int yDifference = calculateYDifference(point1, point2);
    return std::sqrt((xDifference * xDifference) - (yDifference * yDifference));
}

int az::StaticPointsFilterPrivate::calculateXDifference(const Point &point1, const Point &point2) const
{
    return point1.x() - point2.x();
}

int az::StaticPointsFilterPrivate::calculateYDifference(const Point &point1, const Point &point2) const
{
    return point1.y() - point2.y();
}
