#ifndef BINARIZATOR_H
#define BINARIZATOR_H

#include <QColor>
#include <QPair>

#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

#include "binarizatorsettings.h"

class Binarizator
{
public:
    Binarizator();
    Binarizator(BinarizatorSettings settings);
    cv::Mat getImage(const cv::Mat &src);

    void setSettings(const BinarizatorSettings &value);

private:
    BinarizatorSettings settings;
    double thresholdValue = 225;

    cv::Mat binarizationColor(const cv::Mat &src);
    cv::Mat binarizationNormal(const cv::Mat &src);

    std::tuple<cv::Scalar, cv::Scalar> getHsvRanges();
    std::pair<int, int> getRange(int channel, const QPair<int, int> &range, int maxValue = 255);
};

#endif // BINARIZATOR_H
