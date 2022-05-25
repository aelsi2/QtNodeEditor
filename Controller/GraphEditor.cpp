#include "GraphEditor.hpp"

const ConnectionDragAction ConnectionDragAction::New = ConnectionDragAction{ConnectionDragAction::Type::New, QUuid()};
const ConnectionDragAction ConnectionDragAction::None = ConnectionDragAction{ConnectionDragAction::Type::None, QUuid()};
ConnectionDragAction ConnectionDragAction::ModifyExisting(QUuid uuid) { return ConnectionDragAction{ConnectionDragAction::Type::ModifyExisting, uuid}; }

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

void GraphEditor::moveSelection(QPointF delta)
{
    for (auto i = selection.cbegin(); i != selection.cend(); ++i) {
        doMoveNode(*i, delta);
    }
}

QUuid GraphEditor::createNode(NodeType type, QPointF position)
{
    QUuid uuid = QUuid::createUuid();
    doCreateNode(type, position, uuid);
    return uuid;
}

void GraphEditor::deleteSelection()
{
    QSet<QUuid> connections;
    getConnectionsBetween(selection, connections);
    beginMacro("Delete selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        doDeleteConnection(*i);
    }
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        doDeleteNode(*i);
    }
    endMacro();
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
    setClipboard(jsonGraph.toString());
}

void GraphEditor::copySelection()
{
    QSet<QUuid> connections;
    getConnectionsBetween(selection, connections);
    auto jsonGraph = SerializedGraph();
    beginMacro("Cut selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        jsonGraph.addConnection(*graph, *i);
    }
    for (auto i = selection.cbegin(); i != selection.cend(); ++i)
    {
        jsonGraph.addNode(*graph, *i);
    }
    endMacro();
    setClipboard(jsonGraph.toString());
}

void GraphEditor::pasteClipboard()
{
    clearSelection();
    auto jsonGraph = SerializedGraph(getClipboard());
    QMap<QUuid, QUuid> nodeUuidLookup;
    beginMacro("Paste clipboard");
    for (int i = 0; i < jsonGraph.nodeCount(); i++)
    {
        auto node = jsonGraph.getNode(i, true, &nodeUuidLookup);
        doCreateNode(node.type, node.pos, node.uuid);
        selectNode(node.uuid);
    }
    for (int i = 0; i < jsonGraph.connectionCount(); i++)
    {
        auto connection = jsonGraph.getConnection(i, true, &nodeUuidLookup);
        doCreateConnection(connection.nodeIdA, connection.nodeIdB, connection.portIdA, connection.portIdB);
    }
    endMacro();
}

bool GraphEditor::getConnectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const
{
    return graph->connectable(nodeIdA, nodeIdB, portIdA, portIdB);
}
ConnectionDragAction GraphEditor::getDragAction(QUuid nodeId, PortID portId) const
{   
    Node * node = graph->getNode(nodeId);
    if (node == nullptr) return ConnectionDragAction::None;
    if (portId.direction == PortDirection::OUT) return ConnectionDragAction::New;
    QUuid connection = node->getPortConnection(portId);
    if (connection == QUuid()) return ConnectionDragAction::New;
    return ConnectionDragAction::ModifyExisting(connection);
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
        auto nodeConnections = node->getConnections();
        for (auto j = nodeConnections.cbegin(); j != nodeConnections.cend(); ++j)
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
void GraphEditor::beginMacro(QString const &name)
{
    undoStack->beginMacro(name);
}
void GraphEditor::endMacro()
{
    undoStack->endMacro();
}
void GraphEditor::doCreateNode(NodeType type, QPointF position, QUuid uuid)
{
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, uuid));
}
void GraphEditor::doDeleteNode(QUuid uuid)
{
    undoStack->push(new NodeDeleteUndoCommand(graph, uuid));
}
void GraphEditor::doCreateConnection(QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB)
{
    undoStack->push(new ConnectUndoCommand(graph, nodeIdA, nodeIdB, portIdA, portIdB));
}
void GraphEditor::doDeleteConnection(QUuid uuid)
{
    undoStack->push(new ConnectUndoCommand(graph, uuid));
}
void GraphEditor::doMoveNode(QUuid uuid, QPointF newPos, bool relative)
{
    undoStack->push(new NodeMoveUndoCommand(graph, uuid, newPos, relative));
}
void GraphEditor::setClipboard(const QString &text)
{
    clipboard->setText(text);
}
QString GraphEditor::getClipboard() const
{
    return clipboard->text();
}
