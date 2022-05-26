#include "NodeChangeContext.hpp"

NodeChangeContext::NodeChangeContext(NodeGraph * graph, QUndoStack * stack, QUuid nodeId)
    : undoStack(stack),
    graph(graph),
    nodeId(nodeId)
{
    Node * node = graph->getNode(nodeId);
    if (node == nullptr) return;
    node->serializeData(oldState);
    qDebug() << "Change started";
}

NodeChangeContext::~NodeChangeContext()
{
    Node * node = graph->getNode(nodeId);
    if (node == nullptr) return;
    QJsonValue newState;
    node->serializeData(newState);
    qDebug() << "Change ended";
    undoStack->push(new NodeChangeUndoCommand(graph, nodeId, oldState, newState));
}
