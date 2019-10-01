#include "branch.h"

Branch::Branch(const Contour &startContour, const TrackDetectorSettings &settings)
    : contours{startContour}, realChargeSize(settings.size), lifeTime(settings.lifeTime), delta(settings.delta) {}

bool Branch::checkCompatibility(const Contour &newContour) const
{
    return distanceBetweenPoints(newContour.getCenterMass(), contours.back().getCenterMass()) < delta ? true : false;
}

void Branch::addContour(const Contour &newContour)
{
    pairFound = true;
    contours.push_back(newContour);
    lastPoint = newContour.getCenterMass();
}

ObjectTrack Branch::getCrownCharge() const
{
    return ObjectTrack(contours);
}

int Branch::length() const
{
    return static_cast<int>(contours.size());
}

double Branch::getAverageArea() const
{
    return  std::accumulate(contours.begin(), contours.end(), 0.0, [](double sum, const Contour &val){
        return sum += val.getArea();
    });
}

bool Branch::isConfirmedCharge() const
{
    return (contours.size() >= realChargeSize) && (countToDie <= 0);
}

bool Branch::isPairFound() const
{
    return  pairFound;
}

bool Branch::isNoise() const
{
    return (contours.size() < realChargeSize) && (countToDie <= 0);
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
    return contours.front().getCenterMass();
}
