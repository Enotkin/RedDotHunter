#ifndef BACKGROUNANALYZER_H
#define BACKGROUNANALYZER_H
#include <numeric>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"

#include "rectsubtractor.h"

class BackgroundAnalyzer
{
public:
    BackgroundAnalyzer() = default;
    bool isDarkBackground(const cv::Mat &img, const std::vector<cv::Point> &contour);

private:
    using PointGroup = std::vector<cv::Point>;
    using RectGroup = std::vector<cv::Rect>;
    RectSubtractor subtractor;
    cv::Rect imgRect;
    double maxAverageBrightness = 50;
    int rectExpandValue = 3;
    
    std::pair<cv::Rect, int> getEstimatedRect(const std::vector<cv::Point> &contour);
    double calculateAverageBrightness(const cv::Mat &img) const;
    double calculatePercentageExcessArea(const cv::Rect &mainRect, const cv::Rect &minorRect);
    bool isImageContainsRect(const cv::Rect &rect) const;
    int accumulateAreaRects(const RectGroup &rects);
    cv::Rect  expandRect(const cv::Rect &srcRect, const double expandValue);
};

#endif // BACKGROUNANALYZER_H
