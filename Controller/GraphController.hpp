#pragma once

#include <QUndoStack>
#include <QClipboard>
#include <QUuid>
#include <QPointF>
#include <QMap>

#include "Core/NodeGraph.hpp"
#include "NodeGraphUndoCommands.hpp"
#include "Core/PortUtils.hpp"
#include "Core/NodeType.hpp"

class GraphController
{
public:
    GraphController(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard);
    
    void selectNode(QUuid nodeId);
    void deselectNode(QUuid nodeId);
    void clearSelection();
    
    void createNode(NodeType type, QPointF position);
    void deleteNode(QUuid nodeId);
    void deleteSelection();
    
    void cutSelectionToClipboard();
    void copySelectionToClipboard();
    void pasteClipboard(QPointF position);
    
    bool connectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB);
    void performConnectAction(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB);
    
    QJsonObject serializeGraph();
    void deserializeGraph(QJsonObject json);
    
private:
    NodeGraph *graph;
    QUndoStack *undoStack;
    QClipboard *clipboard;
    QMap<QUuid, Node*> selection;
};

