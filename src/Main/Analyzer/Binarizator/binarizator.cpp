#include "binarizator.h"
#include <QDebug>

Binarizator::Binarizator() : settings(BinarizatorSettings()) {}

Binarizator::Binarizator(BinarizatorSettings settings) : settings(settings) {}

cv::Mat Binarizator::getImage(const cv::Mat &src)
{
    switch (settings.mode) {
    case BinarizatorSettings::FilterType::Normal:
        return binarizationNormal(src);
    case BinarizatorSettings::FilterType::Colorfull:
        return binarizationColor(src);
    }
}

void Binarizator::setSettings(const BinarizatorSettings &value)
{
    settings = value;
}

cv::Mat Binarizator::binarizationColor(const cv::Mat &src)
{
    cv::Mat dst, frame_HSV, frame_HSV_2;
    auto [lowHsv, hightHsv] = getHsvRanges();

    cv::cvtColor(src, frame_HSV, cv::COLOR_BGR2HSV);
            frame_HSV_2 =frame_HSV;


    cv::inRange(frame_HSV, lowHsv, hightHsv, frame_HSV_2);

    cv::Mat hsv_channels[3];
    cv::split(frame_HSV_2, hsv_channels);
    return hsv_channels[0];
}

cv::Mat Binarizator::binarizationNormal(const cv::Mat &src)
{
    cv::Mat srcGray, temp, temp2, dst;
    cv::cvtColor(src, srcGray, cv::COLOR_BGR2GRAY);
    cv::threshold(srcGray, dst, 128, 255, cv::THRESH_BINARY);
    return dst;
}

std::tuple<cv::Scalar, cv::Scalar> Binarizator::getHsvRanges()
{
    auto color = settings.color.toHsv();
    auto coefficients = settings.coefficientHsv;
    auto hueRange = getRange(color.hue()/2, coefficients.hue, 179);
    auto saturation = getRange(color.saturation(), coefficients.saturation);
    auto value = getRange(color.value(), coefficients.value);
//    qDebug()<< "Low" << hueRange.first << saturation.first <<  value.first;
//    qDebug()<< "Hight" << hueRange.second << saturation.second << value.second;
    cv::Scalar lowHsv (hueRange.first, saturation.first, value.first);
    cv::Scalar hightHsv (hueRange.second, saturation.second, value.second);
    return std::make_tuple(lowHsv, hightHsv);
}

std::pair<int, int> Binarizator::getRange(int channel, const QPair<int, int> &range, int maxValue)
{
    int min = channel - range.first;
    if (min < 0){
       min = channel;
    }
    int max = channel + range.second;
    if (max > maxValue)
        max = maxValue;
    return std::make_pair(min, max);
}
