#include "reddottimedomainfilter.h"


std::vector<PointGroup> RedDotTimeDomainFilter::filtrate(const std::vector<PointGroup> &points)
{
    std::vector<PointGroup> filtredPoints;
    //TODO Ну тут надо что то делать. Ибо вполне возможно что в первом паке будет не 2 точки.
    auto itFirstPack = points.begin();
    auto itSecondPack = itFirstPack + 1;
    filtredPoints.push_back(*itFirstPack);

    PointGroup secondGroup;
    for (const auto &point : *itSecondPack){
        auto nearbyPoint = getNearbyPointWhitoutUsingAverageCompare(point, *itFirstPack);
    }

    auto itBegin = points.begin() + 1;

    for (auto it = itBegin; it != points.end(); ++it){
        auto pointsAtFrame = *it;
        auto lastConfirmedPoints = filtredPoints.back();
        auto pairsNearbyPoints = getFiltredPointsAtFrame(lastConfirmedPoints, pointsAtFrame);

        PointGroup confirmedPoints;
        std::transform(std::begin(pairsNearbyPoints), std::end(pairsNearbyPoints), std::back_inserter(confirmedPoints), [](const PairPoint &pairPoints){
            return pairPoints.current;
        });

        std::vector<double> distances = getDistances(pairsNearbyPoints);
        recalculateAverageDistance(distances);


        filtredPoints.push_back(confirmedPoints);
    }
    return filtredPoints;
}

std::vector<PairPoint> RedDotTimeDomainFilter::getFiltredPointsAtFrame(const PointGroup &lastConfirmedPoints, const PointGroup &prospectivePoints)
{
    std::vector<PairPoint> filtredPointsAtFrame;
    for (const auto &point : lastConfirmedPoints){
        auto nearbyPoint = getNearbyPoint(point, prospectivePoints);
        filtredPointsAtFrame.push_back(PairPoint(nearbyPoint, point));
    }
    return filtredPointsAtFrame;
}

cv::Point RedDotTimeDomainFilter::getNearbyPoint(const cv::Point &point, const PointGroup &points)
{
    cv::Point nearbyPoint = points.front();
    for (const auto &tmpPoint : points){
        auto minDistance = calculateDistanceBetweenPoints(nearbyPoint, point);
        auto tmpDistance = calculateDistanceBetweenPoints(tmpPoint, point);
        if (minDistance < tmpDistance)
            nearbyPoint = tmpPoint;
    }
    return nearbyPoint;
}

cv::Point RedDotTimeDomainFilter::getNearbyPointWhitoutUsingAverageCompare(const cv::Point &point, const PointGroup &points)
{
    cv::Point nearbyPoint = points.front();
    for (const auto &tmpPoint : points){
        auto minDistance = calculateDistanceBetweenPoints(nearbyPoint, point);
        auto tmpDistance = calculateDistanceBetweenPoints(tmpPoint, point);
        if (minDistance < tmpDistance)
            nearbyPoint = tmpPoint;
    }
    return nearbyPoint;
}

double RedDotTimeDomainFilter::calculateDistanceBetweenPoints(const cv::Point &first, const cv::Point &second)
{
    return cv::norm(first - second);
}

void RedDotTimeDomainFilter::recalculateAverageDistance(const std::vector<double> values)
{
    std::copy(std::begin(values), std::end(values), std::back_inserter(averageArea));
    int deltaSize = maxAverageAreaSize - static_cast<int>(averageArea.size());
    if (deltaSize < 0){
        for(int i = 0; i < deltaSize; i++){
            averageArea.pop_front();
        }
    }
    auto sum = std::accumulate(std::begin(averageArea), std::end(averageArea), 0.0);
    averageDistance = sum / averageArea.size();
}

std::vector<double> RedDotTimeDomainFilter::getDistances(const std::vector<PairPoint> &pairs)
{
    std::vector<double> distances;
    std::transform(std::begin(pairs), std::end(pairs), std::back_inserter(distances), [this](const PairPoint &pairPoints){
        return calculateDistanceBetweenPoints(pairPoints.current, pairPoints.previous);
    });
    return distances;
}
