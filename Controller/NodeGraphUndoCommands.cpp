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
    graph->moveNodeTo(uuid, oldPos);
}

void NodeMoveUndoCommand::redo()
{
    graph->moveNodeTo(uuid, newPos);
}

PasteDeleteUndoCommand::PasteDeleteUndoCommand(NodeGraph *graph, QJsonObject const &json, Mode mode)
    : NodeGraphUndoCommand(graph),
    mode(mode),
    serializedSubgraph(json) {}

PasteDeleteUndoCommand::PasteDeleteUndoCommand(NodeGraph *graph, QJsonObject const &json, QSet<QUuid> const &nodes, QSet<QUuid> const &connections, Mode mode)
    : NodeGraphUndoCommand(graph),
    nodes(nodes),
    connections(connections),
    mode(mode),
    serializedSubgraph(json) {}

void PasteDeleteUndoCommand::undo()
{
    if (mode == Mode::Delete) pasteSubgraph();
    else deleteSubgraph();
}

void PasteDeleteUndoCommand::redo()
{
    if (mode == Mode::Delete) deleteSubgraph();
    else pasteSubgraph();
}

void PasteDeleteUndoCommand::deleteSubgraph()
{
    for (auto i = connections.cbegin(); i != connections.cend(); ++i)
    {
        graph->disconnect(*i);
    }
    for (auto i = nodes.cbegin(); i != nodes.cend(); ++i)
    {
        graph->deleteNode(*i);
    }
    connections.clear();
    nodes.clear();
}

void PasteDeleteUndoCommand::pasteSubgraph()
{
    restoreSubgraph(*graph, serializedSubgraph, &nodes, &connections);
}
