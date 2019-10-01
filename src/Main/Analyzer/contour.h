#ifndef CONTOUR_H
#define CONTOUR_H

#include <QPoint>
#include <list>

#include "opencv2/video.hpp"
#include "opencv2/highgui.hpp"

class Contour;
using Contours = std::list<Contour>;

class Contour
{
public:
    Contour(int frameNumber, const std::vector<cv::Point> &contour);

    std::vector<cv::Point> getContour() const;

    double getArea() const;

    QPoint getQPointCenterMass() const;

    cv::Point getCenterMass() const;

    int getFrameNumber() const;

    friend bool operator<(const Contour& l, const Contour& r){
        return l.getArea() < r.getArea();
    }
private:
    std::vector<cv::Point> contour;
    cv::Moments moments;
    cv::Point centerMass;
    double contourArea;
    size_t frameNumber;
};

#endif // CONTOUR_H
