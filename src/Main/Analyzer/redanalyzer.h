#ifndef REDANALYZER_H
#define REDANALYZER_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QDebug>

#include "opencv2/imgcodecs.hpp"
#include "Binarizator/binarizator.h"
#include "Binarizator/colorbinarizator.h"
#include "Binarizator/normalbinarizator.h"
#include "TimeDomainFilter/staticpointsfilter.h"
#include "asmOpenCV.h"
#include "backgroundanalyzer.h"

struct RedAnalyzerSettings
{
    BinarizatorsSettings binarizatorSettings;
//    TrackDetectorSettings trackDetectorSettings;
};

using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;

using Points = std::vector<cv::Point>;
using PointsPacks = std::vector<std::vector<cv::Point>>;

class RedAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RedAnalyzer(QObject *parent = nullptr);

    Contours getRedDotsCoordinate(const QFileInfoList &imagesInfo);
    PointsPacks timeFiltrate(const PointsPacks &points);
    Points getPoints(const cv::Mat &img);

    void setSettings(const RedAnalyzerSettings settings);

private:
    RedAnalyzerSettings settings;
    BackgroundAnalyzer backgroundAnalyzer;
    az::StaticPointsFilter staticPointsFilter;
    const int increaseStep = 3;
    const int maxAverageBrightness = 50;
    const int maxContourArea = 150;

    cv::Mat drawContour(const cv::Mat &img);
    cv::Mat colorBinarize (const cv::Mat &src);
    cv::Mat normalBinarize (const cv::Mat &src);
    cv::Rect expandRect(const cv::Rect &srcRect, const double expandValue);
    cv::Point getCenterPoint(const Contour &contour);
    bool checkBackground(const cv::Mat &img, const Contour &contour);
    Contours findContours(const cv::Mat &img);
    Contours getContoursContainedInRect(const cv::Rect &rect, const Contours &contours);
    Contours confirmContours(const cv::Mat &img, const Contours &normalContours, const Contours &colorContours);
    Contours filterBackground(const cv::Mat &img, const Contours &contours);
    Contours filterNearbyContours(const Contours &normalContours, const Contours &colorContours);
    Contours filterArea(const Contours &contours);
    Contours findNearbyContours(const Contour &contour, const Contours &otherContours);


    std::vector<QPoint> cvPoints2QPoints(const std::vector<cv::Point> &cvPoints);
    std::vector<cv::Point> qPoints2cvPoints(const std::vector<QPoint> &qPoints);
    std::vector<std::vector<QPoint>> cvPointsPacks2QPointsPack(const std::vector<std::vector<cv::Point>> &cvPointsPack);
    std::vector<std::vector<cv::Point>> qPointsPack2cvPointsPack(const std::vector<std::vector<QPoint>> &qPointsPack);
};

#endif // REDANALYZER_H
