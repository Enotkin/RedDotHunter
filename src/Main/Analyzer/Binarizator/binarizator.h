#ifndef BINARIZATOR_H
#define BINARIZATOR_H

#include <QColor>
#include <QPair>

#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

#include "binarizatorssettings.h"

class Binarizator
{
public:
    Binarizator(){}
    virtual ~Binarizator() = default;
    virtual cv::Mat binarizeImage(const cv::Mat &src) const = 0;
};

#endif // BINARIZATOR_H
