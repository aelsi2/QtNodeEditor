#pragma once

#include <QObject>
#include <QUuid>
#include <QPointF>
#include <QJsonValue>
#include <QVector>

#include "Common/HelperTypes.hpp"
#include "NodeGraph.hpp"
#include "Connection.hpp"

class NodeGraph;
class Connection;

class Node : public QObject
{
    Q_OBJECT  
public:
    Node(NodeGraph *graph, NodeType type, QPointF position, QUuid uuid);
    Node(NodeGraph *graph, NodeType type);
    
    void moveTo(QPointF newPosition);
    void addConnection(QUuid connectionId, PortID portId, Connection *connection);
    void removeConnection(QUuid connectionId, PortID portId);
    
    bool hasConnections() const;
    virtual bool isConnectedTo(QUuid otherNodeId, PortID otherPortId, PortID portId) const;
    virtual PortDataType getPortDataType(PortID portId) const;
    virtual QUuid getPortConnection(PortID portId) const;
    
    virtual void serializeData(QJsonValue &json) const;
    virtual void restoreData(const QJsonValue &json);
    
    QMap<QUuid, Connection*> const & getConnections() const;
    
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

