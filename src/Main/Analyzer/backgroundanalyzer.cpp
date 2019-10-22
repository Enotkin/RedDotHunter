#include "backgroundanalyzer.h"

bool BackgroundAnalyzer::isDarkBackground(const cv::Mat &img, const std::vector<cv::Point> &contour)
{
    imgRect = cv::Rect(0, 0, img.cols, img.rows);
    auto [estimatedRect, difference] = getEstimatedRect(contour);
    auto croppedImage = img(estimatedRect);
    auto averageBrightness = calculateAverageBrightness(croppedImage) * (1 - difference);
    return averageBrightness < maxAverageBrightness ? true : false;
}

std::pair<cv::Rect, int> BackgroundAnalyzer::getEstimatedRect(const std::vector<cv::Point> &contour)
{
    auto rect = cv::boundingRect(contour);
    auto increasedRect = expandRect(rect, rectExpandValue);
    if (isImageContainsRect(increasedRect)){
        return {increasedRect, 0};
    } else {
        cv::Rect availableRect = increasedRect & imgRect;
        auto difference = calculatePercentageExcessArea(imgRect, increasedRect);
        return {availableRect, difference};
    }
}

double BackgroundAnalyzer::calculateAverageBrightness(const cv::Mat &img) const
{
    auto grayImg = img;
    cv::cvtColor(img, grayImg, cv::COLOR_BGR2GRAY);
    auto average = cv::mean(grayImg);
    return average[0];
}

bool BackgroundAnalyzer::isImageContainsRect(const cv::Rect &rect) const
{
    auto resultREct = rect & imgRect;
    return resultREct == rect;
}

double BackgroundAnalyzer::calculatePercentageExcessArea(const cv::Rect &mainRect, const cv::Rect &minorRect)
{
    auto excessRects = subtractor.subtractRect(mainRect, minorRect);
    int excessArea = accumulateAreaRects(excessRects);
    int rectArea = minorRect.area();
    double difference = excessArea / rectArea;
    return difference;
}

int BackgroundAnalyzer::accumulateAreaRects(const BackgroundAnalyzer::RectGroup &rects)
{
    if (rects.size() == 1){
        return rects.front().area();
    }

    int areaSum = 0;
    for (const auto &rect : rects) {
        areaSum += rect.area();
    }
    return areaSum;
}

cv::Rect BackgroundAnalyzer::expandRect(const cv::Rect &srcRect, const double expandValue)
{
    double step = expandValue - 1;
    if (step <= 0)
        step = 0.1;
    cv::Rect increasedRect = srcRect + cv::Size(static_cast<int>(srcRect.width * step),static_cast<int>(srcRect.height * step));
    return increasedRect;
}





























