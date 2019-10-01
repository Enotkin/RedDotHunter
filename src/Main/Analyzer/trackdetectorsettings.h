#ifndef BRANCHSETTINGS_H
#define BRANCHSETTINGS_H

struct TrackDetectorSettings{
    TrackDetectorSettings() : size(3), lifeTime(5), delta(75) {}
    TrackDetectorSettings(int size, int lifeTime, double delta) :
        size(size), lifeTime(lifeTime), delta(delta) {}

    int size;
    int lifeTime;
    double delta;
};

#endif // BRANCHSETTINGS_H
