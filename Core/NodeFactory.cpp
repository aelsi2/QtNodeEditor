#include "NodeFactory.hpp"

Node* AbstractNodeDelegate::createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid) const
{
    return new Node(graph, type, position, uuid);
}

void NodeFactory::addDelegate(NodeType type, NodeFactoryDelegate *delegate)
{
    delegates.insert(type, delegate);
}

Node* NodeFactory::createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid)
{
    if (!delegates.contains(type)) return nullptr;
    return delegates[type]->createNode(type, graph, position, uuid);
}

NodeFactory::~NodeFactory()
{
    qDeleteAll(delegates);
}
