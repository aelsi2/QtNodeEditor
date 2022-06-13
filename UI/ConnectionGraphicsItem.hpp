#pragma once

#include <QGraphicsItem>
#include <QPointF>
#include <QUuid>
#include "NodeGraphicsItem.hpp"

class ConnectionGraphicsItem : public QObject, public QGraphicsItem
{
public:
    ConnectionGraphicsItem(
        QUuid uuid,
        NodeGraphicsItem* nodeItemA,
        NodeGraphicsItem* nodeItemB,
        PortID portIdA,
        PortID portIdB);
    
    void onPreDelete();
    
    bool isConnectedTo(NodeGraphicsItem *nodeItem, PortID portId) const;
    QUuid getUuid() const;
    
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    
public slots:
    void snapToPorts();
    
private:
    
    QPointF startPointLocal() const;
    QPointF endPointLocal() const;
    
    QUuid uuid;
    NodeGraphicsItem *nodeItemA;
    NodeGraphicsItem *nodeItemB;
    PortID portIdA;
    PortID portIdB;
    
    int lineWidth;
    QPointF startPointScene;
    QPointF endPointScene;
};

