#include "GraphEditor.hpp"

const ConnectionDragAction ConnectionDragAction::New = ConnectionDragAction{ConnectionDragAction::Type::New, QUuid()};
const ConnectionDragAction ConnectionDragAction::None = ConnectionDragAction{ConnectionDragAction::Type::None, QUuid()};
ConnectionDragAction ConnectionDragAction::ModifyExisting(QUuid uuid) { return ConnectionDragAction{ConnectionDragAction::Type::ModifyExisting, uuid}; }

GraphEditor::GraphEditor(NodeGraph * nodeGraph, QUndoStack * undoStack, QClipboard * clipboard)
    : graph(nodeGraph), undoStack(undoStack), clipboard(clipboard) {}

void GraphEditor::doSelectNode(QUuid nodeId)
{
    emit requestSelect(nodeId);
}

void GraphEditor::doDeselectNode(QUuid nodeId)
{
    emit requestDeselect(nodeId);
}

void GraphEditor::doClearSelection()
{
    emit requestClearSelection();
}

void GraphEditor::move(NodeCollection const &nodes, QPointF delta)
{
    QSet<QUuid> nodesCopy = nodes;
    for (auto i = nodesCopy.cbegin(); i != nodesCopy.cend(); ++i) {
        doMoveNode(*i, delta);
    }
}

QUuid GraphEditor::createNode(NodeType type, QPointF position)
{
    QUuid uuid = QUuid::createUuid();
    doCreateNode(type, position, uuid);
    return uuid;
}

void GraphEditor::deleteNodes(NodeCollection const &nodes)
{
    QSet<QUuid> nodesCopy = nodes;
    QSet<QUuid> connections;
    getConnectionsBetween(nodesCopy, connections);
    beginMacro("Delete selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        doDeleteConnection(*i);
    }
    for (auto i = nodesCopy.cbegin(); i != nodesCopy.cend(); ++i)
    {
        doDeleteNode(*i);
    }
    endMacro();
    doClearSelection();
}

void GraphEditor::cut(NodeCollection const &nodes)
{
    QSet<QUuid> nodesCopy = nodes; //Copy nodes to a new QSet to prevent errors from items being deselected while the method is executing
    QSet<QUuid> connections;
    getConnectionsBetween(nodesCopy, connections);
    auto jsonGraph = SerializedGraph();
    undoStack->beginMacro("Cut selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        jsonGraph.addConnection(*graph, *i);
        undoStack->push(new ConnectUndoCommand(graph, *i));
    }
    for (auto i = nodesCopy.cbegin(); i != nodesCopy.cend(); ++i)
    {
        jsonGraph.addNode(*graph, *i);
        undoStack->push(new NodeDeleteUndoCommand(graph, *i));
    }
    undoStack->endMacro();
    doClearSelection();
    setClipboard(jsonGraph.toString());
}

void GraphEditor::copy(NodeCollection const &nodes)
{
    QSet<QUuid> connections;
    getConnectionsBetween(nodes, connections);
    auto jsonGraph = SerializedGraph();
    beginMacro("Cut selection");
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        jsonGraph.addConnection(*graph, *i);
    }
    for (auto i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        jsonGraph.addNode(*graph, *i);
    }
    endMacro();
    setClipboard(jsonGraph.toString());
}

void GraphEditor::pasteClipboard()
{
    doClearSelection();
    auto jsonGraph = SerializedGraph(getClipboard());
    QMap<QUuid, QUuid> nodeUuidLookup;
    beginMacro("Paste clipboard");
    for (int i = 0; i < jsonGraph.nodeCount(); i++)
    {
        auto node = jsonGraph.getNode(i, true, &nodeUuidLookup);
        doCreateNode(node.type, node.pos, node.uuid);
        doSelectNode(node.uuid);
    }
    for (int i = 0; i < jsonGraph.connectionCount(); i++)
    {
        auto connection = jsonGraph.getConnection(i, true, &nodeUuidLookup);
        doCreateConnection(connection.nodeIdA, connection.nodeIdB, connection.portIdA, connection.portIdB);
    }
    endMacro();
}

