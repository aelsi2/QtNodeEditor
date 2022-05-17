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
    clearSelection();
    serializedSubgraph["nodes"] = serializedNodes;
    serializedSubgraph["connections"] = serializedConnections;
}

void GraphController::cutSelectionToClipboard()
{
    QJsonObject serializedSubgraph;
    QJsonArray serializedNodes;
    QJsonArray serializedConnections;
    for (auto i = selection.begin(); i != selection.end(); ++i)
    {
        Node *node = graph->getNode(*i);
        if (node == nullptr) continue;
        serializedNodes.append(serializeNode(*graph, *i));
        
        QMap<QUuid, Connection*>::const_iterator j;
        while ((j = node->connectionsConstBegin()) != node->connectionsConstEnd())
        {
            if (selection.contains(graph->getConnection(j.key())->getFirstNodeId()) &&
                selection.contains(graph->getConnection(j.key())->getSecondNodeId()))
            {
                serializedConnections.append(serializeConnection(*graph, j.key()));
            }
            graph->disconnect(j.key());
        }
        graph->deleteNode(*i);
    }
    clearSelection();
    serializedSubgraph["nodes"] = serializedNodes;
    serializedSubgraph["connections"] = serializedConnections;
    clipboard->setText(QJsonDocument(serializedSubgraph).toJson(QJsonDocument::Compact));
}

void GraphController::copySelectionToClipboard()
{
    QJsonObject serializedSubgraph;
    QJsonArray serializedNodes;
    QJsonArray serializedConnections;
    QSet<QUuid> connections;
    for (auto i = selection.begin(); i != selection.end(); ++i)
    {
        Node *node = graph->getNode(*i);
        if (node == nullptr) continue;
        serializedNodes.append(serializeNode(*graph, *i));
        for (auto j = node->connectionsConstBegin(); j != node->connectionsConstEnd(); ++j)
        {
            if (selection.contains(graph->getConnection(j.key())->getFirstNodeId()) &&
                selection.contains(graph->getConnection(j.key())->getSecondNodeId()) &&
                !connections.contains(j.key()))
            {
                connections.insert(j.key());
                serializedConnections.append(serializeConnection(*graph, j.key()));
            }
        }
    }
    serializedSubgraph["nodes"] = serializedNodes;
    serializedSubgraph["connections"] = serializedConnections;
    clipboard->setText(QJsonDocument(serializedSubgraph).toJson(QJsonDocument::Compact));
}

void GraphController::pasteClipboard()
{
    clearSelection();
    restoreSubgraph(*graph, QJsonDocument::fromJson(clipboard->text().toUtf8()).object());
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
