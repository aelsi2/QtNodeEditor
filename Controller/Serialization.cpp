#include "Serialization.hpp"

QJsonObject serializeNode(NodeGraph const &graph, QUuid nodeId)
{
    QJsonObject json;
    Node *node = graph.getNode(nodeId);
    QUuid uuid = node->getUuid();
    QJsonObject jsonPos;
    
    jsonPos["x"] = node->getPosition().x();
    jsonPos["y"] = node->getPosition().y();
    json["position"] = jsonPos;
    json["uuid"] = uuid.toString();
    json["type"] = node->type;
    
    return json;
}

QJsonObject serializeConnection(NodeGraph const &graph, QUuid connectionId)
{
    Connection *connection = graph.getConnection(connectionId);
    if (connection == nullptr) return QJsonObject();
    
    QJsonObject json;
    QJsonObject portAJson;
    QJsonObject portBJson;
    portAJson["direction"] = (connection->getFirstPort().direction == PortDirection::IN) ? 0 : 1;
    portAJson["index"] = connection->getFirstPort().index;
    
    portBJson["direction"] = (connection->getSecondPort().direction == PortDirection::IN) ? 0 : 1;
    portBJson["index"] = connection->getSecondPort().index;
    
    QUuid nodeIdA = connection->getFirstNodeId();
    QUuid nodeIdB = connection->getSecondNodeId();
    
    json["uuid"] = connection->getUuid().toString();
    json["nodeIdA"] = nodeIdA.toString();
    json["nodeIdB"] = nodeIdB.toString();
    json["portIdA"] = portAJson;
    json["portIdB"] = portBJson;
    
    return json;
}

QUuid restoreNode(NodeGraph& graph, QJsonObject const &json, bool newId, QMap<QUuid, QUuid> *uuidMap)
{
    QUuid uuid;
    QPointF pos = QPointF(0, 0);
    NodeType nodeType = NodeType();
    QJsonValue data = QJsonObject();
    
    uuid = QUuid(json["uuid"].toString());
    
    
    if (json["type"].isDouble()) 
        nodeType = json["type"].toInt();
    QJsonValue jsonPos = json["position"];
    if (jsonPos.isObject())
    {
        QJsonValue jsonX = jsonPos["x"];
        QJsonValue jsonY = jsonPos["y"];
        if (jsonX.isDouble() && jsonY.isDouble())
        {
            pos.setX(jsonX.toDouble());
            pos.setY(jsonY.toDouble());
        }
    }
    if (!json["data"].isUndefined()) data = json["data"];
    
    
    
    if (uuid == QUuid()) uuid = QUuid::createUuid();
    else if (newId)
    {
        QUuid newUuid = QUuid::createUuid();
        if (uuidMap != nullptr) uuidMap->insert(uuid, newUuid);
        uuid = newUuid;
    }
    else
    {
        Node *node = graph.getNode(uuid);
        if (node != nullptr) return uuid;
    }
    
    graph.createNode(nodeType, pos, uuid);
    graph.getNode(uuid)->restoreData(data);
    return uuid;
}

QUuid restoreConnection(NodeGraph& graph, QJsonObject const &json, bool newId, QMap<QUuid, QUuid> const *nodeUuidMap)
{
    QJsonValue jsonUuid = json["uuid"];
    QJsonValue jsonNodeIdA = json["nodeIdA"];
    QJsonValue jsonNodeIdB = json["nodeIdB"];
    QJsonValue jsonPortA = json["portIdA"];
    QJsonValue jsonPortB = json["portIdB"];
    
    if (!jsonNodeIdA.isString() || !jsonNodeIdB.isString()) return QUuid();
    if (!jsonPortA.isObject() || !jsonPortB.isObject()) return QUuid();
    
    QJsonValue jsonDirectionA = jsonPortA["direction"];
    QJsonValue jsonDirectionB = jsonPortB["direction"];
    QJsonValue jsonIndexA = jsonPortA["index"];
    QJsonValue jsonIndexB = jsonPortB["index"];
    
    if (!jsonIndexA.isDouble() || !jsonIndexB.isDouble()) return QUuid();
    if (!jsonDirectionA.isDouble() || !jsonDirectionB.isDouble()) return QUuid();
    
    
    QUuid uuid  = QUuid(jsonUuid.toString());
    if (newId || uuid == QUuid()) uuid = QUuid::createUuid();
    
    QUuid nodeIdA = QUuid(jsonNodeIdA.toString());
    QUuid nodeIdB = QUuid(jsonNodeIdB.toString());
    
    if (nodeUuidMap != nullptr)
    {
        nodeIdA = nodeUuidMap->value(nodeIdA, nodeIdA);
        nodeIdB = nodeUuidMap->value(nodeIdB, nodeIdB);
    }
    
    PortID portIdA = PortID(jsonDirectionA.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexA.toInt());
    PortID portIdB = PortID(jsonDirectionB.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexB.toInt());
    
    return graph.connect(nodeIdA, nodeIdB, portIdA, portIdB, uuid);
}

void restoreSubgraph(NodeGraph& graph, QJsonObject const &json, QSet<QUuid> *nodes, QSet<QUuid> *connections)
{
    QJsonArray serializedNodes = json["nodes"].toArray();
    QJsonArray serializedConnections = json["connections"].toArray();
    QMap<QUuid, QUuid> nodeUuidRemappings;
    for (auto i = serializedNodes.begin(); i != serializedNodes.end(); ++i)
    {
        QUuid uuid = restoreNode(graph, i->toObject(), true, &nodeUuidRemappings);
        if (nodes != nullptr) nodes->insert(uuid);
    }
    for (auto i = serializedConnections.begin(); i != serializedConnections.end(); ++i)
    {
        QUuid uuid = restoreConnection(graph, i->toObject(), true, &nodeUuidRemappings);
        if (connections != nullptr) connections->insert(uuid);
    }
}
