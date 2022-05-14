#pragma once

#include <QUuid>
#include <QPointF>
#include <QMap>
#include <QMutableMapIterator>
#include <QObject>

#include "Node.hpp"
#include "Connection.hpp"
#include "NodeType.hpp"
#include "PortUtils.hpp"
#include "Controller/NodeFactory.hpp"

class Node;
class Connection;
class NodeFactory;

class NodeGraph : public QObject
{
    Q_OBJECT
public:
    NodeGraph(NodeFactory *nodeFactory);
    
    void updateNodePosition(QUuid uuid, QPointF newPosition);
    QUuid createNode(NodeType type, QPointF position, QUuid uuid = QUuid::createUuid());
    void deleteNode(QUuid uuid);
    bool connectable(QUuid nodeIdA, QUuid nodeIdB, 
                     PortID portIdA, PortID portIdB) const;
    //Doesn't check if the corresponding nodes/ports are connectable!
    void connect(QUuid nodeIdA, QUuid NodeIdB, 
                 PortID portIdA, PortID portIdB);
    void disconnect(QUuid connectionId);
    
    Node* getNode(QUuid uuid) const;
    Connection* getConnection(QUuid uuid) const;

signals:
    void nodeMoved(QUuid, QPointF position) const;
    void nodeCreated(NodeType type, QUuid uuid, Node *node, QPointF position);
    void nodeDeleted(QUuid uuid);
    void connectionMade(QUuid uuid, Connection *connection,
                        QUuid firstNode, QUuid secondNode,
                        PortID firstPort, PortID secondPort);
    void connectionRemoved(QUuid uuid);
    
protected:
    void deleteNodeInternal(QUuid uuid);
    
    NodeFactory* factory;
    QMap<QUuid, Node*> nodes;
    QMap<QUuid, Connection*> connections;
};

