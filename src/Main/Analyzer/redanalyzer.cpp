#include "redanalyzer.h"

RedAnalyzer::RedAnalyzer(QObject *parent) : QObject(parent)
{

}

std::vector<std::vector<cv::Point>> RedAnalyzer::getRedDotsCoordinate(const QFileInfoList &imagesInfo)
{
    std::vector<std::vector<cv::Point>> framePoints;
    for(const auto &imgInfo : imagesInfo){
        auto matImg = cv::imread(imgInfo.absoluteFilePath().toStdString());
        auto points = getPoints(matImg);
        framePoints.push_back(points);
        qDebug()<<imgInfo.fileName();
    }
    return framePoints;
}


std::vector<cv::Point> RedAnalyzer::getPoints(const cv::Mat &img)
{
    auto normalBinImage = normalBinarize(img);
    auto colorBinImage = colorBinarize(img);

    cv::morphologyEx(colorBinImage, colorBinImage, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)), cv::Point(-1, -1), 2);

    auto normalContours = findContours(normalBinImage);
    auto colorContours = findContours(colorBinImage);
    auto confirmedContours = confirmContours(img, normalContours, colorContours);
    std::vector<cv::Point> resultPoints;
    std::transform(std::begin(confirmedContours), std::end(confirmedContours), std::back_inserter(resultPoints),
                   [](const auto &contour){
       Contour contr(contour);
       return contr.getCenterMass();
    });
    return resultPoints;
}

std::vector<std::vector<cv::Point>> RedAnalyzer::confirmContours(const cv::Mat &img,
                                                                 const std::vector<std::vector<cv::Point>> &normalContours,
                                                                 const std::vector<std::vector<cv::Point>> &colorContours)
{
    std::vector<std::vector<cv::Point>> confirmedContours;
    for (const auto &contour : normalContours){
        auto rect = cv::boundingRect(contour);
        auto increasedRect = expandRect(rect, increaseStep);
        auto nearbyContours = getContoursContainedInRect(increasedRect, colorContours);
        if (!nearbyContours.empty() && checkBackground(img, contour))
            confirmedContours.push_back(contour);
    }
    return confirmedContours;
}

std::vector<std::vector<cv::Point>> RedAnalyzer::getContoursContainedInRect(const cv::Rect &rect, const std::vector<std::vector<cv::Point>> &contours)
{
    std::vector<std::vector<cv::Point>> nearbyContours;
    std::copy_if(std::begin(contours), std::end(contours), std::back_inserter(nearbyContours),
                 [&rect](const auto &contour){
        Contour contr(contour);
        return rect.contains(contr.getCenterMass());
    });
    return nearbyContours;
}

cv::Rect RedAnalyzer::expandRect(const cv::Rect &srcRect, const double expandValue)
{
    double step = expandValue - 1;
    if (step <= 0)
        step = 0.1;
    cv::Rect increasedRect = srcRect + cv::Size(static_cast<int>(srcRect.width * step),static_cast<int>(srcRect.height * step));
    return increasedRect;
}

cv::Mat RedAnalyzer::colorBinarize(const cv::Mat &src)
{
    ColorBinarizator colorBinarizator;
    ColorBinarizatorSettings colorBinarizatorSettings;
    {
        HsvMargins margins(50, 70, 70);
        colorBinarizatorSettings.hsvMargin = margins;
        colorBinarizatorSettings.color = QColor::fromHsv(0, 100, 100);
    }
    colorBinarizator.setSettings(colorBinarizatorSettings);
    return colorBinarizator.binarizeImage(src);
}

cv::Mat RedAnalyzer::normalBinarize(const cv::Mat &src)
{
    NormalBinarizator normalBinarizator(NormalBinarizatorSettings{128});
    return normalBinarizator.binarizeImage(src);
}

bool RedAnalyzer::checkBackground(const cv::Mat &img, const std::vector<cv::Point> &contour)
{
    return backgroundAnalyzer.isDarkBackground(img, contour);
}



void RedAnalyzer::setSettings(const RedAnalyzerSettings settings)
{
    this->settings = settings;
}

std::vector<std::vector<cv::Point>> RedAnalyzer::findContours(const cv::Mat &img)
{
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(img, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    return contours;
}


cv::Mat RedAnalyzer::drawContour(const cv::Mat &img)
{
    auto contours = findContours(img);
    cv::Mat resultImg = img;
    for (const auto &contour : contours){
        auto rect = cv::boundingRect(contour);
        cv::rectangle(resultImg, cv::Point(rect.x-5,rect.y-5), cv::Point(rect.x+5+rect.width,rect.y+5+rect.height), cv::Scalar(255, 255, 255), 1);
    }
//    cv::drawContours(resultImg, contours, -1, cv::Scalar(0, 255, 0), 3);
    return resultImg;
}


