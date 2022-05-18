#include "GraphEditor.hpp"

GraphEditor::GraphEditor(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard)
    : graph(nodeGraph), undoStack(undoStack), clipboard(clipboard) {}

void GraphEditor::selectNode(QUuid nodeId)
{
    selection.insert(nodeId);
}

void GraphEditor::deselectNode(QUuid nodeId)
{
    selection.remove(nodeId);
}

void GraphEditor::clearSelection()
{
    selection.clear();
}

QUuid GraphEditor::createNode(NodeType type, QPointF position)
{
    QUuid uuid = QUuid::createUuid();
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, uuid));
    return uuid;
}

void GraphEditor::deleteSelection()
{
    QSet<QUuid> connections;
    getConnectionsBetween(selection, connections);
    undoStack->beginMacro("Delete selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        undoStack->push(new ConnectUndoCommand(graph, *i));
    }
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        undoStack->push(new NodeDeleteUndoCommand(graph, *i));
    }
    undoStack->endMacro();
    clearSelection();
}

void GraphEditor::cutSelection()
{
    QSet<QUuid> connections;
    getConnectionsBetween(selection, connections);
    auto jsonGraph = SerializedGraph();
    undoStack->beginMacro("Cut selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        jsonGraph.addConnection(*graph, *i);
        undoStack->push(new ConnectUndoCommand(graph, *i));
    }
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        jsonGraph.addNode(*graph, *i);
        undoStack->push(new NodeDeleteUndoCommand(graph, *i));
    }
    undoStack->endMacro();
    clearSelection();
    clipboard->setText(jsonGraph.jsonDocument().toJson(QJsonDocument::Indented));
}

void GraphEditor::copySelection()
{
    QSet<QUuid> connections;
    getConnectionsBetween(selection, connections);
    auto jsonGraph = SerializedGraph();
    undoStack->beginMacro("Cut selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        jsonGraph.addConnection(*graph, *i);
    }
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        jsonGraph.addNode(*graph, *i);
    }
    undoStack->endMacro();
    clipboard->setText(jsonGraph.jsonDocument().toJson(QJsonDocument::Indented));
}

void GraphEditor::pasteClipboard()
{
    clearSelection();
    auto json = QJsonDocument::fromJson(clipboard->text().toUtf8());
    auto jsonGraph = SerializedGraph(json);
    QMap<QUuid, QUuid> nodeUuidLookup;
    undoStack->beginMacro("Paste clipboard");
    for (int i = 0; i < jsonGraph.nodeCount(); i++)
    {
        auto node = jsonGraph.getNode(i, true, &nodeUuidLookup);
        undoStack->push(new NodeCreateUndoCommand(graph, node.type, node.pos, node.data, node.uuid));
        selectNode(node.uuid);
    }
    for (int i = 0; i < jsonGraph.connectionCount(); i++)
    {
        auto connection = jsonGraph.getConnection(i, true, &nodeUuidLookup);
        undoStack->push(new ConnectUndoCommand(graph, connection.nodeIdA, connection.nodeIdB,
                                               connection.portIdA, connection.portIdB, connection.uuid));
    }
    undoStack->endMacro();
}

bool GraphEditor::connectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const
{
    return graph->connectable(nodeIdA, nodeIdB, portIdA, portIdB);
}

void GraphEditor::performConnectAction(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB)
{
    ConnectAction::Pair actions = graph->getConnectActions(nodeIdA, nodeIdB, portIdA, portIdB);
    
    if (actions.first.createNew && actions.second.createNew)
    {
        undoStack->beginMacro("Connect");
        if (actions.first.disconnectExisting)
        {
            undoStack->push(new ConnectUndoCommand(graph, actions.first.connectionToDelete));
        }
        if (actions.first.disconnectExisting &&
            actions.first.connectionToDelete != actions.second.connectionToDelete)
        {
            undoStack->push(new ConnectUndoCommand(graph, actions.second.connectionToDelete));
        }
        undoStack->push(new ConnectUndoCommand(graph, nodeIdA, nodeIdB, portIdA, portIdB));
        undoStack->endMacro();
    }
    else if (actions.first.disconnectExisting && actions.second.disconnectExisting &&
             actions.first.connectionToDelete == actions.second.connectionToDelete)
    {
        undoStack->push(new ConnectUndoCommand(graph, actions.first.connectionToDelete));
    }
}

void GraphEditor::getConnectionsBetween(QSet<QUuid> const &nodes, QSet<QUuid> &connections)
{
    for (auto i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        Node *node = graph->getNode(*i);
        if (node == nullptr) continue;
        for (auto j = node->connectionsConstBegin(); j != node->connectionsConstEnd(); ++j)
        {
            Connection *connection = graph->getConnection(j.key());
            if (connection == nullptr) continue;
            
            if (nodes.contains(connection->getFirstNodeId()) &&
                nodes.contains(connection->getSecondNodeId()))
            {
                connections.insert(j.key());
            }
        }
    }
}
