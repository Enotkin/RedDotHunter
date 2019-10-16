#ifndef COLORBINARIZATOR_H
#define COLORBINARIZATOR_H

#include "binarizator.h"
#include "binarizatorssettings.h"

class ColorBinarizator : public Binarizator
{
public:
    explicit ColorBinarizator();
    ColorBinarizator(const ColorBinarizatorSettings &settings);

    virtual cv::Mat binarizeImage(const cv::Mat &src) const override final;
    void setSettings(const ColorBinarizatorSettings &settings);

private:
    QColor color;
    HsvMargins margins;

    std::pair<cv::Scalar, cv::Scalar> getHsvRanges() const;
    std::pair<int, int> getRange(int channel, const QPair<int, int> &range, int maxValue = 255) const;
};

#endif // COLORBINARIZATOR_H
