#include "CirclePort.hpp"

CirclePort::CirclePort(QPointF position, float radius) : radius(radius)
{
    pos = position;
}

void CirclePort::paint(QPainter *painter) const
{
    painter->drawEllipse(pos, radius, radius);
}

bool CirclePort::isInside(QPointF point) const
{
    float x = point.x() - pos.x();
    float y = point.y() - pos.y();
    float distance = std::sqrt(x * x + y * y);
    return distance < radius;
}
