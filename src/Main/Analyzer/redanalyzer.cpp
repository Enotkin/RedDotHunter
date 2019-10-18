#include "redanalyzer.h"

RedAnalyzer::RedAnalyzer(QObject *parent) : QObject(parent)
{

}

Contours RedAnalyzer::getRedDotsCoordinate(const QFileInfoList &imagesInfo)
{
    std::vector<std::vector<cv::Point>> framePoints;
    for(const auto &imgInfo : imagesInfo){
        auto matImg = cv::imread(imgInfo.absoluteFilePath().toStdString());
        auto points = getPoints(matImg);
        framePoints.push_back(points);
        qDebug()<<imgInfo.fileName();
    }
    int findedPoints = 0;
    for(const auto &points : framePoints){
        findedPoints += static_cast<int>(points.size());
    }
    qDebug()<<"Всего найдено точек" << findedPoints;
    return framePoints;
}


Points RedAnalyzer::getPoints(const cv::Mat &img)
{
    auto normalBinImage = normalBinarize(img);
    auto colorBinImage = colorBinarize(img);

    cv::morphologyEx(colorBinImage, colorBinImage, cv::MORPH_DILATE, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3)), cv::Point(-1, -1), 2);

    auto normalContours = findContours(normalBinImage);
    auto colorContours = findContours(colorBinImage);
    auto confirmedContours = confirmContours(img, normalContours, colorContours);
    std::vector<cv::Point> resultPoints;
    std::transform(std::begin(confirmedContours), std::end(confirmedContours), std::back_inserter(resultPoints),
                   [this](const auto &contour){
        return getCenterPoint(contour);
    });
    return resultPoints;
}

Contours RedAnalyzer::confirmContours(const cv::Mat &img, const Contours &normalContours, const Contours &colorContours)
{
    std::vector<std::vector<cv::Point>> confirmedContours = filterArea(normalContours);
    confirmedContours = filterNearbyContours(confirmedContours, colorContours);
    confirmedContours = filterBackground(img, confirmedContours);
    return confirmedContours;
}

Contours RedAnalyzer::filterBackground(const cv::Mat &img, const Contours &contours)
{
    std::vector<std::vector<cv::Point>> filtredContours;
    std::copy_if(contours.begin(), contours.end(), std::back_inserter(filtredContours), [&img, this](const auto &contour){
        return checkBackground(img, contour);
    });
    return filtredContours;
}

Contours RedAnalyzer::filterNearbyContours(const Contours &normalContours, const Contours &colorContours)
{
    std::vector<std::vector<cv::Point>> filtredContours;
    std::copy_if(normalContours.begin(), normalContours.end(), std::back_inserter(filtredContours), [this, &colorContours](const auto &contour){
        auto nearbyContours = findNearbyContours(contour, colorContours);
        return !nearbyContours.empty();
    });
    return filtredContours;
}

Contours RedAnalyzer::findNearbyContours(const Contour &contour, const Contours &otherContours)
{
    auto rect = cv::boundingRect(contour);
    auto increasedRect = expandRect(rect, increaseStep);
    auto nearbyContours = getContoursContainedInRect(increasedRect, otherContours);
    return nearbyContours;
}

Contours RedAnalyzer::filterArea(const Contours &contours)
{
    std::vector<std::vector<cv::Point>> filtredContours;
    std::copy_if(contours.begin(), contours.end(), std::back_inserter(filtredContours), [&](const auto &contour){
        return cv::contourArea(contour) <= maxContourArea;
    });
    return filtredContours;
}

Contours RedAnalyzer::getContoursContainedInRect(const cv::Rect &rect, const Contours &contours)
{
    std::vector<std::vector<cv::Point>> nearbyContours;
    auto isContourInRect = [this, &rect](const auto &contour)->bool{
        auto centerPoint = getCenterPoint(contour);
        return rect.contains(centerPoint);
    };
    std::copy_if(std::begin(contours), std::end(contours), std::back_inserter(nearbyContours), isContourInRect);
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

cv::Point RedAnalyzer::getCenterPoint(const Contour &contour)
{
    auto moments = cv::moments(contour);
    auto centerPoint = cv::Point(static_cast<int>(moments.m10/moments.m00),
                                 static_cast<int>(moments.m01/moments.m00));
    return centerPoint;
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

bool RedAnalyzer::checkBackground(const cv::Mat &img, const Contour &contour)
{
    return backgroundAnalyzer.isDarkBackground(img, contour);
}

void RedAnalyzer::setSettings(const RedAnalyzerSettings settings)
{
    this->settings = settings;
}

Contours RedAnalyzer::findContours(const cv::Mat &img)
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


