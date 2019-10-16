#ifndef RECTSUBTRACTOR_H
#define RECTSUBTRACTOR_H
#include <numeric>
#include "opencv2/highgui.hpp"

#include <QRect>
class RectSubtractor
{
public:
    RectSubtractor() = default;
    std::vector<cv::Rect> subtractRect(const cv::Rect &minued, const cv::Rect &subtrahend);

private:
    using PointGroup = std::vector<cv::Point>;
    using RectGroup = std::vector<cv::Rect>;
    cv::Rect imgRect;

    PointGroup getPointsOutOfArea(const cv::Rect &majorRect, const cv::Rect &minorRect) const;

    RectGroup getCornerRects (const PointGroup &points, const cv::Rect &minorRect, const cv::Rect &crossingRect);
    cv::Rect getSideRect(const PointGroup &points, const cv::Rect &minorRect, const cv::Rect &crossingRect);

    RectGroup assambleRectGroup(const cv::Rect &rect);

    bool isTwoPointBottom(const PointGroup &points);
    bool isTwoPointTop(const PointGroup &points);
    bool isTwoPointRight(const PointGroup &points);
    bool isTwoPointLeft(const PointGroup &points);

    RectGroup getTopRightCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    RectGroup getTopLeftCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    RectGroup getBottomRightCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    RectGroup getBottomLeftCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect);

    cv::Rect getRightRect(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    cv::Rect getTopRect(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    cv::Rect getBottomRect(const cv::Rect &minorRect, const cv::Rect &crossingRect);
    cv::Rect getLeftRect(const cv::Rect &minorRect, const cv::Rect &crossingRect);

};

#endif // RECTSUBTRACTOR_H
