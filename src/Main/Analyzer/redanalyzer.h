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
#include "trackdetector.h"
#include "objecttrack.h"
#include "asmOpenCV.h"
#include "backgroundanalyzer.h"

struct RedAnalyzerSettings
{
    BinarizatorsSettings binarizatorSettings;
    TrackDetectorSettings trackDetectorSettings;
};

class RedAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RedAnalyzer(QObject *parent = nullptr);

    std::vector<std::vector<cv::Point> > getRedDotsCoordinate(const QFileInfoList &imagesInfo);
    std::vector<cv::Point> getPoints(const cv::Mat &img);

    cv::Mat colorBinarize (const cv::Mat &src);
    cv::Mat normalBinarize (const cv::Mat &src);

    bool checkBackground(const cv::Mat &img, const std::vector<cv::Point> &contour);

    std::vector<std::vector<cv::Point> > confirmContours(const cv::Mat &img, const std::vector<std::vector<cv::Point> > &normalContours, const std::vector<std::vector<cv::Point> > &colorContours);

    cv::Mat drawContour(const cv::Mat &img);

//    Contours getContours(const QImage &image);

    void setSettings(const RedAnalyzerSettings settings);


signals:
    void progress(int);

private:
    QFileInfoList imagesInfo;
    RedAnalyzerSettings settings;
    BackgroundAnalyzer backgroundAnalyzer;
    std::list<Contour> objectTracks;
    const int increaseStep = 3;
    const int maxAverageBrightness = 50;

    std::vector<std::vector<cv::Point>> findContours(const cv::Mat &img);

    std::list<ObjectTrack> tracks;
    
    std::vector<std::vector<cv::Point>> getContoursContainedInRect(const cv::Rect &rect, const std::vector<std::vector<cv::Point> > &contours);

    cv::Rect  expandRect(const cv::Rect &srcRect, const double expandValue);
};

#endif // REDANALYZER_H
