#pragma once

#include <QPainter>
#include <QPointF>

class PortGraphics
{
public:
    PortGraphics();
    virtual void paint(QPainter *painter, QPointF position) const;
    virtual bool inside(QPointF point) const;
};

