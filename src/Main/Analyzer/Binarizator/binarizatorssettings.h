#ifndef BINARIZATORSETTINGS_H
#define BINARIZATORSETTINGS_H

#include <QPair>
#include <QColor>

struct HsvMargins {
    QPair<int, int> hue {20, 20};
    QPair<int, int> saturation {30,30};
    QPair<int, int> value {30, 30};

    HsvMargins() = default;
    HsvMargins(int hue, int saturation, int value) :
        hue({hue, hue}), saturation({saturation, saturation}), value({value, value}){}

    void setHue(int value){ hue = {value, value};}
    void setHue(int leftValue, int rightValue){hue = {leftValue, rightValue};}
    void setSaturation(int value){ hue = {value, value};}
    void setSaturation(int leftValue, int rightValue){saturation = {leftValue, rightValue};}
    void setValue(int value){ hue = {value, value};}
    void setValue(int leftValue, int rightValue){value = {leftValue, rightValue};}
};



struct ColorBinarizatorSettings {
    QColor color{Qt::white};
    HsvMargins hsvMargin;
};

struct NormalBinarizatorSettings {
    int tresholdValue = 128;
};

struct BinarizatorsSettings
{
    ColorBinarizatorSettings colorSettings;
    NormalBinarizatorSettings normalSettings;
};

#endif // BINARIZATORSETTINGS_H
