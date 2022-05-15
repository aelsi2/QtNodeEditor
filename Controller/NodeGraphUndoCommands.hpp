#pragma once

#include <QUndoCommand>
#include <QUuid>
#include <QPointF>

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
    NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF nodePosition, QUuid nodeUuid);
    void undo() override;
    void redo() override;
private:
    NodeType type;
    QPointF position;
    QUuid uuid;
};

class NodeDeleteUndoCommand : public NodeGraphUndoCommand
{
public:
    void undo() override;
    void redo() override;
private:
    
};

class NodeMoveUndoCommand : public NodeGraphUndoCommand
{
public:
    NodeMoveUndoCommand(NodeGraph *graph, QUuid nodeUuid, QPointF oldPosition, QPointF newPosition);
    void undo() override;
    void redo() override;
private:
    QPointF oldPos;
    QPointF newPos;
    QUuid uuid;
};

class ConnectUndoCommand : public NodeGraphUndoCommand
{
public:
    void undo() override;
    void redo() override;
private:
    
};

class PasteUndoCommand : public NodeGraphUndoCommand
{
public:
    void undo() override;
    void redo() override;
private:
    
};
