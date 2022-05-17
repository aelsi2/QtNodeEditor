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

QJsonObject serializeNode(NodeGraph const &graph, QUuid nodeId);
QJsonObject serializeConnection(NodeGraph const &graph, QUuid connection);

QUuid restoreNode(NodeGraph& graph, QJsonObject const &json, bool newId = false, QMap<QUuid, QUuid> *uuidMap = nullptr);
QUuid restoreConnection(NodeGraph& graph, QJsonObject const &json, bool newId = false, QMap<QUuid, QUuid> const *nodeUuidLookup = nullptr);

void restoreSubgraph(NodeGraph& graph, QJsonObject const &json, QSet<QUuid> *nodes = nullptr, QSet<QUuid> *connections = nullptr);
