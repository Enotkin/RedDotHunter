#include "analyzersettingsform.h"
#include "ui_analyzersettingsform.h"

AnalyzerSettingsForm::AnalyzerSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnalyzerSettingsForm)
{
    ui->setupUi(this);
    RedAnalyzerSettings defaultSetting;
//    ui->areaSizeSpinBox->setValue(defaultSetting.trackDetectorSettings.size);
//    ui->distantSpinBox->setValue(defaultSetting.trackDetectorSettings.delta);
//    ui->lifeTimeSpinBox->setValue(defaultSetting.trackDetectorSettings.lifeTime);

//    ui->tresholdSpinBox->setValue(defaultSetting.binarizatorSettings.thresholdValue);
}

AnalyzerSettingsForm::~AnalyzerSettingsForm()
{
    delete ui;
}

RedAnalyzerSettings AnalyzerSettingsForm::getSettings()
{
//    TrackDetectorSettings trackSettings;
//    trackSettings.size = ui->areaSizeSpinBox->value();
//    trackSettings.delta = ui->distantSpinBox->value();
//    trackSettings.lifeTime = ui->lifeTimeSpinBox->value();

    BinarizatorsSettings binarizatorSettings;

    RedAnalyzerSettings resultSetting;


    return resultSetting;
}
