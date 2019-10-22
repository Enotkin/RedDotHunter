#ifndef STATICPOINTSFILTERPRIVATE_H
#define STATICPOINTSFILTERPRIVATE_H

#include <map>
#include "Analyzer/TimeDomainFilter/pointsfilter_common.h"
#include "pointtrace.h"
#include "staticpointsfiltersettings.h"

namespace az {

class StaticPointsFilterPrivate
{
public:
    StaticPointsFilterPrivate();
    ~StaticPointsFilterPrivate();

    void run(const FramePoints &framePoints);
    const FramePoints &filteredPoints() const;
    void clear();

    const StaticPointsFilterSettings &settings() const;
    void setSettings(const StaticPointsFilterSettings &settings);

protected:
    using MarkedPoints = std::vector<MarkedPoint>;
    using PointTraceId = int;
    using PointTraces = std::map<PointTraceId, PointTrace>;

protected:
    static PointTraceId undefinedPointTraceId();

protected:
    void filterPointsAtFrame(const Points &points, int frame);
    void filterPointAtFrame(const Point &point, int frame);
    void removeFarTraces(int frame);
    void distributeLeftTraces();
    PointTraceId generatePointTraceId();
    void updatePointTrace(PointTraceId pointTraceId, const MarkedPoint &markedPoint);
    void createTrace(const MarkedPoint &markedPoint);
    PointTraceId findAppropriateTrace(const MarkedPoint &markedPoint) const;
    bool isValidPointTraceId(PointTraceId id) const;

    // mark != mark
    bool isAppropriatePoint(const MarkedPoint &markedPoint, const PointTrace &pointTrace) const;
    bool isLocatedClosely(const MarkedPoint &markedPoint, const MarkedPoint &markedPointCheck) const;
    bool isLocatedClosely(const Point &point, const Point &pointCheck) const;
    bool isBetterMatch(const MarkedPoint &markedPoint, const PointTrace &pointTrace, const PointTrace &pointTraceCheck) const;
    bool isFarPointTrace(const PointTrace &pointTrace, int frame) const;
    bool isAppropriatePointTrace(const PointTrace &pointTrace) const;
    void distributePointTrace(const PointTrace &pointTrace);
    void distributeMarkedPoint(const MarkedPoint &markedPoint);
    double calculateDistance(const Point &point1, const Point &point2) const;
    int calculateXDifference(const Point &point1, const Point &point2) const;
    int calculateYDifference(const Point &point1, const Point &point2) const;

private:
    FramePoints m_filteredPoints;
    PointTraces m_pointTraces; // temporary (used only in 'run' function)
    StaticPointsFilterSettings m_settings;
    PointTraceId m_lastPointTraceGeneratedId;
};

} // namespace az

#endif // STATICPOINTSFILTERPRIVATE_H
