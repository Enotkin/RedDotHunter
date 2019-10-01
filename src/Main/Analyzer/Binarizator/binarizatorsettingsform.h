#ifndef BINARIZATORSETTINGSFORM_H
#define BINARIZATORSETTINGSFORM_H

#include <QWidget>
#include <QColor>

#include "colorselectorhsv.h"
#include "colorselectorrgb.h"
#include "binarizatorsettings.h"

namespace Ui {
class BinarizatorSettingsForm;
}

class BinarizatorSettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit BinarizatorSettingsForm(QWidget *parent = nullptr);
    ~BinarizatorSettingsForm();

    void setColor(const QColor color);

    BinarizatorSettings getSettings();

private slots:
    void on_comboBoxTypeFilter_activated(int index);

    void on_comboBoxColorFormat_activated(int index);

private:
    enum class ColorMode{
        RGB,
        HSV
    };

    Ui::BinarizatorSettingsForm *ui;

    QColor currentColor;
    ColorMode colorMode = ColorMode::RGB;
    ColorSelector *currentColorSelector = nullptr;

    void displayColor(const QColor color);
    void setFilterMode(const BinarizatorSettings::FilterType type);

    void activateNormalMode();
    void activateColorfullMode();

    void activate();
    void deactivate();


};

#endif // BINARIZATORSETTINGSFORM_H
