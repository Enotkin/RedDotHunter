#include "rectsubtractor.h"

std::vector<cv::Rect> RectSubtractor::subtractRect(const cv::Rect &minued, const cv::Rect &subtrahend)
{
    auto pointsOutOfArea = getPointsOutOfArea(minued, subtrahend);
    auto crossingRect = minued & subtrahend;

    if (pointsOutOfArea.size() == 3){
        RectGroup excessRects = getCornerRects(pointsOutOfArea, subtrahend, crossingRect);
        return excessRects;

    } else if (pointsOutOfArea.size() == 2) {
        cv::Rect excessRect = getSideRect(pointsOutOfArea, subtrahend, crossingRect);
        return assambleRectGroup(excessRect);

    } else {
        return RectGroup();
    }
}

RectSubtractor::RectGroup RectSubtractor::assambleRectGroup(const cv::Rect &rect)
{
    if (!rect.empty()) {
        return RectGroup{rect};
    } else {
        return RectGroup();
    }
}

RectSubtractor::PointGroup RectSubtractor::getPointsOutOfArea(const cv::Rect &majorRect, const cv::Rect &minorRect) const
{
    std::vector<cv::Point> points {minorRect.tl(),
                                   minorRect.br(),
                                   cv::Point(minorRect.x + minorRect.width, minorRect.y),
                                   cv::Point(minorRect.x, minorRect.y + minorRect.height)};

    std::vector<cv::Point> pointsOutOfArea(points.size());

    auto isContains = [&majorRect](const cv::Point &point){
        return majorRect.contains(point);
    };

    std::copy_if(points.begin(), points.end(), pointsOutOfArea.begin(), isContains);

    return pointsOutOfArea;
}

RectSubtractor::RectGroup RectSubtractor::getCornerRects(const RectSubtractor::PointGroup &points, const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    if (isTwoPointRight(points) && isTwoPointTop(points)){
        return getTopRightCornerRects(minorRect, crossingRect);

    } else if (isTwoPointLeft(points) && isTwoPointTop(points)){
        return getTopLeftCornerRects(minorRect, crossingRect);

    } else if (isTwoPointRight(points) && isTwoPointBottom(points)){
        return getBottomRightCornerRects(minorRect, crossingRect);

    } else if (isTwoPointLeft(points) && isTwoPointBottom(points)){
        return getBottomLeftCornerRects(minorRect, crossingRect);

    } else {
        return RectGroup();
    }
}

cv::Rect RectSubtractor::getSideRect(const RectSubtractor::PointGroup &points, const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    if (isTwoPointRight(points)){
        return getRightRect(minorRect, crossingRect);

    } else  if (isTwoPointLeft(points)){
        return getLeftRect(minorRect, crossingRect);

    } else if (isTwoPointTop(points)){
        return getTopRect(minorRect, crossingRect);

    } else if (isTwoPointBottom(points)){
        return getBottomRect(minorRect, crossingRect);

    } else {
        return cv::Rect();
    }
}

bool RectSubtractor::isTwoPointBottom(const RectSubtractor::PointGroup &points)
{
    return std::count_if(points.begin(), points.end(), [this](const cv::Point &point){return point.y > imgRect.height;}) == 2;
}

bool RectSubtractor::isTwoPointTop(const RectSubtractor::PointGroup &points)
{
    return std::count_if(points.begin(), points.end(), [](const cv::Point &point){return point.y < 0;}) == 2;
}

bool RectSubtractor::isTwoPointRight(const RectSubtractor::PointGroup &points)
{
    return std::count_if(points.begin(), points.end(), [this](const cv::Point &point){return point.x > imgRect.width;}) == 2;
}

bool RectSubtractor::isTwoPointLeft(const RectSubtractor::PointGroup &points)
{
    return std::count_if(points.begin(), points.end(), [](const cv::Point &point){return point.x < 0;}) == 2;
}

RectSubtractor::RectGroup RectSubtractor::getTopRightCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Rect topRect = getTopRect(minorRect, crossingRect);
    cv::Rect rightRect = getRightRect(minorRect, crossingRect);

    cv::Point middleTopLeft = cv::Point(topRect.x + topRect.width, topRect.y);
    cv::Size middleRectSize = cv::Size(rightRect.width, topRect.height);

    cv::Rect middleRect(middleTopLeft, middleRectSize);
    return RectGroup{topRect, middleRect, rightRect};
}

RectSubtractor::RectGroup RectSubtractor::getTopLeftCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Rect topRect = getTopRect(minorRect, crossingRect);
    cv::Rect leftRect = getLeftRect(minorRect, crossingRect);

    cv::Point middleTopLeft = cv::Point(leftRect.x, leftRect.y - topRect.height);
    cv::Size middleRectSize = cv::Size(leftRect.width, topRect.height);

    cv::Rect middleRect(middleTopLeft, middleRectSize);
    return RectGroup{topRect, middleRect, leftRect};
}

RectSubtractor::RectGroup RectSubtractor::getBottomRightCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Rect rightRect = getRightRect(minorRect, crossingRect);
    cv::Rect bottomRect = getBottomRect(minorRect, crossingRect);

    cv::Point middleTopLeft = cv::Point(bottomRect.x + bottomRect.width, bottomRect.y);
    cv::Size middleRectSize = cv::Size(rightRect.width, bottomRect.height);

    cv::Rect middleRect(middleTopLeft, middleRectSize);
    return RectGroup{bottomRect, middleRect, rightRect};
}

RectSubtractor::RectGroup RectSubtractor::getBottomLeftCornerRects(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Rect bottomRect = getBottomRect(minorRect, crossingRect);
    cv::Rect leftRect = getLeftRect(minorRect, crossingRect);

    cv::Point middleTopLeft = cv::Point(leftRect.x, leftRect.y - leftRect.height);
    cv::Size middleRectSize = cv::Size(leftRect.width, bottomRect.height);

    cv::Rect middleRect(middleTopLeft, middleRectSize);
    return RectGroup{bottomRect, middleRect, leftRect};
}

cv::Rect RectSubtractor::getRightRect(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Point tl = cv::Point(crossingRect.x + crossingRect.width, crossingRect.y);
    cv::Size size = cv::Size(minorRect.width - crossingRect.width, crossingRect.height);
    return cv::Rect(tl, size);
}

cv::Rect RectSubtractor::getTopRect(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Point tl = cv::Point(crossingRect.x, minorRect.y);
    cv::Size size = cv::Size(crossingRect.width, minorRect.height - crossingRect.height);
    return cv::Rect(tl, size);
}

cv::Rect RectSubtractor::getBottomRect(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Point tl = cv::Point(crossingRect.x, crossingRect.y + crossingRect.height);
    cv::Size size = cv::Size(crossingRect.width, minorRect.height - crossingRect.height);
    return cv::Rect(tl, size);
}

cv::Rect RectSubtractor::getLeftRect(const cv::Rect &minorRect, const cv::Rect &crossingRect)
{
    cv::Point tl = cv::Point(crossingRect.x - (minorRect.width - crossingRect.width), crossingRect.y);
    cv::Size size = cv::Size((minorRect.width - crossingRect.width), crossingRect.height);
    return cv::Rect(tl, size);
}