void GraphEditor::duplicate(NodeCollection const &nodes)
{
    QSet<QUuid> nodesCopy = nodes;
    QSet<QUuid> connections;
    QMap<QUuid, QUuid> nodeUuidLookup;
    getConnectionsBetween(nodesCopy, connections);
    doClearSelection();
    beginMacro("Duplicate selection");
    for (auto i = nodesCopy.cbegin(); i != nodesCopy.cend(); ++i)
    {
        QUuid newUuid = QUuid::createUuid();
        Node * node = graph->getNode(*i);
        doCreateNode(node, newUuid);
        nodeUuidLookup.insert(node->getUuid(), newUuid);
        doSelectNode(newUuid);
    }
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        Connection * connection = graph->getConnection(*i);
        QUuid nodeIdA = nodeUuidLookup.value(connection->getFirstNodeId(), connection->getFirstNodeId());
        QUuid nodeIdB = nodeUuidLookup.value(connection->getSecondNodeId(), connection->getSecondNodeId());
        PortID portIdA = connection->getFirstPort();
        PortID portIdB = connection->getSecondPort();
        doCreateConnection(nodeIdA, nodeIdB, portIdA, portIdB);
    }
    endMacro();
}

bool GraphEditor::getConnectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const
{
    if (portIdA.direction == portIdB.direction) return false;
    Node * nodeA = graph->getNode(nodeIdA);
    Node * nodeB = graph->getNode(nodeIdB);
    if (nodeA == nullptr || nodeB == nullptr) return false;
    if (nodeA->getPortDataType(portIdA) != nodeB->getPortDataType(portIdB)) return false;
    return true;
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

void GraphEditor::connect(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB)
{
    if (portIdA.direction != PortDirection::INOUT && portIdA.direction == portIdB.direction) return;
    Node * nodeA = graph->getNode(nodeIdA);
    Node * nodeB = graph->getNode(nodeIdB);
    if (nodeA == nullptr || nodeB == nullptr) return;
    if (nodeA->isConnectedTo(nodeIdB, portIdB, portIdA))
    {
        doDeleteConnection(nodeA->getPortConnection(portIdA));
        return;
    }
    beginMacro("Connect nodes");
    if (portIdA.direction == PortDirection::IN) doDeleteConnection(nodeA->getPortConnection(portIdA));
    else if (portIdB.direction == PortDirection::IN)  doDeleteConnection(nodeB->getPortConnection(portIdB));
    doCreateConnection(nodeIdA, nodeIdB, portIdA, portIdB);
    endMacro();
}

void GraphEditor::removeConnections(QVector<QUuid> const & connectionIds)
{
    beginMacro("Disconnect multiple");
    for (auto i = connectionIds.cbegin(); i != connectionIds.cend(); ++i)
    {
        doDeleteConnection(*i);
    }
    endMacro();
}

NodeChangeContext GraphEditor::beginNodeStateChange(QUuid nodeId) const
{
    return NodeChangeContext(graph, undoStack, nodeId);
}

void GraphEditor::getConnectionsBetween(QSet<QUuid> const & nodes, QSet<QUuid> & connections)
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
void GraphEditor::beginMacro(QString const & name)
{
    undoStack->beginMacro(name);
}
void GraphEditor::endMacro()
{
    undoStack->endMacro();
}
void GraphEditor::doCreateNode(const Node *prototype, QUuid uuid)
{
    if (prototype == nullptr) return;
    QPointF pos = prototype->getPosition();
    NodeType type = prototype->type;
    QJsonValue data;
    prototype->serializeData(data);
    doCreateNode(type, pos, data, uuid);
}
void GraphEditor::doCreateNode(NodeType type, QPointF position, QUuid uuid)
{
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, uuid));
}
void GraphEditor::doCreateNode(NodeType type, QPointF position, QJsonValue const & data, QUuid uuid)
{
    undoStack->push(new NodeCreateUndoCommand(graph, type, position, data, uuid));
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
void GraphEditor::setClipboard(const QString & text)
{
    clipboard->setText(text);
}
QString GraphEditor::getClipboard() const
{
    return clipboard->text();
}
