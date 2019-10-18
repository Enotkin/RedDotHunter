#include "branch.h"

Branch::Branch(const cv::Point &startPoint, const TrackDetectorSettings &settings)
    : points{startPoint}, realChargeSize(settings.size), lifeTime(settings.lifeTime), delta(settings.delta) {}

bool Branch::checkCompatibility(const cv::Point &point) const
{
    auto distance = distanceBetweenPoints(point, points.back());
    return distance < delta ? true : false;
}

void Branch::addPoint(const cv::Point &point)
{
    pairFound = true;
    points.push_back(point);
    lastPoint = point;
}

std::vector<cv::Point> Branch::getPoints() const
{
    return points;
}

int Branch::length() const
{
    return static_cast<int>(points.size());
}

bool Branch::isConfirmedBranche() const
{
    return (points.size() >= realChargeSize) && (countToDie <= 0);
}

bool Branch::isPairFound() const
{
    return  pairFound;
}

bool Branch::isNoise() const
{
    return (points.size() < realChargeSize) && (countToDie <= 0);
}

void Branch::endRound()
{
    if (pairFound){
        countToDie = lifeTime;
    } else {
       countToDie--;
    }
    pairFound = false;
}

double Branch::distanceBetweenPoints(const cv::Point &first, const cv::Point &second) const
{
    return cv::norm(first - second);
}

cv::Point Branch::getLastPoint() const
{
    return lastPoint;
}

cv::Point Branch::getFistPoint() const
{
    return points.front();
}
