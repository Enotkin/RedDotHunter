#ifndef NORMALBINARIZATOR_H
#define NORMALBINARIZATOR_H

#include "binarizator.h"
#include "binarizatorssettings.h"

class NormalBinarizator : public Binarizator
{
public:
    explicit NormalBinarizator();
    NormalBinarizator (const NormalBinarizatorSettings &settings);

    virtual cv::Mat binarizeImage(const cv::Mat &src) const override final;
    void setSettings(const NormalBinarizatorSettings &settings);

private:
    int tresholdValue = 128;
};

#endif // NORMALBINARIZATOR_H
