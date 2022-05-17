#pragma once

#include <QUndoCommand>
#include <QUuid>
#include <QPointF>

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
    NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF nodePosition, QUuid nodeUuid);
    void undo() override;
    void redo() override;
private:
    NodeType type;
    QPointF position;
    QUuid uuid;
};

class PasteDeleteUndoCommand : public NodeGraphUndoCommand
{
public:
    
    enum class Mode
    {
        Delete, Paste
    };

    PasteDeleteUndoCommand(NodeGraph *graph, QJsonObject const &json, Mode mode = Mode::Delete);
    PasteDeleteUndoCommand(NodeGraph *graph, QJsonObject const &json, QSet<QUuid> const &nodes, QSet<QUuid> const &connections, Mode mode = Mode::Delete);
    void undo() override;
    void redo() override;
    
    QSet<QUuid> nodes;
    QSet<QUuid> connections;
    
private:
    void deleteSubgraph();
    void pasteSubgraph();
    
    Mode mode = Mode::Delete;
    QJsonObject serializedSubgraph;
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
    //ConnectUndoCommand();
    void undo() override;
    void redo() override;
private:

};

