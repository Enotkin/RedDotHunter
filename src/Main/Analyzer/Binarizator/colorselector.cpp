#include "colorselector.h"
#include "ui_colorselector.h"

ColorSelector::ColorSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorSelector)
{
    ui->setupUi(this);
}

ColorSelector::~ColorSelector()
{
    delete ui;
}

void ColorSelector::setColor(QColor color)
{
    auto [firstValue, secondValue, thirdValue] = disassemblyColor(color);

    first = firstValue;
    second = secondValue;
    third = thirdValue;

    ui->horizontalSliderFirst->setValue(firstValue);
    ui->horizontalSliderSecond->setValue(secondValue);
            ui->horizontalSliderThird->setValue(thirdValue);
}

void ColorSelector::setLabelText(QVector<QString> texts)
{
    ui->labelFirst->setText(texts.at(0));
    ui->labelSecond->setText(texts.at(1));
    ui->labelThird->setText(texts.at(2));
}

void ColorSelector::setRanges(QVector<int> ranges)
{
    ui->horizontalSliderFirst->setRange(0, ranges.at(0));
    ui->spinBoxFirst->setRange(0, ranges.at(0));

    ui->horizontalSliderSecond->setRange(0, ranges.at(1));
    ui->spinBoxSecond->setRange(0, ranges.at(1));

    ui->spinBoxThird->setRange(0, ranges.at(2));
    ui->spinBoxThird->setRange(0, ranges.at(2));
}

void ColorSelector::on_horizontalSliderFirst_valueChanged(int value)
{
    first = value;
    emitColor();
}

void ColorSelector::on_horizontalSliderSecond_valueChanged(int value)
{
    second = value;
    emitColor();
}

void ColorSelector::on_horizontalSliderThird_valueChanged(int value)
{
    third = value;
    emitColor();
}

void ColorSelector::emitColor()
{
    auto color = assemblyColor(first, second, third);
    emit colorChanged(color);
}
