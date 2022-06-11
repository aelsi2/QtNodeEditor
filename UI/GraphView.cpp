#include "GraphView.hpp"

GraphView::GraphView(QGraphicsScene *scene) : QGraphicsView(scene)
{
    if (scene != nullptr) QObject::connect(scene, &QGraphicsScene::selectionChanged, this, &GraphView::syncSelection);
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    Qt::MouseButtons buttons = event->buttons();
    if (dragState.type != DragState::Type::Idle) return;
    if (buttons & Qt::RightButton)
    {
        
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent *event)
{
    Qt::MouseButtons buttons = event->buttons();
    if (!buttons)
    {
        dragState.type = DragState::Type::Idle;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::syncSelection()
{
    selection.clear();
    auto items = scene()->selectedItems();
    for (auto i = items.cbegin(); i != items.cend(); ++i)
    {
        QGraphicsItem *item = *i;
        NodeGraphicsItem *node = dynamic_cast<NodeGraphicsItem*>(item);
        if (node == nullptr) continue;
        selection.insert(node->getUuid());
    }
    qDebug() << selection.size();
}

void GraphView::BeginAddNode(QPointF)
{
    
}
