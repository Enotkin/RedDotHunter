#include "colorselectorhsv.h"

ColorSelectorHsv::ColorSelectorHsv()
{
    setRanges(QVector<int>{359, 255, 255});
    setLabelText(QVector<QString>{"Цвет", "Насыщенность", "Яркость"});
}

QColor ColorSelectorHsv::assemblyColor(int first, int second, int third)
{
    return QColor::fromHsv(first, second, third);
}

std::tuple<int, int, int> ColorSelectorHsv::disassemblyColor(const QColor &color)
{
    return std::make_tuple(color.hue(), color.saturation(), color.value());
}
