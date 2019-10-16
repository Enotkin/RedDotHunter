#include "contour.h"

Contour::Contour(int frameNumber, const std::vector<cv::Point> &contour) : contour(contour), frameNumber(frameNumber)
{
    moments = cv::moments(contour);
    contourArea = cv::contourArea(contour);
    centerMass = cv::Point(static_cast<int>(moments.m10/moments.m00),
                           static_cast<int>(moments.m01/moments.m00));
}

Contour::Contour(const std::vector<cv::Point> &contour) : contour(contour)
{
    moments = cv::moments(contour);
    contourArea = cv::contourArea(contour);
    centerMass = cv::Point(static_cast<int>(moments.m10/moments.m00),
                           static_cast<int>(moments.m01/moments.m00));
}

std::vector<cv::Point> Contour::getContour() const
{
    return contour;
}

double Contour::getArea() const
{
    return contourArea;
}

QPoint Contour::getQPointCenterMass() const
{
    return QPoint(static_cast<int>(moments.m10/moments.m00),
                  static_cast<int>(moments.m01/moments.m00));
}

cv::Point Contour::getCenterMass() const
{
    return centerMass;
}

int Contour::getFrameNumber() const
{
    return frameNumber;
}
