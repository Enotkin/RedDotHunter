#ifndef CROWNCHARGEDETECTOR_H
#define CROWNCHARGEDETECTOR_H

#include <set>
#include <algorithm>
#include "branchselector.h"

class TrackDetector
{
public:
    TrackDetector();
    TrackDetector(const TrackDetectorSettings &settings);
    ~TrackDetector();

    std::vector<std::vector<cv::Point> > separatePoints(const std::vector<std::vector<cv::Point>> &points);
    void setSettings(TrackDetectorSettings settings);

private:
    std::vector<cv::Point> detectedCharges;
    std::vector<Branch> branches;
    TrackDetectorSettings suspetctSettings;

    std::vector<BranchSelector> createSelectors(const std::vector<cv::Point> &points);
    void branchDistribution(std::vector<BranchSelector> &selectorList, std::vector<Branch> &branchList);
};

#endif // CROWNCHARGEDETECTOR_H
