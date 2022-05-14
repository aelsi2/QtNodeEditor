#pragma once

#include <QGraphicsItem>
#include <QPointF>
#include <QUuid>

class ConnectionGraphicsItem : public QGraphicsItem
{
public:
    ConnectionGraphicsItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void updatePoints();
private:
    QUuid uuid;
    QPointF startPoint;
    QPointF endPoint;
};

