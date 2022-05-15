#include "GraphController.hpp"

GraphController::GraphController(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard)
    : graph(nodeGraph), undoStack(undoStack), clipboard(clipboard) {}

void GraphController::selectNode(QUuid nodeId)
{
    selection.insert(nodeId);
}

void GraphController::deselectNode(QUuid nodeId)
{
    selection.remove(nodeId);
}

void GraphController::clearSelection()
{
    selection.clear();
}

void GraphController::createNode(NodeType type, QPointF position)
{
    QUuid uuid = graph->createNode(type, position);
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, uuid));
}

void GraphController::deleteSelection()
{
    QSet<Connection*> connections;
    for (auto i = selection.begin(); i != selection.end(); ++i)
    {
        Node *node = graph->getNode(*i);
        if (node == nullptr) continue;
    }
}

bool GraphController::connectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const
{
    return graph->connectable(nodeIdA, nodeIdB, portIdA, portIdB);
}

void GraphController::performConnectAction(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB)
{
    ConnectAction::Pair actions = graph->getConnectActions(nodeIdA, nodeIdB, portIdA, portIdB);
    if (actions.first.createNew && actions.second.createNew)
    {
        graph->disconnect(actions.first.connectionToDelete);
        if (actions.first.connectionToDelete != actions.second.connectionToDelete)
            graph->disconnect(actions.second.connectionToDelete);
        graph->connect(nodeIdA, nodeIdB, portIdA, portIdB);
    }
    else if (actions.first.disconnectExisting && actions.second.disconnectExisting &&
             actions.first.connectionToDelete == actions.second.connectionToDelete)
    {
        graph->disconnect(actions.first.connectionToDelete);
    }
}
