#include "UndoCommands.hpp"

NodeGraphUndoCommand::NodeGraphUndoCommand(NodeGraph *nodeGraph) : graph(nodeGraph) {}

NodeCreateUndoCommand::NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF position, QJsonValue const &internalData, QUuid uuid)
    : NodeGraphUndoCommand(graph),
    type(nodeType),
    uuid(uuid),
    pos(position),
    data(internalData) {}

NodeCreateUndoCommand::NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF position, QUuid uuid)
    : NodeGraphUndoCommand(graph),
    type(nodeType),
    uuid(uuid),
    pos(position) {}

void NodeCreateUndoCommand::undo()
{
    graph->deleteNode(uuid);
}

void NodeCreateUndoCommand::redo()
{
    graph->createNode(type, pos, uuid);
    if (!data.has_value()) return;
    Node *node = graph->getNode(uuid);
    if (node == nullptr) return;
    node->restoreData(data.value());
}

NodeDeleteUndoCommand::NodeDeleteUndoCommand(NodeGraph *graph, QUuid uuid, NodeType nodeType, QPointF position, QJsonValue const &internalData)
    : NodeGraphUndoCommand(graph),
    type(nodeType),
    uuid(uuid),
    pos(position),
    data(internalData) {}

NodeDeleteUndoCommand::NodeDeleteUndoCommand(NodeGraph *graph, QUuid uuid)
    : NodeGraphUndoCommand(graph),
    uuid(uuid)
{
    Node *node = graph->getNode(uuid);
    if (node == nullptr) return;
    type = node->type;
    pos = node->getPosition();
    QJsonValue json;
    node->serializeData(json);
    data = json;
}

void NodeDeleteUndoCommand::undo()
{
    graph->createNode(type, pos, uuid);
    Node *node = graph->getNode(uuid);
    if (node == nullptr) return;
    node->restoreData(data);
}

void NodeDeleteUndoCommand::redo()
{
    graph->deleteNode(uuid);
}

ConnectUndoCommand::ConnectUndoCommand(NodeGraph *graph, QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB, QUuid uuid, bool disconnect)
    : NodeGraphUndoCommand(graph),
    connectionId(uuid),
    nodeIdA(nodeIdA),
    nodeIdB(nodeIdB),
    portIdA(portIdA),
    portIdB(portIdB),
    disconnect(disconnect) {}

ConnectUndoCommand::ConnectUndoCommand(NodeGraph *graph, QUuid uuid)
    : NodeGraphUndoCommand(graph),
    connectionId(uuid),
    disconnect(true)
{
    Connection *connection = graph->getConnection(connectionId);
    if (connection == nullptr) return;
    nodeIdA = connection->getFirstNodeId();
    nodeIdB = connection->getSecondNodeId();
    portIdA = connection->getFirstPort();
    portIdB = connection->getSecondPort();
}

void ConnectUndoCommand::undo()
{
    if (disconnect)
        graph->connect(nodeIdA, nodeIdB, portIdA, portIdB, connectionId);
    else
        graph->disconnect(connectionId);
}

void ConnectUndoCommand::redo()
{
    if (disconnect)
        graph->disconnect(connectionId);
    else
        graph->connect(nodeIdA, nodeIdB, portIdA, portIdB, connectionId);
}
