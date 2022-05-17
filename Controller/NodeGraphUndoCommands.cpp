#include "NodeGraphUndoCommands.hpp"

NodeGraphUndoCommand::NodeGraphUndoCommand(NodeGraph *nodeGraph) : graph(nodeGraph) {}

NodeCreateUndoCommand::NodeCreateUndoCommand(NodeGraph *graph, NodeType nodeType, QPointF nodePosition, QUuid nodeUuid)
    : NodeGraphUndoCommand(graph), type(nodeType), position(nodePosition), uuid(nodeUuid) {}

void NodeCreateUndoCommand::undo()
{
    graph->deleteNode(uuid);
}

void NodeCreateUndoCommand::redo()
{
    graph->createNode(type, position, uuid);
}

NodeMoveUndoCommand::NodeMoveUndoCommand(NodeGraph *graph, QUuid nodeUuid, QPointF oldPosition, QPointF newPosition)
    : NodeGraphUndoCommand(graph), oldPos(oldPosition), newPos(newPosition), uuid(nodeUuid) {}

void NodeMoveUndoCommand::undo()
{
    graph->moveNode(uuid, oldPos);
}

void NodeMoveUndoCommand::redo()
{
    graph->moveNode(uuid, newPos);
}
