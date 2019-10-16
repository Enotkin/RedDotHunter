#ifndef BINARIZATORSETTINGSFORM_H
#define BINARIZATORSETTINGSFORM_H

#include <QWidget>
#include <QColor>

#include "colorselector.h"
#include "binarizatorssettings.h"

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

    BinarizatorsSettings getSettings();

public slots:
    void displayColor(const QColor color);

private:
    Ui::BinarizatorSettingsForm *ui;

    QColor currentColor;

    HsvMargins collectMargins();

};

#endif // BINARIZATORSETTINGSFORM_H
