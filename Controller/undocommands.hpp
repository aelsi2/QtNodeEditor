#pragma once

#include <QUndoCommand>
#include <QUuid>
#include <QPointF>
#include <optional>

#include "Serialization.hpp"
#include "Core/NodeGraph.hpp"

class NodeGraphUndoCommand : public QUndoCommand
{
public:
    NodeGraphUndoCommand(NodeGraph &nodeGraph);
protected:
    NodeGraph &graph;
};

class NodeCreateUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeCreateUndoCommand(NodeGraph &graph, NodeType nodeType, QPointF position, QJsonValue const &internalState, QUuid uuid  = QUuid::createUuid());
    NodeCreateUndoCommand(NodeGraph &graph, NodeType nodeType, QPointF position, QUuid uuid  = QUuid::createUuid());
    void undo() override;
    void redo() override;
    
private:
    NodeType type;
    QUuid uuid;
    QPointF pos;
    std::optional<QJsonValue> state;
};

class NodeDeleteUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeDeleteUndoCommand(NodeGraph &graph, QUuid uuid, NodeType nodeType, QPointF position, QJsonValue const &state);
    NodeDeleteUndoCommand(NodeGraph &graph, QUuid uuid);
    void undo() override;
    void redo() override;
private:
    NodeType type;
    QUuid uuid;
    QPointF pos;
    QJsonValue state;
};

class ConnectUndoCommand : public NodeGraphUndoCommand
{
public:
    ConnectUndoCommand(NodeGraph &graph, QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB, QUuid uuid  = QUuid::createUuid(), bool disconnect = false);
    ConnectUndoCommand(NodeGraph &graph, QUuid uuid);
    void undo() override;
    void redo() override;
private:
    QUuid connectionId;
    QUuid nodeIdA;
    QUuid nodeIdB;
    PortID portIdA;
    PortID portIdB;
    bool disconnect;
};


