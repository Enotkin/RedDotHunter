#include "colorbinarizator.h"

ColorBinarizator::ColorBinarizator(){}

ColorBinarizator::ColorBinarizator(const ColorBinarizatorSettings &settings) :
    color(settings.color.toHsv()), margins(settings.hsvMargin) {}

cv::Mat ColorBinarizator::binarizeImage(const cv::Mat &src) const
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

void ColorBinarizator::setSettings(const ColorBinarizatorSettings &settings)
{
    color = settings.color.toHsv();
    margins = settings.hsvMargin;
}

std::pair<cv::Scalar, cv::Scalar> ColorBinarizator::getHsvRanges() const
{
    auto hueRange = getRange(color.hue()/2, margins.hue, 179);
    auto saturation = getRange(color.saturation(), margins.saturation);
    auto value = getRange(color.value(), margins.value);

    cv::Scalar lowHsv (hueRange.first, saturation.first, value.first);
    cv::Scalar hightHsv (hueRange.second, saturation.second, value.second);

    return {lowHsv, hightHsv};
}

std::pair<int, int> ColorBinarizator::getRange(int channel, const QPair<int, int> &range, int maxValue) const
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
