#include "Node.hpp"

Node::Node(NodeGraph *graph, NodeType type, QPointF position, QUuid uuid)
    : type(type), graph(graph), uuid(uuid), position(position) {}

Node::Node(NodeGraph *graph, NodeType type)
    : type(type), graph(graph), uuid(), position(0.0, 0.0) {}

QUuid Node::getUuid() const { return uuid; }
NodeGraph* Node::getGraph() const { return graph; }
QPointF Node::getPosition() const { return position; }

QMap<QUuid, Connection*> const & Node::getConnections() const { return connections; }

bool Node::hasConnections() const
{
    return connections.size() != 0;
}

void Node::moveTo(QPointF newPosition)
{
    position = newPosition;
}

void Node::addConnection(QUuid connectionId, PortID portId, Connection *connection)
{
    if (connections.contains(connectionId)) return;
    connections.insert(connectionId, connection);
    onConnectionAdded(connectionId, portId, connection);
}

void Node::removeConnection(QUuid connectionId, PortID portId)
{
    if (!connections.contains(connectionId)) return;
    connections.remove(connectionId);
    onConnectionRemoved(connectionId, portId);
}

bool Node::isConnectedTo(QUuid otherNodeId, PortID otherPortId, PortID portId) const
{
    for (auto i = connections.constBegin(); i != connections.constEnd(); i++)
    {
        Connection * conn = i.value();
        if (!conn->contains(uuid, portId)) continue;
        if (conn->contains(otherNodeId, otherPortId)) return true;
    }
    return false;
}

PortDataType Node::getPortDataType(PortID portId) const
{
    Q_UNUSED(portId)
    return PortDataType();
}

QUuid Node::getPortConnection(PortID portId) const
{
    for (auto i = connections.constBegin(); i != connections.constEnd(); i++)
    {
        if (i.value()->getThisPort(uuid) == portId) return i.key();
    }
    return QUuid();
}

void Node::onConnectionAdded(QUuid connectionId, PortID portId, Connection *connection)
{
    Q_UNUSED(connectionId)
    Q_UNUSED(portId)
    Q_UNUSED(connection)
}

void Node::onConnectionRemoved(QUuid connectionId, PortID portId)
{
    Q_UNUSED(connectionId)
    Q_UNUSED(portId)
}

void Node::serializeData(QJsonValue &json) const
{
    Q_UNUSED(json)
}

void Node::restoreData(const QJsonValue &json)
{
    Q_UNUSED(json)
}
