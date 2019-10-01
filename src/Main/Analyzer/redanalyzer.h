#ifndef REDANALYZER_H
#define REDANALYZER_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QDebug>

#include "opencv2/imgcodecs.hpp"
#include "Binarizator/binarizator.h"
#include "contour.h"
#include "trackdetector.h"
#include "objecttrack.h"
#include "asmOpenCV.h"

struct RedAnalyzerSettings
{
    BinarizatorSettings binarizatorSettings;
    TrackDetectorSettings trackDetectorSettings;
};

class RedAnalyzer : public QObject
{
    Q_OBJECT
public:
    explicit RedAnalyzer(QObject *parent = nullptr);
    void analyze(const QFileInfoList &imagesInfo);

    Contours getContours(const QImage &image);

    std::list<ObjectTrack> getResults();

    void setSettings(const RedAnalyzerSettings settings);


signals:
    void progress(int);

private:
    QFileInfoList imagesInfo;
    RedAnalyzerSettings settings;
    std::list<Contour> objectTracks;

    std::list<Contour> searchContours (int frameIndex, const cv::Mat &img);
    std::list<ObjectTrack> tracks;
};

#endif // REDANALYZER_H
