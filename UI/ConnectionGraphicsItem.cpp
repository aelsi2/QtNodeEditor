#include "ConnectionGraphicsItem.hpp"

ConnectionGraphicsItem::ConnectionGraphicsItem(
    QUuid uuid,
    NodeGraphicsItem* nodeItemA,
    NodeGraphicsItem* nodeItemB,
    PortID portIdA,
    PortID portIdB) :
    uuid(uuid),
    nodeItemA(nodeItemA),
    nodeItemB(nodeItemB),
    portIdA(portIdA),
    portIdB(portIdB)
{
    lineWidth = 3;
    setZValue(-100);
    snapToPorts();
    QObject::connect(nodeItemA, &NodeGraphicsItem::nodeMoved, this, &ConnectionGraphicsItem::snapToPorts);
    QObject::connect(nodeItemB, &NodeGraphicsItem::nodeMoved, this, &ConnectionGraphicsItem::snapToPorts);
}

bool ConnectionGraphicsItem::isConnectedTo(NodeGraphicsItem *nodeItem, PortID portId) const
{
    if (nodeItem == nodeItemA)
    {
        return portId == portIdA;
    }
    if (nodeItem == nodeItemB)
    {
        return portId == portIdB;
    }
    return false;
}

QUuid ConnectionGraphicsItem::getUuid() const
{
    return uuid;
}

void ConnectionGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    QPen pen = painter->pen();
    pen.setWidth(lineWidth);
    //painter->drawRect(boundingRect());
    //painter->drawPath(shape());
    painter->setPen(pen);
    painter->drawLine(startPointLocal(), endPointLocal());
    
}

QRectF ConnectionGraphicsItem::boundingRect() const
{
    QPointF start = startPointLocal();
    QPointF end = endPointLocal();
    QPointF topLeft = QPointF(std::min(start.x(), end.x()), std::min(start.y(), end.y())) + QPointF(-lineWidth, -lineWidth);
    QPointF bottomRight = QPointF(std::max(start.x(), end.x()), std::max(start.y(), end.y())) + QPointF(lineWidth, lineWidth);
    return QRectF(topLeft, bottomRight);
}

QPainterPath ConnectionGraphicsItem::shape() const
{
    QPointF startPoint = startPointLocal();
    QPointF endPoint = endPointLocal();
    float halfWidth = lineWidth / 2;
    
    QPointF lineVector = endPoint - startPoint;
    float lineVectorDotX = lineVector.x();
    float lineVectorLength = std::sqrt(lineVector.x() * lineVector.x() + lineVector.y() * lineVector.y());
    float lineAngleCos = lineVectorDotX / lineVectorLength;
    float lineAngleSin = (lineVector.y() < 0 ? 1 : -1) * std::sqrt(1 - lineAngleCos * lineAngleCos);

    QPainterPath path;
    path.moveTo(startPoint + QPointF(-lineAngleSin * halfWidth, -lineAngleCos * halfWidth));
    path.lineTo(endPoint + QPointF(-lineAngleSin * halfWidth, -lineAngleCos * halfWidth));
    path.lineTo(endPoint + QPointF(lineAngleSin * halfWidth, lineAngleCos * halfWidth));
    path.lineTo(startPoint + QPointF(lineAngleSin * halfWidth, lineAngleCos * halfWidth));
    return path;
}

void ConnectionGraphicsItem::snapToPorts()
{
    prepareGeometryChange();
    startPointScene = nodeItemA->getPortPositionScene(portIdA);
    endPointScene = nodeItemB->getPortPositionScene(portIdB);
}

QPointF ConnectionGraphicsItem::startPointLocal() const
{
    return mapFromScene(startPointScene);
}

QPointF ConnectionGraphicsItem::endPointLocal() const
{
    return mapFromScene(endPointScene);
}
