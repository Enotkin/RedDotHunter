#ifndef COLORSELECTOR_H
#define COLORSELECTOR_H

#include <QWidget>
#include <QColor>
#include <QDebug>

namespace Ui {
class ColorSelector;
}

class ColorSelector : public QWidget
{
    Q_OBJECT

public:
    explicit ColorSelector(QWidget *parent = nullptr);
    ~ColorSelector();

    void setColor(QColor color);

protected:
    QColor assemblyColor(int arg1, int arg2, int arg3);
    std::tuple<int, int, int> disassemblyColor(const QColor &color);
    void setLabelText(QVector<QString> texts);
    void setRanges(QVector<int> ranges);

signals:
    void colorChanged(QColor color);

private slots:
    void on_horizontalSliderFirst_valueChanged(int value);

    void on_horizontalSliderSecond_valueChanged(int value);

    void on_horizontalSliderThird_valueChanged(int value);

private:
    Ui::ColorSelector *ui;
    int first;
    int second;
    int third;

    void emitColor();
};

#endif // COLORSELECTOR_H
