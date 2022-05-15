#pragma once

#include <QUuid>

#include "Node.hpp"
#include "PortUtils.hpp"
#include "NodeGraph.hpp"
#include "JSONSerializable.hpp"

class Node;
class NodeGraph;

class Connection : public JSONSerializable
{
public:
    Connection(NodeGraph *graph,
               QUuid firstNode, QUuid secondNode,
               PortID firstPort, PortID secondPort, QUuid uuid);
    Connection(NodeGraph *graph,
               QUuid firstNode, QUuid secondNode,
               PortID firstPort, PortID secondPort);
    
    Node* getFirstNode() const;
    Node* getSecondNode() const;
    
    QUuid getFirstNodeId() const;
    QUuid getSecondNodeId() const;
    
    PortID getFirstPort() const;
    PortID getSecondPort() const;
    
    PortID getThisPort(QUuid nodeId) const;
    PortID getOtherPort(QUuid nodeId) const;
    QUuid getOtherNodeId(QUuid nodeId) const;
    Node* getOtherNode(QUuid nodeId) const;
    
    bool contains(QUuid nodeId, PortID portId) const;
    
    void serialize(QJsonObject &json) const override;
    void deserialize(QJsonObject &json) override;
    
    NodeGraph* getGraph() const;
    QUuid getUuid() const;
    
private:
    NodeGraph *graph;
    QUuid uuid;
    
    QUuid nodeIdA;
    QUuid nodeIdB;
    PortID portIdA;
    PortID portIdB;
};
