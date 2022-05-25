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
    NodeGraphUndoCommand(NodeGraph *nodeGraph);
protected:
    NodeGraph *graph;
};

class NodeCreateUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF position, QJsonValue const &internalData, QUuid uuid  = QUuid::createUuid());
    NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF position, QUuid uuid  = QUuid::createUuid());
    void undo() override;
    void redo() override;
    
private:
    NodeType type;
    QUuid uuid;
    QPointF pos;
    std::optional<QJsonValue> data;
};

class NodeDeleteUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeDeleteUndoCommand(NodeGraph *graph, QUuid uuid, NodeType nodeType, QPointF position, QJsonValue const &internalData);
    NodeDeleteUndoCommand(NodeGraph *graph, QUuid uuid);
    void undo() override;
    void redo() override;
private:
    NodeType type;
    QUuid uuid;
    QPointF pos;
    QJsonValue data;
};

class ConnectUndoCommand : public NodeGraphUndoCommand
{
public:
    ConnectUndoCommand(NodeGraph *graph, QUuid nodeIdA, QUuid nodeIdB, PortID portIdA, PortID portIdB, QUuid uuid  = QUuid::createUuid(), bool disconnect = false);
    ConnectUndoCommand(NodeGraph *graph, QUuid uuid);
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

class NodeMoveUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeMoveUndoCommand(NodeGraph *graph, QUuid nodeId, QPointF oldPos, QPointF newPos, bool relative = false);
    NodeMoveUndoCommand(NodeGraph *graph, QUuid nodeId, QPointF newPos, bool relative = true);
    void undo() override;
    void redo() override;
private:
    QUuid uuid;
    QPointF oldPos;
    QPointF newPos;
};
