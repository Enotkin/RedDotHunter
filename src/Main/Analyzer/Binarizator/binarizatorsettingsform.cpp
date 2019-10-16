#include "binarizatorsettingsform.h"
#include "ui_binarizatorsettingsform.h"

BinarizatorSettingsForm::BinarizatorSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinarizatorSettingsForm)
{
    ui->setupUi(this);
    connect(ui->colorSelector, &ColorSelector::colorChanged, this, &BinarizatorSettingsForm::displayColor);
}

BinarizatorSettingsForm::~BinarizatorSettingsForm()
{
    delete ui;
}

void BinarizatorSettingsForm::setColor(const QColor color)
{
    currentColor = color.toRgb();
    displayColor(color); 
    ui->colorSelector->setColor(color);
}

void BinarizatorSettingsForm::displayColor(const QColor color)
{
    QPixmap colorImage(ui->labelViewColor->rect().width(), ui->labelViewColor->rect().height());
    colorImage.fill(color);
    ui->labelViewColor->setPixmap(colorImage);

    auto colorName = QString("%1%2%3").arg(color.red(), 0, 16).arg(color.green(), 0, 16).arg(color.blue(), 0, 16);
    ui->labelNameColor->setText(colorName);
}

HsvMargins BinarizatorSettingsForm::collectMargins()
{
    HsvMargins margins;
    margins.setHue(ui->hueLeftMarginSpinBox->value(), ui->hueRightMarginSpinBox->value());
    margins.setSaturation(ui->saturationLeftMarginSpinBox->value(), ui->saturationRightMarginSpinBox->value());
    margins.setValue(ui->valueLeftMarginSpinBox->value(), ui->valueRightMarginSpinBox->value());
    return margins;
}

BinarizatorsSettings BinarizatorSettingsForm::getSettings()
{
    NormalBinarizatorSettings normalSettings;
    normalSettings.tresholdValue = ui->tresholdSpinBox->value();

    ColorBinarizatorSettings colorSettings;
    colorSettings.color = currentColor;
    colorSettings.hsvMargin = collectMargins();

    return {colorSettings, normalSettings};
}

