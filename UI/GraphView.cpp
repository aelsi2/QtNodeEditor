#include "GraphView.hpp"

void GraphView::DragState::unsuspendConnection()
{
    if (suspendedConnection == nullptr) return;
    suspendedConnection->setVisible(true);
}

GraphView::GraphView(GraphScene *scene, GraphEditor *editor, QMap<QString, NodeType> const & nodes) : QGraphicsView(scene), editor(editor)
{
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setContextMenuPolicy(Qt::CustomContextMenu);
    graphScene = scene;
    addNodeWindow = new AddNodeWindow(editor, nodes);
}

void GraphView::showAddNodeWindow()
{
    addNodeWindow->setPosition(mapToGlobal(QPoint()), mapToScene(viewport()->rect().center()));
    addNodeWindow->show();
}

void GraphView::mousePressEvent(QMouseEvent *event)
{
    Qt::MouseButtons buttons = event->buttons();
    if (!(buttons & Qt::LeftButton)) return;
    if (dragState.type != DragState::Type::Idle) return;
    QGraphicsView::mousePressEvent(event);
    if (!event->isAccepted()) editor->clearSelection();
}

void GraphView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos = event->globalPosition();
    switch (dragState.type)
    {
    case DragState::Type::NodeMove:
        doNodeMove(event);
        break;
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphView::mouseReleaseEvent(QMouseEvent *event)
{
    switch (dragState.type)
    {
    case DragState::Type::Connection:
        doConnectionDrop(event->position());
        break;
    case DragState::Type::NodeMove:
        endNodeMove(event);
        break;
    }
    resetStateIfNothingPressed(event);
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphView::select(QUuid nodeId)
{
    editor->clearSelection();
    editor->selectNode(nodeId);
}
void GraphView::addToSelection(QUuid nodeId)
{
    editor->selectNode(nodeId);
}
void GraphView::deselect(QUuid nodeId)
{
    editor->deselectNode(nodeId);
}
void GraphView::clearSelection()
{
    editor->clearSelection();
}

void GraphView::beginMove(QPointF pos)
{
    dragState.type = DragState::Type::NodeMove;
    dragState.startScenePos = pos;
}
void GraphView::beginConnection(NodeGraphicsItem *node, PortID portId)
{
    QUuid nodeId = node->getUuid();
    auto action = editor->getDragAction(nodeId, portId);
    
    dragState.type = DragState::Type::Connection;
    dragState.suspendConnection = false;
    if (action.type == ConnectionDragAction::Type::ModifyExisting)
    {
        if (auto connection = graphScene->getConnectionItem(action.existingId))
        {
            dragState.suspendedConnection = connection;
            dragState.suspendConnection = true;
            connection->setVisible(false);
        }
    }
    dragState.connectionNodeA = node;
    dragState.connectionPortA = portId;
}

void GraphView::doNodeMove(QMouseEvent *event)
{
    if (initialNodePositions.isEmpty())
    {
        QMap<QUuid, NodeGraphicsItem*> selection = graphScene->getGraphSelection();
        for (auto i = selection.cbegin(); i != selection.cend(); ++i)
        {
            QGraphicsItem *item = i.value();
            initialNodePositions.insert(item, item->scenePos());
        }
    }
    QPointF mouseScenePos = mapToScene(event->pos());
    for (auto i = initialNodePositions.cbegin(); i != initialNodePositions.cend(); ++i)
    {
        QGraphicsItem *item = i.key();
        QPointF initialPos = i.value();
        item->setPos(initialPos + mouseScenePos - dragState.startScenePos);
    }
}

void GraphView::endNodeMove(QMouseEvent *event)
{
    if (dragState.type != DragState::Type::NodeMove) return;
    dragState.type = DragState::Type::Idle;
    initialNodePositions.clear();
    QPointF mouseScenePos = mapToScene(event->pos());
    editor->moveSelection(mouseScenePos - dragState.startScenePos);
}

void GraphView::doConnectionDrop(QPointF viewMousePos)
{
    if (dragState.type != DragState::Type::Connection) return;
    dragState.type = DragState::Type::Idle;
    
    QPointF sceneMousePos = mapToScene(viewMousePos.toPoint());
    QGraphicsItem *itemAtMouse = graphScene->itemAt(sceneMousePos, transform());
    
    NodeGraphicsItem *nodeItemA = dragState.connectionNodeA;
    NodeGraphicsItem *nodeItemB = dynamic_cast<NodeGraphicsItem*>(itemAtMouse);
    
    if (nodeItemA == nullptr) return;
    
    if (nodeItemB == nullptr)
    {
        if(dragState.suspendedConnection != nullptr)
            editor->disconnect(dragState.suspendedConnection->getUuid());
        return;
    }
    
    PortID portIdA = dragState.connectionPortA;
    PortID portIdB = nodeItemB->getNearestPortScene(sceneMousePos);

    if (dragState.suspendConnection && dragState.suspendedConnection != nullptr)
    {
        if (dragState.suspendedConnection->isConnectedTo(nodeItemB, portIdB))
        {
            dragState.unsuspendConnection();
            return;
        }
        editor->disconnect(dragState.suspendedConnection->getUuid());
    }
    if (nodeItemA == nodeItemB) return;
    QUuid nodeIdA = nodeItemA->getUuid();
    QUuid nodeIdB = nodeItemB->getUuid();
    editor->connect(nodeIdA, nodeIdB, portIdA, portIdB);
}

void GraphView::resetStateIfNothingPressed(QMouseEvent *event)
{
    if (event->buttons()) return;
    dragState.type = DragState::Type::Idle;
    initialNodePositions.clear();
}
