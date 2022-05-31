#pragma once

#include <QPainter>
#include <QPointF>
#include <cmath>

class PortGraphics
{
public:
    virtual void paint(QPainter *painter) const = 0;
    virtual bool isInside(QPointF point) const = 0;
protected:
    QPointF pos;
};

