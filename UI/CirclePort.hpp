#pragma once

#include "PortGraphics.hpp"

class CirclePort : public PortGraphics
{
public:
    CirclePort(QPointF pos, float radius);
    virtual void paint(QPainter *painter) const override;
    virtual bool isInside(QPointF point) const override;
protected:
    float radius;
};

