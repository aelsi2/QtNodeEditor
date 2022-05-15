#include "GraphController.hpp"

GraphController::GraphController(NodeGraph *nodeGraph, QUndoStack *undoStack, QClipboard *clipboard)
    : graph(nodeGraph), undoStack(undoStack), clipboard(clipboard) {}

void GraphController::createNode(NodeType type, QPointF position)
{
    
}
