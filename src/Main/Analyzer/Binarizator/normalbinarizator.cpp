#include "normalbinarizator.h"

NormalBinarizator::NormalBinarizator() {}

NormalBinarizator::NormalBinarizator(const NormalBinarizatorSettings &settings) :
    tresholdValue(settings.tresholdValue){}

cv::Mat NormalBinarizator::binarizeImage(const cv::Mat &src) const
{
    cv::Mat srcGray, temp, temp2, dst;
    cv::cvtColor(src, srcGray, cv::COLOR_BGR2GRAY);
    cv::threshold(srcGray, dst, tresholdValue, 255, cv::THRESH_BINARY);
    return dst;
}

void NormalBinarizator::setSettings(const NormalBinarizatorSettings &settings)
{
    tresholdValue = settings.tresholdValue;
}
