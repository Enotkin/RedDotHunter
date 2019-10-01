#include "trackdetector.h"
#include <QDebug>

TrackDetector::TrackDetector() {}

TrackDetector::TrackDetector(const TrackDetectorSettings &settings) : suspetctSettings(settings) {}

TrackDetector::~TrackDetector()
{
    if (!branches.empty())
        branches.clear();
}

void TrackDetector::findTrack(std::list<Contour> &contours)
{
    contours.sort([](const Contour &a, const Contour &b){ return a.getArea() > b.getArea();});

    //Если нет веток, то все контура это начало новых веток
    if (branches.empty()){
        for(const auto &contour : contours)
            branches.emplace_back(contour, suspetctSettings);
        return;
    }

    //Создание селекторов
    auto selectors = createSelectors(contours);

    //Распределение веток для селекторов
    branchDistribution(selectors, branches);

    //Выбор веток селекторами
    std::for_each(selectors.begin(), selectors.end(), [](auto &selector){selector.selectionBranch();});

    //Конец раунда, уменьшение времени жизни обездоленых
    std::for_each(branches.begin(), branches.end(), [](auto &branche){branche.endRound();});

    //Копирование подтверждённых зарядов
    copyConfirmedCharges(branches);

    //Удаление шумовых разрядов из общей кучи веток
    branches.remove_if([](const auto &suspectCrownCharge){
        return suspectCrownCharge.isNoise();});

    branches.remove_if([](const auto &suspectCrownCharge){
        return suspectCrownCharge.isConfirmedCharge();});

    //Дополнение веток, селекторами без пар
    for (auto &selector : selectors){
        if (!selector.isSelectingEnd())
            branches.emplace_back(selector.getContour(), suspetctSettings);
    }

    //Сортировка веток
    branches.sort([](const Branch &a, const Branch &b){ return a.length() > b.length();});
}

std::list<BranchSelector> TrackDetector::createSelectors(const std::list<Contour> &contours)
{
    std::list<BranchSelector> selectors;
    for (auto &contour : contours) {
        selectors.emplace_back(contour);
    }
    return selectors;
}

void TrackDetector::branchDistribution(std::list<BranchSelector> &selectorList, std::list<Branch> &branchList)
{
    for (auto &selector : selectorList)
        for (auto &branche : branchList)
            if(branche.checkCompatibility(selector.getContour()))
                selector.addBranche(branche);
}

void TrackDetector::copyConfirmedCharges(const std::list<Branch> &branches)
{
    lastDetectedCharges.clear();
    for (const auto &branche : branches){
        if (branche.isConfirmedCharge())
            lastDetectedCharges.push_back(branche.getCrownCharge());
    }
    std::copy(lastDetectedCharges.begin(), lastDetectedCharges.end(), std::back_inserter(detectedCharges));
}

std::list<ObjectTrack> TrackDetector::getTracks() const
{
    return detectedCharges;
}

void TrackDetector::setSettings(TrackDetectorSettings settings)
{
    suspetctSettings = settings;
}

bool TrackDetector::isConfirmed(const Branch &branch)
{
    return branch.isConfirmedCharge();
}
