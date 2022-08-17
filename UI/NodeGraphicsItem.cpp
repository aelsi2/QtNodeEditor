#include "NodeGraphicsItem.hpp"

NodeGraphicsItem::NodeGraphicsItem(Node *node, QUuid uuid)
    : QGraphicsObject(nullptr), node(node), uuid(uuid)
{
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
    update();
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
        float distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
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
    QPen pen = painter->pen();
    if (nodeIsSelected) pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    
    painter->drawRoundedRect(getNodeRect(), 5, 5);
    QFontMetrics textMetrics = painter->fontMetrics();
    QRectF textRect = textMetrics.boundingRect("Node");
    painter->drawText(QPointF(size.x()/2 - textRect.width()/2, size.y()/2 + textRect.height()/2), "Node");
    
    for (auto i = ports.cbegin(); i != ports.cend(); ++i)
    {
        i.value()->paint(painter);
    }
}

QRectF NodeGraphicsItem::boundingRect() const
{
    return getNodeRect();
}

QPainterPath NodeGraphicsItem::shape() const
{
    QPainterPath path;
    path.addRect(getNodeRect());
    return path;
}

QRectF NodeGraphicsItem::getNodeRect() const
{
    return QRectF(QPointF(0, 0), size);
}

void NodeGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    
    
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
    
    if (isPort){
        ungrabMouse();
        graphView->beginConnection(this, port);
    }
    else {
        if (event->modifiers() & Qt::CTRL)
            graphView->addToSelection(uuid);
        else if (!nodeIsSelected) graphView->select(uuid);
    }
}
void NodeGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    GraphView *graphView = getGraphView(event);
    ungrabMouse();
    graphView->beginMove(event->scenePos());
}

QVariant NodeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == GraphicsItemChange::ItemScenePositionHasChanged)
        emit nodeMoved(pos());
    return QGraphicsItem::itemChange(change, value);
}

GraphView * NodeGraphicsItem::getGraphView(QGraphicsSceneMouseEvent *event)
{
    return dynamic_cast<GraphView*>(event->widget()->parentWidget());
}
