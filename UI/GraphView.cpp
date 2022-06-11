#include "GraphView.hpp"

GraphView::GraphView(QGraphicsScene *scene) : QGraphicsView(scene)
{
    
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

void GraphView::BeginAddNode(QPointF)
{
    
}
