#include "colorselectorrgb.h"

ColorSelectorRgb::ColorSelectorRgb()
{
    setRanges(QVector<int>{255, 255, 255});
    setLabelText(QVector<QString>{"Красный", "Зелёный", "Синий"});
}

QColor ColorSelectorRgb::assemblyColor(int first, int second, int third)
{
    return QColor::fromRgb(first, second, third);
}

std::tuple<int, int, int> ColorSelectorRgb::disassemblyColor(const QColor &color)
{
    return std::make_tuple(color.red(), color.green(), color.blue());
}
