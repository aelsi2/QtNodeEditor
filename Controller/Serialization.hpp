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

QJsonObject serializeNode(NodeGraph& graph, QUuid nodeId, QMap<QUuid, QUuid> *uuidMap = nullptr);
QJsonObject serializeConnection(NodeGraph& graph, QUuid connectionId, QMap<QUuid, QUuid> *nodeUuidMap = nullptr);

void restoreNode(NodeGraph& graph, const QJsonObject &json);
void restoreConnection(NodeGraph& graph, const QJsonObject &json);
