#ifndef SUSPECTCROWNCHARGE_H
#define SUSPECTCROWNCHARGE_H
#include <vector>
#include <list>
#include <numeric>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "trackdetectorsettings.h"


class Branch
{
public:
    Branch (const cv::Point &startPoint, const TrackDetectorSettings &settings = TrackDetectorSettings());

    bool checkCompatibility(const cv::Point &point) const;
    void addPoint(const cv::Point &point);
    std::vector<cv::Point> getPoints() const;
    int length() const;
    bool isConfirmedBranche() const;
    bool isPairFound() const;
    bool isNoise() const;
    void endRound();


    friend bool operator<(const Branch& l, const Branch& r){
        return l.length() < r.length();
    }

    cv::Point getLastPoint() const;
    cv::Point getFistPoint() const;

private:
    std::vector<cv::Point> points;
    size_t realChargeSize = 2;
    int lifeTime = 5;
    double delta = 10;
    int countToDie = lifeTime;
    double distanceBetweenPoints(const cv::Point &first, const cv::Point &second) const;
    bool pairFound = false;
    cv::Point lastPoint;

};

#endif // SUSPECTCROWNCHARGE_H
