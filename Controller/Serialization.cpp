#include "Serialization.hpp"

QJsonObject serializeNode(NodeGraph& graph, QUuid nodeId, QMap<QUuid, QUuid> *uuidMap)
{
    QJsonObject json;
    Node *node = graph.getNode(nodeId);
    QUuid uuid = node->getUuid();
    QJsonObject jsonPos;
    
    if (uuidMap != nullptr)
    {
        uuid = QUuid::createUuid();
        uuidMap->insert(node->getUuid(), uuid);
    }
    
    jsonPos["x"] = node->getPosition().x();
    jsonPos["y"] = node->getPosition().y();
    json["position"] = jsonPos;
    json["uuid"] = uuid.toString();
    json["type"] = node->type;
    
    return json;
}

QJsonObject serializeConnection(NodeGraph& graph, QUuid connectionId, QMap<QUuid, QUuid> *nodeUuidMap)
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
    
    if (nodeUuidMap != nullptr)
    {
        nodeIdA = nodeUuidMap->value(nodeIdA, nodeIdA);
        nodeIdB = nodeUuidMap->value(nodeIdB, nodeIdB);
    }
    
    json["nodeIdA"] = nodeIdA.toString();
    json["nodeIdB"] = nodeIdB.toString();
    json["portIdA"] = portAJson;
    json["portIdB"] = portBJson;
    
    return json;
}

void restoreNode(NodeGraph& graph, const QJsonObject &json)
{
    QUuid uuid = QUuid::createUuid();
    QPointF pos = QPointF(0, 0);
    NodeType nodeType = NodeType();
    QJsonValue data = QJsonObject();
    
    if (json["uuid"].isString())
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
    
    Node *node = graph.getNode(uuid);
    if (node != nullptr) return;
    
    graph.createNode(nodeType, pos, uuid);
    graph.getNode(uuid)->restoreData(data);
    
}

void restoreConnection(NodeGraph& graph, const QJsonObject &json)
{
    QJsonValue jsonNodeIdA = json["nodeIdA"];
    QJsonValue jsonNodeIdB = json["nodeIdB"];
    QJsonValue jsonPortA = json["portIdA"];
    QJsonValue jsonPortB = json["portIdB"];
    
    if (!jsonNodeIdA.isString() || !jsonNodeIdB.isString()) return;
    if (!jsonPortA.isObject() || !jsonPortB.isObject()) return;
    
    QJsonValue jsonDirectionA = jsonPortA["direction"];
    QJsonValue jsonDirectionB = jsonPortA["direction"];
    QJsonValue jsonIndexA = jsonPortA["index"];
    QJsonValue jsonIndexB = jsonPortA["index"];
    
    if (!jsonIndexA.isDouble() || !jsonIndexB.isDouble()) return;
    if (!jsonDirectionA.isDouble() || !jsonDirectionB.isDouble()) return;
    
    QUuid nodeIdA = QUuid(jsonNodeIdA.toString());
    QUuid nodeIdB = QUuid(jsonNodeIdB.toString());
    PortID portIdA = PortID(jsonDirectionA.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexA.toInt());
    PortID portIdB = PortID(jsonDirectionB.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexB.toInt());
    
    graph.connect(nodeIdA, nodeIdB, portIdA, portIdB);
}
