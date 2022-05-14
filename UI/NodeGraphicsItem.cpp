#include "NodeGraphicsItem.hpp"

NodeGraphicsItem::NodeGraphicsItem(NodeGraph *graph, Node *node, QUuid uuid)
    : graph(graph), node(node), uuid(uuid)
{
    
}

void NodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    
}

QRectF NodeGraphicsItem::boundingRect() const
{
    
}

QPainterPath NodeGraphicsItem::shape() const
{
    
}
