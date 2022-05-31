#pragma once

#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QUuid>
#include <QHash>
#include <QGraphicsSceneMouseEvent>

#include "PortGraphics.hpp"
#include "CirclePort.hpp"
#include "Controller/GraphEditor.hpp"
#include "Core/Node.hpp"


class NodeGraphicsItem : public QGraphicsItem
{
public:
    NodeGraphicsItem(GraphEditor * editor, Node * node, QUuid uuid);
    
    QPointF getPortPosition(PortID portId);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    
protected:
    
    QRectF getRect() const;
    
    QPointF size;
    GraphEditor * editor;
    Node * node;
    QUuid uuid;
    QHash<PortID, PortGraphics*> ports;
};

