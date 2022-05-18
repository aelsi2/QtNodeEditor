#include "Connection.hpp"

Connection::Connection(NodeGraph &graph, 
                       QUuid firstNode, QUuid secondNode, 
                       PortID firstPort, PortID secondPort) 
    : graph(graph),
      uuid(QUuid::createUuid()),
      nodeIdA(firstNode),
      nodeIdB(secondNode), 
      portIdA(firstPort), 
      portIdB(secondPort) {}

Connection::Connection(NodeGraph &graph, 
                       QUuid firstNode, QUuid secondNode, 
                       PortID firstPort, PortID secondPort, QUuid uuid) 
    : graph(graph),
      uuid(uuid),
      nodeIdA(firstNode),
      nodeIdB(secondNode), 
      portIdA(firstPort), 
      portIdB(secondPort) {}

Node* Connection::getFirstNode() const { return graph.getNode(nodeIdA); }
Node* Connection::getSecondNode() const { return graph.getNode(nodeIdB); }

QUuid Connection::getFirstNodeId() const { return nodeIdA; }
QUuid Connection::getSecondNodeId() const { return nodeIdB; }

PortID Connection::getFirstPort() const { return portIdA; }
PortID Connection::getSecondPort() const { return portIdB; }

PortID Connection::getOtherPort(QUuid nodeId) const
{
    if (nodeId == nodeIdB) return portIdA;
    else return portIdB;
}

PortID Connection::getThisPort(QUuid nodeId) const
{
    if (nodeId == nodeIdA) return portIdA;
    else return portIdB;
}

QUuid Connection::getOtherNodeId(QUuid nodeId) const
{
    if (nodeId == nodeIdA) return nodeIdB;
    else return nodeIdB;
}

Node* Connection::getOtherNode(QUuid nodeId) const
{
    if (nodeId == nodeIdA) return getSecondNode();
    return getFirstNode();
}

bool Connection::contains(QUuid nodeId, PortID portId) const
{
    if (nodeIdA == nodeId && portIdA == portId) return true;
    if (nodeIdB == nodeId && portIdB == portId) return true;
    return false;
}

NodeGraph& Connection::getGraph() const { return graph;}
QUuid Connection::getUuid() const { return uuid;}
