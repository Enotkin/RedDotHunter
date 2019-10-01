#ifndef CONTOURSEACHER_H
#define CONTOURSEACHER_H

#include <QObject>
#include "Analyzer/contour.h"

class ContourSeacher : public QObject
{
    Q_OBJECT
public:
    explicit ContourSeacher(QObject *parent = nullptr);


signals:

public slots:
};

#endif // CONTOURSEACHER_H
