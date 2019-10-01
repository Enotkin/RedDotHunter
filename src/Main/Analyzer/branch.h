#ifndef SUSPECTCROWNCHARGE_H
#define SUSPECTCROWNCHARGE_H
#include <vector>
#include <list>
#include <numeric>

#include "contour.h"
#include "objecttrack.h"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "trackdetectorsettings.h"


class Branch
{
public:
    Branch (const Contour &startContour, const TrackDetectorSettings &settings = TrackDetectorSettings());

    bool checkCompatibility(const Contour& newContour) const;
    void addContour(const Contour &newContour);
    ObjectTrack getCrownCharge() const;
    int length() const;
    double getAverageArea() const;
    bool isConfirmedCharge() const;
    bool isPairFound() const;
    bool isNoise() const;
    void endRound();


    friend bool operator<(const Branch& l, const Branch& r){
        return l.length() < r.length();
    }

    cv::Point getLastPoint() const;
    cv::Point getFistPoint() const;

private:
    std::list<Contour> contours;
    std::vector<Contour> tempContours;
    size_t realChargeSize = 2;
    int lifeTime = 5;
    double delta = 10;
    int countToDie = lifeTime;
    double distanceBetweenPoints(const cv::Point &first, const cv::Point &second) const;
    bool pairFound = false;
    cv::Point lastPoint;

};

#endif // SUSPECTCROWNCHARGE_H
