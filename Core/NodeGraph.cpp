#include "NodeGraph.hpp"

NodeGraph::NodeGraph(NodeFactory *nodeFactory) : nodeFactory(nodeFactory) {}

void NodeGraph::moveNodeTo(QUuid uuid, QPointF newPosition)
{
    if (!nodes.contains(uuid)) return;
    nodes.value(uuid)->updatePosition(newPosition);
    emit nodeMoved(uuid, newPosition);
}

QUuid NodeGraph::createNode(NodeType type, QPointF position, QUuid uuid)
{
    if (nodes.contains(uuid)) return uuid;
    Node *node = nodeFactory->createNode(type, this, position, uuid);
    nodes.insert(uuid, node);
    emit nodeCreated(type, uuid, node, position);
    return uuid;
}

void NodeGraph::deleteNode(QUuid uuid)
{
    if (!nodes.contains(uuid)) return;
    Node *node = nodes.value(uuid);
    nodes.remove(uuid);
    delete node;
    emit nodeDeleted(uuid);
    return;
}

bool NodeGraph::connectable(QUuid nodeIdA, QUuid nodeIdB, 
                            PortID portIdA, PortID portIdB) const
{
    if (!nodes.contains(nodeIdA) || !nodes.contains(nodeIdB)) return false;
    Node *nodeA = nodes.value(nodeIdA);
    Node *nodeB = nodes.value(nodeIdB);
    //if (portIdA.direction == portIdB.direction) return false;
    PortDataType portTypeA = nodeA->getPortDataType(portIdA);
    PortDataType portTypeB = nodeB->getPortDataType(portIdB);
    if (!nodeA->connectable(portIdA, nodeIdB, portIdB, portTypeB)) return false;
    if (!nodeB->connectable(portIdB, nodeIdA, portIdA, portTypeA)) return false;
    return true;
}

ConnectAction::Pair NodeGraph::getConnectActions(QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB) const
{
    if (!nodes.contains(nodeIdA) || !nodes.contains(nodeIdB)) return ConnectAction::Pair{ConnectAction::Nothing, ConnectAction::Nothing};
     Node *nodeA = nodes.value(nodeIdA);
     Node *nodeB = nodes.value(nodeIdB);
     PortDataType portTypeA = nodeA->getPortDataType(portIdA);
     PortDataType portTypeB = nodeB->getPortDataType(portIdB);
     return ConnectAction::Pair{
         nodeA->getConnectAction(portIdA, nodeIdB, portIdB, portTypeB),
         nodeB->getConnectAction(portIdB, nodeIdA, portIdA, portTypeA)
     };
}

QUuid NodeGraph::connect(QUuid nodeIdA, QUuid nodeIdB, 
                        PortID portIdA, PortID portIdB, QUuid connectionId)
{
    if (!nodes.contains(nodeIdA) || !nodes.contains(nodeIdB)) return QUuid();
    Node *nodeA = nodes.value(nodeIdA);
    Node *nodeB = nodes.value(nodeIdB);
    
    Connection *connection = new Connection(this, nodeIdA, nodeIdB, portIdA, portIdB, connectionId);
    connections.insert(connectionId, connection);
    
    nodeA->addConnection(connectionId, portIdA, connection);
    nodeB->addConnection(connectionId, portIdB, connection);
    
    emit connectionMade(connectionId, connection, nodeIdA, nodeIdB, portIdA, portIdB);
    return connectionId;
}

void NodeGraph::disconnect(QUuid connectionId)
{
    if (!connections.contains(connectionId)) return;
    Connection *connection = connections.value(connectionId);
    
    Node *nodeA = connection->getFirstNode();
    Node *nodeB = connection->getSecondNode();
    
    PortID portIdA = connection->getFirstPort();
    PortID portIdB = connection->getSecondPort();
    
    nodeA->removeConnection(connectionId, portIdA);
    nodeB->removeConnection(connectionId, portIdB);
    
    connections.remove(connectionId);
    
    delete connection;
    
    emit connectionRemoved(connectionId);
}

Node* NodeGraph::getNode(QUuid uuid) const
{
    if (!nodes.contains(uuid)) return nullptr;
    return nodes.value(uuid);
}

Connection* NodeGraph::getConnection(QUuid uuid) const
{
    if (!connections.contains(uuid)) return nullptr;
    return connections.value(uuid);
}
