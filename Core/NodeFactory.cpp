#include "NodeFactory.hpp"

NodeFactoryDelegate::NodeFactoryDelegate(std::function<Node*(NodeType,NodeGraph*,QPointF,QUuid)> function) : function(function){}

Node* NodeFactoryDelegate::createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid) const
{
    return function(type, graph, position, uuid);
}

void NodeFactory::addDelegate(NodeType type, NodeFactoryDelegate *delegate)
{
    delegates.insert(type, delegate);
}

void NodeFactory::addDelegate(NodeType type, std::function<Node*(NodeType,NodeGraph*,QPointF,QUuid)> function)
{
    NodeFactoryDelegate *delegate = new NodeFactoryDelegate(function);
    delegates.insert(type, delegate);
}

Node* NodeFactory::createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid)
{
    if (!delegates.contains(type)) return nullptr;
    return delegates.value(type)->createNode(type, graph, position, uuid);
}
