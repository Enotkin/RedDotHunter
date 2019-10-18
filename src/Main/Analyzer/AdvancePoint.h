#ifndef ADVANCEPOINT_H
#define ADVANCEPOINT_H
#include "opencv2/imgcodecs.hpp"

class AdvancePoint : public cv::Point {
    int frameNumber = 0;
};

#endif // ADVANCEPOINT_H
