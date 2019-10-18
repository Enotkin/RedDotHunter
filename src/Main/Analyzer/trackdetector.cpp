#include "trackdetector.h"
#include <QDebug>

TrackDetector::TrackDetector() {}

TrackDetector::TrackDetector(const TrackDetectorSettings &settings) : suspetctSettings(settings) {}

TrackDetector::~TrackDetector()
{
    if (!branches.empty())
        branches.clear();
}

std::vector<std::vector<cv::Point> >  TrackDetector::separatePoints(const std::vector<std::vector<cv::Point> > &points)
{
    for (const auto &pointsAtImg : points){
        //Если нет веток, то все точки это начало новых веток
        if (branches.empty()){
            for(const auto &point : pointsAtImg)
                branches.emplace_back(point, suspetctSettings);
            continue;
        }

        //Создание селекторов
        auto selectors = createSelectors(pointsAtImg);

        //Распределение веток для селекторов
        branchDistribution(selectors, branches);

        //Выбор веток селекторами
        std::for_each(selectors.begin(), selectors.end(), [](auto &selector){selector.selectionBranch();});

        //Конец раунда, уменьшение времени жизни обездоленых
        std::for_each(branches.begin(), branches.end(), [](auto &branche){branche.endRound();});

        //очистка всякого
        //Удаление шумовых разрядов из общей кучи веток
        std::remove_if(std::begin(branches), std::end(branches), [](const auto &branche){
            return branche.isNoise();});


//        //Дополнение веток, селекторам без пар
//        for (auto &selector : selectors){
//            if (!selector.isSelectingEnd()){
//                Branch branche(selector.getPoint(), suspetctSettings);
//                branches.push_back(branche);
//            }
////                branches.emplace_back(selector.getPoint(), suspetctSettings);
//        }

        //Сортировка веток
        std::sort(std::begin(branches), std::end(branches), [](const Branch &a, const Branch &b){ return a.length() > b.length();});
    }


    std::vector<std::vector<cv::Point>> result;
    std::transform(std::begin(branches), std::end(branches), std::back_inserter(result), [](const auto &branche){
        return branche.getPoints();
    });
    return result;

}

std::vector<BranchSelector> TrackDetector::createSelectors(const std::vector<cv::Point> &points)
{
    std::vector<BranchSelector> selectors;
    for (const auto &point : points) {
        selectors.emplace_back(point);
    }
    return selectors;
}

void TrackDetector::branchDistribution(std::vector<BranchSelector> &selectorList, std::vector<Branch> &branchList)
{
    for (auto &selector : selectorList)
        for (auto &branche : branchList)
            if(branche.checkCompatibility(selector.getPoint()))
                selector.addBrancheRef(branche);
}

void TrackDetector::setSettings(TrackDetectorSettings settings)
{
    suspetctSettings = settings;
}
