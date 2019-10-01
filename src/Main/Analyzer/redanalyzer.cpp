#include "redanalyzer.h"

RedAnalyzer::RedAnalyzer(QObject *parent) : QObject(parent)
{

}

void RedAnalyzer::analyze(const QFileInfoList &imagesInfo)
{
    TrackDetector detector (settings.trackDetectorSettings);
    Binarizator binarizator (settings.binarizatorSettings);
    for(int frameIndex = 0; frameIndex < imagesInfo.size(); frameIndex++){
        auto matImage = cv::imread(imagesInfo.at(frameIndex).absoluteFilePath().toStdString());
        auto binImage = binarizator.getImage(matImage);
        auto contours = searchContours(frameIndex, binImage);
        detector.findTrack(contours);
    }
    auto tracks = detector.getTracks();
}

Contours RedAnalyzer::getContours(const QImage &image)
{
    Binarizator bin(settings.binarizatorSettings);
    auto matImage = ASM::QImageToCvMat(image);
    auto binImage = bin.getImage(matImage);
    auto contours = searchContours(0, binImage);
}

std::list<ObjectTrack> RedAnalyzer::getResults()
{
    return tracks;
}

void RedAnalyzer::setSettings(const RedAnalyzerSettings settings)
{
    this->settings = settings;
}

std::list<Contour> RedAnalyzer::searchContours(int frameIndex, const cv::Mat &img)
{
    std::list<Contour> contours;
    std::vector<std::vector<cv::Point>> srcContours, dstContours;
    cv::findContours(img, srcContours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    std::copy_if(srcContours.begin(), srcContours.end(), std::back_inserter(dstContours),
                 [](const auto &contour){ return (cv::contourArea(contour, false) > 5.0) ? true : false; });
    for (const auto &contour : dstContours) {
        contours.emplace_back(frameIndex, contour);
    }
    return contours;
}
