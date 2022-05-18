#pragma once

#include <QUndoStack>
#include <QClipboard>
#include <QUuid>
#include <QPointF>
#include <QSet>
#include <QMap>

#include "Core/NodeGraph.hpp"
#include "UndoCommands.hpp"
#include "Core/HelperTypes.hpp"
#include "Core/ConnectAction.hpp"
#include "Serialization.hpp"

class GraphEditor
{
public:
    GraphEditor(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard);
    
    void selectNode(QUuid nodeId);
    void deselectNode(QUuid nodeId);
    void clearSelection();
    
    QUuid createNode(NodeType type, QPointF position = QPointF(0, 0));
    void deleteSelection();
    
    void cutSelection();
    void copySelection();
    void pasteClipboard();
    
    bool connectable(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB) const;
    void performConnectAction(QUuid nodeIdA, PortID portIdA, QUuid nodeIdB, PortID portIdB);
    
private:
    
    void getConnectionsBetween(QSet<QUuid> const &nodes, QSet<QUuid> &connections);
    
    NodeGraph *graph;
    QUndoStack *undoStack;
    QClipboard *clipboard;
    QSet<QUuid> selection;
};

