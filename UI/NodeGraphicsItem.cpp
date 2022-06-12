#include "NodeGraphicsItem.hpp"

NodeGraphicsItem::NodeGraphicsItem(Node *node, QUuid uuid)
    : QGraphicsObject(nullptr), node(node), uuid(uuid)
{
    qDebug() << uuid.toString();
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    size = QPointF(128, 64);
    ports.insert(PortID(PortDirection::IN, 0), new CirclePort(QPointF(64, 10), 5));
    ports.insert(PortID(PortDirection::OUT, 0), new CirclePort(QPointF(64, 54), 5));
}

QUuid NodeGraphicsItem::getUuid() const
{
    return uuid;
}

void NodeGraphicsItem::setNodeSelected(bool selected)
{
    nodeIsSelected = selected;
}
bool NodeGraphicsItem::getNodeSelected() const
{
    return nodeIsSelected;
}

QPointF NodeGraphicsItem::getPortPositionLocal(PortID portId) const
{
    QPointF pos = QPointF(0, 0);
    if (ports.contains(portId))
    {
        pos = ports.value(portId)->getPos();
    }
    return pos;
}

QPointF NodeGraphicsItem::getPortPositionScene(PortID portId) const
{
    QPointF localPos = getPortPositionLocal(portId);
    return mapToScene(localPos);
}

PortID NodeGraphicsItem::getNearestPortLocal(QPointF point) const
{
    PortID portId;
    float minDistance;
    bool found = false;
    for (auto i = ports.cbegin(); i != ports.cend(); ++i)
    {
        QPointF delta = i.value()->getPos() - point;
        float distance = std::sqrt(delta.x() * delta.x() + delta.y() + delta.y());
        if (!found || distance < minDistance) {
            found = true;
            minDistance = distance;
            portId = i.key();
        }
    }
    return portId;
}

PortID NodeGraphicsItem::getNearestPortScene(QPointF point) const
{
    QPointF localPos = mapFromScene(point);
    return getNearestPortLocal(localPos);
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
    event->accept();
    ungrabMouse();
    
    GraphView *graphView = getGraphView(event);
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
        if (event->modifiers() & Qt::CTRL)
            graphView->addToSelection(uuid);
        else graphView->select(uuid);
        graphView->beginMove(event->scenePos());
    }
    else {
        graphView->beginConnection(this, port);
    }
    
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemScenePositionHasChanged)
        emit nodeMoved(value.toPointF());
}

GraphView * NodeGraphicsItem::getGraphView(QGraphicsSceneMouseEvent *event)
{
    return dynamic_cast<GraphView*>(event->widget()->parentWidget());
}
