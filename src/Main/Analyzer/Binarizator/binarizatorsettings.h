#ifndef BINARIZATORSETTINGS_H
#define BINARIZATORSETTINGS_H

#include <QPair>
#include <QColor>

struct BinarizatorSettings
{
    enum class FilterType
    {
        Normal,
        Colorfull
    };

    struct CoefficientHsv {
        QPair<int, int> hue {20, 20};
        QPair<int, int> saturation {30,30};
        QPair<int, int> value {30, 30};
    };

    QColor color {Qt::white};
    FilterType mode = FilterType::Normal;
    double thresholdValue = 225;
    CoefficientHsv coefficientHsv;
};

#endif // BINARIZATORSETTINGS_H
