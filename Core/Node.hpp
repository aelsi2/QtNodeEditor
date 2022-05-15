#pragma once

#include <QObject>
#include <QUuid>
#include <QPointF>
#include <optional>
#include <QVector>

#include "NodeGraph.hpp"
#include "Connection.hpp"
#include "PortUtils.hpp"
#include "ConnectAction.hpp"
#include "JSONSerializable.hpp"

class NodeGraph;
class Connection;

class Node : public QObject
{
    Q_OBJECT  
public:
    Node(NodeGraph *graph, QPointF position, QUuid uuid);
    
    bool hasConnections() const;
    void updatePosition(QPointF newPosition);
    void addConnection(QUuid connectionId, PortID portId, Connection *connection);
    void removeConnection(QUuid connectionId, PortID portId);
    void removeConnection(QUuid connectionId); //Should only be used before the connection object gets deleted
    
    //Intended to be overriden but some default implementations are provided
    virtual PortDataType getPortDataType(PortID portId) const;
    virtual bool connectable(PortID portId, QUuid otherNodeId, PortID otherPortId, PortDataType otherdataType) const; //Default implementation relies on getConnectionAction(...)
    virtual ConnectAction getConnectAction(PortID portId, QUuid otherNodeId, PortID otherPortId, PortDataType otherdataType) const;
    
    //Const iterators for node connections
    QMap<QUuid, Connection*>::const_iterator connectionsConstBegin() const;
    QMap<QUuid, Connection*>::const_iterator connectionsConstEnd() const;
    
    QUuid getUuid() const;
    NodeGraph* getGraph() const;
    
protected:
    //Called when the corresponding public methods are called (after the changes)
    virtual void onConnectionAdded(QUuid connectionId, PortID portId, Connection *connection);
    virtual void onConnectionRemoved(QUuid connectionId, PortID portId);
    
    QMap<QUuid, Connection*> connections;
    NodeGraph* graph;
    QUuid uuid;
    QPointF position;
};

