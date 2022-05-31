#include "NodeGraphicsItem.hpp"

NodeGraphicsItem::NodeGraphicsItem(GraphEditor * editor, Node * node, QUuid uuid)
    : editor(editor), node(node), uuid(uuid)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    size = QPointF(128, 64);
    ports.insert(PortID(PortDirection::OUT, 0), new CirclePort(QPointF(64, 54), 5));
}

void NodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(getRect(), 5, 5);
    for (auto i = ports.cbegin(); i != ports.cend(); ++i)
    {
        i.value()->paint(painter);
    }
}

QRectF NodeGraphicsItem::boundingRect() const
{
    return getRect();
}

QPainterPath NodeGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(getRect());
    return path;
}

QRectF NodeGraphicsItem::getRect() const
{
    return QRectF(QPointF(0, 0), size);
}

void NodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->pos();
    PortID port;
    bool isPort = false;
    for (auto i = ports.cbegin(); i!= ports.cend(); ++i)
    {
        if (i.value()->isInside(pos)) {
            port = i.key();
            isPort = true;
            break;
        }
    }
    if (!isPort){
        QGraphicsItem::mousePressEvent(event);
        return;
    }
    ungrabMouse();
}
