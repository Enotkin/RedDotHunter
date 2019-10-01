#ifndef ANALYZERSETTINGSFORM_H
#define ANALYZERSETTINGSFORM_H

#include <QWidget>
#include "Analyzer/trackdetectorsettings.h"
#include "Analyzer/redanalyzer.h"

namespace Ui {
class AnalyzerSettingsForm;
}

class AnalyzerSettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyzerSettingsForm(QWidget *parent = nullptr);
    ~AnalyzerSettingsForm();

    RedAnalyzerSettings getSettings();

private:
    Ui::AnalyzerSettingsForm *ui;
};

#endif // ANALYZERSETTINGSFORM_H
