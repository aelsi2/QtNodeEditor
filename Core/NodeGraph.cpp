#include "NodeGraph.hpp"

NodeGraph::NodeGraph(NodeFactory *nodeFactory) : nodeFactory(nodeFactory) {}

void NodeGraph::updateNodePosition(QUuid uuid, QPointF newPosition)
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
    if (node->hasConnections()){
        QMutableMapIterator<QUuid, Connection*> i(connections);
        for (i.next(); i.hasNext();){
            if (i.value()->getFirstNode() == node || i.value()->getSecondNode() == node){
                QUuid connectionId = i.value()->getUuid();
                disconnect(connectionId);
            }
            else i.next();
        }
    }
    nodes.remove(uuid);
    delete node;
    emit nodeDeleted(uuid);
    return;
}

bool NodeGraph::connectable(QUuid nodeIdA, QUuid nodeIdB, 
                            PortID portIdA, PortID portIdB) const
{
    //Check if the given nodes exist in the dictionary and get pointers to them
    if (!nodes.contains(nodeIdA) || !nodes.contains(nodeIdB)) return false;
    Node *nodeA = nodes.value(nodeIdA);
    Node *nodeB = nodes.value(nodeIdB);
    //Prohibit input-to-input and output-to-output connections
    if (portIdA.direction == portIdB.direction) return false;
    //Get the ports' data types and check if the opposite nodes accept them
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

void NodeGraph::connect(QUuid nodeIdA, QUuid nodeIdB, 
                        PortID portIdA, PortID portIdB)
{
    if (!nodes.contains(nodeIdA) || !nodes.contains(nodeIdB)) return;
    Node *nodeA = nodes.value(nodeIdA);
    Node *nodeB = nodes.value(nodeIdB);
    
    QUuid connectionId = QUuid::createUuid();
    Connection *connection = new Connection(this, nodeIdA, nodeIdB, portIdA, portIdB, connectionId);
    connections.insert(connectionId, connection);
    
    nodeA->addConnection(connectionId, portIdA, connection);
    nodeB->addConnection(connectionId, portIdB, connection);
    
    emit connectionMade(connectionId, connection, nodeIdA, nodeIdB, portIdA, portIdB);
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
