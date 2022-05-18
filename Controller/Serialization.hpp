#pragma once

#include <QMap>
#include <QUuid>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "Core/Node.hpp"
#include "Core/Connection.hpp"
#include "Core/NodeGraph.hpp"

class SerializedGraph
{
public:
    struct NodeData
    {
        QUuid uuid;
        NodeType type;
        QPointF pos;
        QJsonValue data;
    };
    struct ConnectionData
    {
        QUuid uuid;
        QUuid nodeIdA;
        QUuid nodeIdB;
        PortID portIdA;
        PortID portIdB;
    };
    
    SerializedGraph();
    SerializedGraph(QJsonDocument const &document);
    //SerializedGraph(NodeGraph const &nodeGraph);
    
    int nodeCount();
    int connectionCount();
    
    NodeData getNode(int index, bool newId = false, QMap<QUuid, QUuid> *uuidMap = nullptr);
    ConnectionData getConnection(int index, bool newId = false, QMap<QUuid, QUuid> const *nodeUuidLookup = nullptr);
    
    void addNode(NodeGraph const &graph, QUuid nodeId);
    void addConnection(NodeGraph const &graph, QUuid connectionId);
    
    QJsonDocument jsonDocument() const;
    
private:
    
    NodeData deserializeNode(QJsonObject const &json, bool newId = false, QMap<QUuid, QUuid> *uuidMap = nullptr) const;
    ConnectionData deserializeConnection(QJsonObject const &json, bool newId = false, QMap<QUuid, QUuid> const *nodeUuidLookup = nullptr) const;
    
    QJsonArray serializedNodes;
    QJsonArray serializedConnections;
};
