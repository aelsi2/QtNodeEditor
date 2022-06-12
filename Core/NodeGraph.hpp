#pragma once

#include <QUuid>
#include <QPointF>
#include <QMap>
#include <QMutableMapIterator>
#include <QObject>

#include "Node.hpp"
#include "Connection.hpp"
#include "Common/HelperTypes.hpp"
#include "NodeFactory.hpp"

class Node;
class Connection;
class NodeFactory;

class NodeGraph : public QObject
{
    Q_OBJECT
public:
    NodeGraph(NodeFactory *nodeFactory);
    
    void moveNodeTo(QUuid uuid, QPointF newPosition);
    QUuid createNode(NodeType type, QPointF position, QUuid uuid = QUuid::createUuid());
    void deleteNode(QUuid uuid);
    
    QUuid connect(QUuid nodeIdA, QUuid NodeIdB,
                 PortID portIdA, PortID portIdB, QUuid connectionId = QUuid::createUuid());
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
    NodeFactory * nodeFactory;
    QMap<QUuid, Node*> nodes;
    QMap<QUuid, Connection*> connections;
};

