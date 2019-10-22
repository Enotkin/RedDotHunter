#include "redanalyzer.h"

RedAnalyzer::RedAnalyzer(QObject *parent) : QObject(parent)
{
    auto settings = staticPointsFilter.settings();
    settings.setMinPointTraceSize(50);
    staticPointsFilter.setSettings(settings);
}

Contours RedAnalyzer::getRedDotsCoordinate(const QFileInfoList &imagesInfo)
{
    PointsPacks framePoints;
    for(const auto &imgInfo : imagesInfo){
        auto matImg = cv::imread(imgInfo.absoluteFilePath().toStdString());
        auto points = getPoints(matImg);
        framePoints.push_back(points);
        qDebug()<<imgInfo.fileName();
    }
    PointsPacks resultFramePoints = timeFiltrate(framePoints);
    return resultFramePoints;
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

PointsPacks RedAnalyzer::timeFiltrate(const PointsPacks &points)
{
    az::FramePoints azFramePoints = cvPointsPacks2QPointsPack(points);
    staticPointsFilter.run(azFramePoints);
    auto filtredPoints = staticPointsFilter.filteredPoints();
    std::vector<std::vector<cv::Point>> resultFramePoints = qPointsPack2cvPointsPack(filtredPoints);
    return resultFramePoints;
}

std::vector<QPoint> RedAnalyzer::cvPoints2QPoints(const std::vector<cv::Point> &cvPoints)
{
    std::vector<QPoint> qPoints;
    std::transform(cvPoints.begin(), cvPoints.end(), std::back_inserter(qPoints), [](const cv::Point &point){
        return QPoint(point.x, point.y);
    });
    return qPoints;
}

std::vector<cv::Point> RedAnalyzer::qPoints2cvPoints(const std::vector<QPoint> &qPoints)
{
    std::vector<cv::Point> cvPoints;
    std::transform(qPoints.begin(), qPoints.end(), std::back_inserter(cvPoints), [](const QPoint &point){
        return cv::Point(point.x(), point.y());
    });
    return cvPoints;
}

std::vector<std::vector<QPoint> > RedAnalyzer::cvPointsPacks2QPointsPack(const std::vector<std::vector<cv::Point> > &cvPointsPack)
{
    std::vector<std::vector<QPoint>> qPointsPack;
    std::transform(cvPointsPack.begin(), cvPointsPack.end(), std::back_inserter(qPointsPack), [this](const auto &pointPack){
        return cvPoints2QPoints(pointPack);
    });
    return qPointsPack;
}

std::vector<std::vector<cv::Point> > RedAnalyzer::qPointsPack2cvPointsPack(const std::vector<std::vector<QPoint> > &qPointsPack)
{
   std::vector<std::vector<cv::Point>> cvPointsPack;
   std::transform(qPointsPack.begin(), qPointsPack.end(), std::back_inserter(cvPointsPack), [this](const auto &pointPack){
       return qPoints2cvPoints(pointPack);
   });
   return cvPointsPack;
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
        HsvMargins margins(60, 80, 80);
        colorBinarizatorSettings.hsvMargin = margins;
        colorBinarizatorSettings.color = QColor::fromHsv(0, 100, 100);
    }
    colorBinarizator.setSettings(colorBinarizatorSettings);
    return colorBinarizator.binarizeImage(src);
}

cv::Mat RedAnalyzer::normalBinarize(const cv::Mat &src)
{
    NormalBinarizator normalBinarizator(NormalBinarizatorSettings{100});
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


