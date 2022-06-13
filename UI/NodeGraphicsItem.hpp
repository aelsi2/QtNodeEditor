#pragma once

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPainterPath>
#include <QUuid>
#include <QHash>
#include <QGraphicsSceneMouseEvent>

class GraphView;

#include "PortGraphics.hpp"
#include "CirclePort.hpp"
#include "Core/Node.hpp"
#include "UI/GraphView.hpp"

class NodeGraphicsItem : public QGraphicsObject
{
    Q_OBJECT
public:
    NodeGraphicsItem(Node *node, QUuid uuid);
    
    void onPreDelete();
    
    void setNodeSelected(bool selected);
    bool getNodeSelected() const;
    
    QPointF getPortPositionLocal(PortID portId) const;
    QPointF getPortPositionScene(PortID portId) const;
    PortID getNearestPortLocal(QPointF point) const;
    PortID getNearestPortScene(QPointF point) const;
    
    QUuid getUuid() const;
    
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    
signals:
    void nodeMoved(QPointF newPosition);
    
protected:
    
    QRectF getNodeRect() const;
    
    static GraphView* getGraphView(QGraphicsSceneMouseEvent *event);
    
    bool nodeIsSelected = false;
    
    QPointF size;
    Node * node;
    QUuid uuid;
    QHash<PortID, PortGraphics*> ports;
};

