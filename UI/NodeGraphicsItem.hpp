#pragma once

#include <QGraphicsItem>
#include <QUuid>

#include "Core/NodeGraph.hpp"
#include "Core/Node.hpp"


class NodeGraphicsItem : public QGraphicsItem
{
public:
    NodeGraphicsItem(NodeGraph *graph, Node *node, QUuid uuid);
    
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    
protected:
    NodeGraph *graph;
    Node *node;
    QUuid uuid;
};

