#pragma once

#include <QMap>
#include <QUuid>
#include <QJsonObject>
#include <QJsonValue>

#include "Core/Node.hpp"
#include "Core/Connection.hpp"
#include "Core/NodeGraph.hpp"

QJsonObject serializeNode(NodeGraph& graph, QUuid nodeId, QMap<QUuid, QUuid> &uuidMap);
QJsonObject serializeConnection(NodeGraph& graph, QUuid connectionId, QMap<QUuid, QUuid> *nodeUuidMap);

void restoreNode(NodeGraph& graph, const QJsonObject &json);
void restoreConnection(NodeGraph& graph, const QJsonObject &json);
