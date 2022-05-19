#pragma once

#include <QObject>
#include <QUuid>
#include <QPointF>
#include <QJsonValue>
#include <QVector>

#include "HelperTypes.hpp"
#include "NodeGraph.hpp"
#include "Connection.hpp"
#include "HelperTypes.hpp"
#include "ConnectAction.hpp"

class NodeGraph;
class Connection;

class Node : public QObject
{
    Q_OBJECT  
public:
    Node(NodeGraph *graph, NodeType type, QPointF position, QUuid uuid);
    Node(NodeGraph *graph, NodeType type);
    
    bool hasConnections() const;
    void moveTo(QPointF newPosition);
    void addConnection(QUuid connectionId, PortID portId, Connection *connection);
    void removeConnection(QUuid connectionId, PortID portId);
    void removeConnection(QUuid connectionId); //Should only be used before the connection object gets deleted
    
    //Intended to be overriden but some default implementations are provided
    virtual PortDataType getPortDataType(PortID portId) const;
    virtual bool connectable(PortID portId, QUuid otherNodeId, PortID otherPortId, PortDataType otherdataType) const; //Default implementation relies on getConnectionAction(...)
    virtual ConnectAction getConnectAction(PortID portId, QUuid otherNodeId, PortID otherPortId, PortDataType otherdataType) const;
    virtual void serializeData(QJsonValue &json) const;
    virtual void restoreData(const QJsonValue &json);
    
    //Const iterators for node connections
    QMap<QUuid, Connection*>::const_iterator connectionsConstBegin() const;
    QMap<QUuid, Connection*>::const_iterator connectionsConstEnd() const;
    
    QUuid getUuid() const;
    NodeGraph* getGraph() const;
    QPointF getPosition() const;
    
    const NodeType type;
    
protected:
    //Called when the corresponding public methods are called (after the changes)
    virtual void onConnectionAdded(QUuid connectionId, PortID portId, Connection *connection);
    virtual void onConnectionRemoved(QUuid connectionId, PortID portId);
    
    QMap<QUuid, Connection*> connections;
    NodeGraph *graph;
    QUuid uuid;
    QPointF position;
};

