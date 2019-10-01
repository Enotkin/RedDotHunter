#ifndef COLORSELECTORHSV_H
#define COLORSELECTORHSV_H

#include <QWidget>
#include "colorselector.h"

class ColorSelectorHsv : public ColorSelector
{
public:
    ColorSelectorHsv();

private:
    QColor assemblyColor(int first, int second, int third) override final;
    std::tuple<int, int, int> disassemblyColor(const QColor &color) override final;
};

#endif // COLORSELECTORHSV_H
