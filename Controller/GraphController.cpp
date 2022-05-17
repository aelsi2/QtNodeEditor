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

QUuid GraphController::createNode(NodeType type, QPointF position)
{
    QUuid uuid = graph->createNode(type, position);
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, uuid));
    return uuid;
}

void GraphController::deleteSelection()
{
    QJsonObject serializedSubgraph;
    QJsonArray serializedNodes;
    QJsonArray serializedConnections;
    for (auto i = selection.begin(); i != selection.end(); ++i)
    {
        Node *node = graph->getNode(*i);
        if (node == nullptr) continue;
        QMap<QUuid, Connection*>::const_iterator j;
        while ((j = node->connectionsConstBegin()) != node->connectionsConstEnd())
        {
            serializedConnections.append(serializeConnection(*graph, j.key()));
            graph->disconnect(j.key());
        }
        serializedNodes.append(serializeNode(*graph, *i));
        graph->deleteNode(*i);
    }
    serializedSubgraph["nodes"] = serializedNodes;
    serializedSubgraph["connections"] = serializedConnections;
    qDebug() << serializedSubgraph;
}

void GraphController::cutSelectionToClipboard()
{
    
}

void GraphController::copySelectionToClipboard()
{
    
}

void GraphController::pasteClipboard(QPointF position)
{
    clearSelection();
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

void GraphController::restoreJson(QJsonObject &json, QPointF offset, bool select)
{
    QJsonArray serializedNodes = json["nodes"].toArray();
    QJsonArray serializedConnections = json["connections"].toArray();
    for (auto i = serializedNodes.begin(); i != serializedNodes.end(); ++i)
    {
        restoreNode(*graph, i->toObject());
    }
    for (auto i = serializedConnections.begin(); i != serializedConnections.end(); ++i)
    {
        restoreConnection(*graph, i->toObject());
    }
}
