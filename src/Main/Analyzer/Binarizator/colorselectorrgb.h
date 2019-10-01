#ifndef COLORSELECTORRGB_H
#define COLORSELECTORRGB_H

#include <QColor>
#include "colorselector.h"

class ColorSelectorRgb : public ColorSelector
{
public:
    ColorSelectorRgb();

private:
    QColor assemblyColor(int first, int second, int third) override final;
    std::tuple<int, int, int> disassemblyColor(const QColor &color) override final;
};

#endif // COLORSELECTORRGB_H
