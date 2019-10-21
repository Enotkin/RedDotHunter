#ifndef REDDOTTIMEDOMAINFILTER_H
#define REDDOTTIMEDOMAINFILTER_H

#include "opencv2/imgcodecs.hpp"
#include <queue>
#include <numeric>

using PointGroup = std::vector<cv::Point>;
using PointIterator = std::vector<cv::Point>::iterator;

struct PairPoint
{
    PairPoint(const cv::Point &current, const cv::Point &previous) : current(current), previous(previous) {}
    const cv::Point current;
    const cv::Point previous;
};

class RedDotTimeDomainFilter
{
public:
    RedDotTimeDomainFilter() = default;
    std::vector<PointGroup> filtrate(const std::vector<PointGroup> &points);

private:
    std::vector<PairPoint> getFiltredPointsAtFrame(const PointGroup &lastConfirmedPoints, const PointGroup &prospectivePoints);
    cv::Point getNearbyPoint(const cv::Point &point, const PointGroup &points);
    cv::Point getNearbyPointWhitoutUsingAverageCompare(const cv::Point &point, const PointGroup &points);
    double calculateDistanceBetweenPoints(const cv::Point &first, const cv::Point &second);
    void recalculateAverageDistance(const std::vector<double> values);
    std::vector<double> getDistances(const std::vector<PairPoint> &pairs);

    double averageDistance = 0;
    double delta = 0;
    int maxAverageAreaSize = 60;

    std::deque <double> averageArea;
};

#endif // REDDOTTIMEDOMAINFILTER_H
