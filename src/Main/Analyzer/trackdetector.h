#ifndef CROWNCHARGEDETECTOR_H
#define CROWNCHARGEDETECTOR_H

#include "contour.h"
#include <set>
#include <algorithm>
#include "branch.h"
#include "objecttrack.h"
#include "branchselector.h"

class TrackDetector
{
public:
    TrackDetector();
    TrackDetector(const TrackDetectorSettings &settings);
    ~TrackDetector();

    void findTrack(std::list<Contour> &contours);
    std::list<ObjectTrack> getTracks() const;
    void setSettings(TrackDetectorSettings settings);

private:
    std::list<ObjectTrack> lastDetectedCharges;
    std::list<ObjectTrack> detectedCharges;
    std::list<Branch> branches;
    TrackDetectorSettings suspetctSettings;

    std::list<BranchSelector> createSelectors(const std::list<Contour> &contours);
    void branchDistribution(std::list<BranchSelector> &selectorList, std::list<Branch> &branchList);
    void copyConfirmedCharges(const std::list<Branch> &branches);

    bool isConfirmed(const Branch &branch);

};

#endif // CROWNCHARGEDETECTOR_H
