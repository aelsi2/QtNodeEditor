#include "Serialization.hpp"

SerializedGraph::SerializedGraph() {}

SerializedGraph::SerializedGraph(QJsonDocument const &document)
{
    serializedNodes = document["nodes"].toArray();
    serializedConnections = document["connections"].toArray();
}

SerializedGraph::SerializedGraph(QString const & string)
{
    auto document = QJsonDocument::fromJson(string.toUtf8());
    serializedNodes = document["nodes"].toArray();
    serializedConnections = document["connections"].toArray();
}

void SerializedGraph::addNode(NodeGraph const &graph, QUuid nodeId)
{
    Node *node = graph.getNode(nodeId);
    if (node == nullptr) return;
    
    QJsonObject json;
    QJsonObject jsonPos;
    
    QJsonValue jsonCustomData;
    node->serializeData(jsonCustomData);
     
    jsonPos["x"] = node->getPosition().x();
    jsonPos["y"] = node->getPosition().y();
    json["position"] = jsonPos;
    json["uuid"] = nodeId.toString();
    json["type"] = node->type;
    json["data"] = jsonCustomData;
    serializedNodes.append(json);
}

void SerializedGraph::addConnection(NodeGraph const &graph, QUuid connectionId)
{
    Connection *connection = graph.getConnection(connectionId);
    if (connection == nullptr) return;
    
    QUuid nodeIdA = connection->getFirstNodeId();
    QUuid nodeIdB = connection->getSecondNodeId();
    PortID portIdA = connection->getFirstPort();
    PortID portIdB = connection->getSecondPort();
    
    QJsonObject json;
    QJsonObject portAJson;
    QJsonObject portBJson;
    
    portAJson["direction"] = (portIdA.direction == PortDirection::IN) ? 0 : 1;
    portAJson["index"] = portIdA.index;
    portBJson["direction"] = (portIdB.direction == PortDirection::IN) ? 0 : 1;
    portBJson["index"] = portIdB.index;
    json["uuid"] = connection->getUuid().toString();
    json["nodeIdA"] = nodeIdA.toString();
    json["nodeIdB"] = nodeIdB.toString();
    json["portIdA"] = portAJson;
    json["portIdB"] = portBJson;
    serializedConnections.append(json);
}

SerializedGraph::NodeData SerializedGraph::deserializeNode(QJsonObject const &json, bool newId, QMap<QUuid, QUuid> *uuidMap) const
{
    NodeData nd;
    
    nd.uuid = QUuid(json["uuid"].toString());
    if (json["type"].isDouble()) 
        nd.type = json["type"].toInt();
    QJsonValue jsonPos = json["position"];
    if (jsonPos.isObject())
    {
        QJsonValue jsonX = jsonPos["x"];
        QJsonValue jsonY = jsonPos["y"];
        if (jsonX.isDouble() && jsonY.isDouble())
        {
            nd.pos.setX(jsonX.toDouble());
            nd.pos.setY(jsonY.toDouble());
        }
    }
    if (!json["data"].isUndefined()) nd.data = json["data"];
    
    if (nd.uuid == QUuid())
    {
        nd.uuid = QUuid::createUuid();
    }
    else if (newId)
    {
        QUuid newId = QUuid::createUuid();
        if (uuidMap != nullptr) uuidMap->insert(nd.uuid, newId);
        nd.uuid = newId;
    }
    return nd;
}

SerializedGraph::ConnectionData SerializedGraph::deserializeConnection(const QJsonObject &json, bool newId, QMap<QUuid, QUuid> const *nodeUuidLookup) const
{
    ConnectionData cd;
    cd.uuid = QUuid(json["uuid"].toString());
    
    cd.nodeIdA = QUuid(json["nodeIdA"].toString());
    cd.nodeIdB = QUuid(json["nodeIdB"].toString());
    if (cd.nodeIdA == QUuid() || cd.nodeIdB == QUuid()) return ConnectionData();
    
    QJsonValue jsonDirectionA = json["portIdA"]["direction"];
    QJsonValue jsonDirectionB = json["portIdB"]["direction"];
    if (!jsonDirectionA.isDouble() || !jsonDirectionB.isDouble()) return ConnectionData();
    QJsonValue jsonIndexA = json["portIdA"]["index"];
    QJsonValue jsonIndexB = json["portIdB"]["index"]; 
    if (!jsonIndexA.isDouble() || !jsonIndexB.isDouble()) return ConnectionData();
    
    if (nodeUuidLookup != nullptr)
    {
        cd.nodeIdA = nodeUuidLookup->value(cd.nodeIdA, cd.nodeIdA);
        cd.nodeIdB = nodeUuidLookup->value(cd.nodeIdB, cd.nodeIdB);
    }
    cd.portIdA = PortID(jsonDirectionA.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexA.toInt());
    cd.portIdB = PortID(jsonDirectionB.toInt() == 0 ? PortDirection::IN : PortDirection::OUT, jsonIndexB.toInt());
    
    if (cd.uuid == QUuid())
    {
        cd.uuid = QUuid::createUuid();
    }
    else if (newId)
    {
        QUuid newId = QUuid::createUuid();
        cd.uuid = newId;
    }
    return cd;
}

int SerializedGraph::nodeCount() { return serializedNodes.count(); }
int SerializedGraph::connectionCount() { return serializedConnections.count(); }

SerializedGraph::NodeData SerializedGraph::getNode(int index, bool newId, QMap<QUuid, QUuid> *uuidMap)
{
    if (index >= nodeCount()) return NodeData();
    return deserializeNode(serializedNodes[index].toObject(), newId, uuidMap);
}

SerializedGraph::ConnectionData SerializedGraph::getConnection(int index, bool newId, QMap<QUuid, QUuid> const *nodeUuidLookup)
{
    if (index >= connectionCount()) return ConnectionData();
    return deserializeConnection(serializedConnections[index].toObject(), newId, nodeUuidLookup);
}

QJsonDocument SerializedGraph::jsonDocument() const
{
    QJsonObject json;
    json["nodes"] = serializedNodes;
    json["connections"] = serializedConnections;
    return QJsonDocument(json);
}

QString SerializedGraph::toString() const
{
    return jsonDocument().toJson(QJsonDocument::Indented);
}
