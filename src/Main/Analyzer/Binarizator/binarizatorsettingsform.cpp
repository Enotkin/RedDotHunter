#include "binarizatorsettingsform.h"
#include "ui_binarizatorsettingsform.h"

BinarizatorSettingsForm::BinarizatorSettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BinarizatorSettingsForm)
{
    ui->setupUi(this);
}

BinarizatorSettingsForm::~BinarizatorSettingsForm()
{
    delete ui;
}

void BinarizatorSettingsForm::setColor(const QColor color)
{
    currentColor = color.toRgb();
    displayColor(color);
    currentColorSelector->setColor(color);
}

BinarizatorSettings BinarizatorSettingsForm::getSettings()
{
    BinarizatorSettings result;
//    result.mode = ;
    result.color = currentColor;
//    result.coefficientHsv = currentColorSelector->
    return result;
}

void BinarizatorSettingsForm::displayColor(const QColor color)
{
    QPixmap colorImage(ui->labelViewColor->rect().width(), ui->labelViewColor->rect().height());
    colorImage.fill(color);
    ui->labelViewColor->setPixmap(colorImage);

    auto colorName = QString("%1%2%3").arg(color.red(), 0, 16).arg(color.green(), 0, 16).arg(color.blue(), 0, 16);
    ui->labelNameColor->setText(colorName);

    if(currentColorSelector)
        currentColorSelector->setColor(color);
}

void BinarizatorSettingsForm::setFilterMode(const BinarizatorSettings::FilterType type)
{
    switch (type) {
    case BinarizatorSettings::FilterType::Normal: activateNormalMode(); break;
    case BinarizatorSettings::FilterType::Colorfull: activateColorfullMode(); break;
    }
}

void BinarizatorSettingsForm::on_comboBoxTypeFilter_activated(int index)
{
    setFilterMode(static_cast<BinarizatorSettings::FilterType>(index));
}

void BinarizatorSettingsForm::on_comboBoxColorFormat_activated(int index)
{
    if (currentColorSelector){
        currentColorSelector->disconnect();
        ui->layoutColorSelector->removeWidget(currentColorSelector);
        delete currentColorSelector;
    }
    switch (static_cast<ColorMode>(index)) {
    case ColorMode::RGB:
        currentColorSelector = new ColorSelectorRgb();
        break;
    case ColorMode::HSV:
        currentColorSelector = new ColorSelectorHsv();
        break;
    }
    currentColorSelector->setColor(currentColor);
    connect(currentColorSelector, &ColorSelector::colorChanged, this, &BinarizatorSettingsForm::displayColor);
    ui->layoutColorSelector->addWidget(currentColorSelector);
}

void BinarizatorSettingsForm::activateNormalMode()
{
    ui->comboBoxTypeFilter->setCurrentIndex(static_cast<int>(BinarizatorSettings::FilterType::Normal));
    ui->widgetColorSelect->hide();
    displayColor(QColor(Qt::white).toRgb());
}

void BinarizatorSettingsForm::activateColorfullMode()
{
    ui->comboBoxTypeFilter->setCurrentIndex(static_cast<int>(BinarizatorSettings::FilterType::Colorfull));
    ui->widgetColorSelect->show();
    displayColor(currentColor.toRgb());


    if (currentColorSelector){
        currentColorSelector->disconnect();
        ui->layoutColorSelector->removeWidget(currentColorSelector);
        delete currentColorSelector;
    }
    currentColorSelector = new ColorSelectorRgb();
    currentColorSelector->setColor(currentColor);
    connect(currentColorSelector, &ColorSelector::colorChanged, this, &BinarizatorSettingsForm::displayColor);
    ui->layoutColorSelector->addWidget(currentColorSelector);
}
