#pragma once

#include <QUndoStack>
#include <QClipboard>
#include <QUuid>
#include <QPointF>
#include <QSet>
#include <QMap>

#include "Core/NodeGraph.hpp"
#include "NodeGraphUndoCommands.hpp"
#include "Core/PortUtils.hpp"
#include "Core/NodeType.hpp"
#include "Core/ConnectAction.hpp"
#include "Serialization.hpp"

class GraphController
{
public:
    GraphController(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard);
    
    void selectNode(QUuid nodeId);
    void deselectNode(QUuid nodeId);
    void clearSelection();
    
    QUuid createNode(NodeType type, QPointF position = QPointF(0, 0));
    void deleteSelection();
    
    void cutSelectionToClipboard();
    void copySelectionToClipboard();
    void pasteClipboard();
    
    bool connectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const;
    void performConnectAction(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB);
    
    QJsonObject serializeGraph();
    void deserializeGraph(QJsonObject json);
    
private:
    NodeGraph *graph;
    QUndoStack *undoStack;
    QClipboard *clipboard;
    QSet<QUuid> selection;
};

