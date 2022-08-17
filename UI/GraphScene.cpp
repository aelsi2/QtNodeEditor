#include "GraphScene.hpp"

GraphScene::GraphScene(GraphicsFactory *factory, NodeGraph *graph, GraphEditor *editor)
    : QGraphicsScene(), factory(factory)
{
    QObject::connect(graph, &NodeGraph::nodeMoved, this, &GraphScene::nodeMoved);
    QObject::connect(graph, &NodeGraph::nodeCreated, this, &GraphScene::nodeCreated);
    QObject::connect(graph, &NodeGraph::nodeDeleted, this, &GraphScene::nodeDeleted);
    QObject::connect(graph, &NodeGraph::connectionMade, this, &GraphScene::connectionMade);
    QObject::connect(graph, &NodeGraph::connectionRemoved, this, &GraphScene::connectionRemoved);
    QObject::connect(editor, &GraphEditor::nodeSelected, this, &GraphScene::nodeSelected);
    QObject::connect(editor, &GraphEditor::nodeDeselected, this, &GraphScene::nodeDeselected);
    QObject::connect(editor, &GraphEditor::selectionCleared, this, &GraphScene::selectionCleared);
}


QMap<QUuid, NodeGraphicsItem*> GraphScene::getGraphSelection() const
{
    return selection;
}

NodeGraphicsItem* GraphScene::getNodeItem(QUuid uuid) const
{
    return nodes.value(uuid, nullptr);
}

ConnectionGraphicsItem* GraphScene::getConnectionItem(QUuid uuid) const
{
    return connections.value(uuid, nullptr);
}

void GraphScene::nodeMoved(QUuid nodeId, QPointF position)
{
    NodeGraphicsItem *nodeItem = nodes.value(nodeId);
    if (nodeItem == nullptr) return;
    nodeItem->setPos(position);
}
void GraphScene::nodeCreated(NodeType type, QUuid uuid, Node *node, QPointF position)
{
    NodeGraphicsItem *nodeItem = factory->createNode(type, position, node, uuid);
    if (nodeItem == nullptr) return;
    addItem(nodeItem);
    nodes.insert(uuid, nodeItem);
}
void GraphScene::nodeDeleted(QUuid uuid)
{
    selection.remove(uuid);
    NodeGraphicsItem *nodeItem = nodes.value(uuid);
    if (nodeItem == nullptr) return;
    nodes.remove(uuid);
    removeItem(nodeItem);
    delete nodeItem;
}
void GraphScene::connectionMade(QUuid uuid, Connection *connection,
                    QUuid nodeIdA, QUuid nodeIdB,
                    PortID portIdA, PortID portIdB)
{
    Q_UNUSED(connection)
    auto nodeItemA = nodes.value(nodeIdA, nullptr);
    auto nodeItemB = nodes.value(nodeIdB, nullptr);
    if (nodeItemA == nullptr || nodeItemB == nullptr) return;
    auto connectionItem = factory->createConnection(uuid, nodeItemA, nodeItemB, portIdA, portIdB);
    connections.insert(uuid, connectionItem);
    addItem(connectionItem);
}
void GraphScene::connectionRemoved(QUuid uuid)
{
    ConnectionGraphicsItem *connectionItem = connections.value(uuid);
    if (connectionItem == nullptr) return;
    connections.remove(uuid);
    removeItem(connectionItem);
    delete connectionItem;
}

void GraphScene::nodeSelected(QUuid uuid)
{
    
    NodeGraphicsItem *nodeItem = nodes.value(uuid);
    if (nodeItem == nullptr) return;
    selection.insert(uuid, nodeItem);
    nodeItem->setNodeSelected(true);
}

void GraphScene::nodeDeselected(QUuid uuid)
{
    selection.remove(uuid);
    NodeGraphicsItem *nodeItem = nodes.value(uuid);
    if (nodeItem == nullptr) return;
    nodeItem->setNodeSelected(false);
}
void GraphScene::selectionCleared()
{
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        i.value()->setNodeSelected(false);
    }
    selection.clear();
}
