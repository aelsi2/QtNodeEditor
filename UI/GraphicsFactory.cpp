#include "GraphicsFactory.hpp"

GraphicsFactoryDelegate::GraphicsFactoryDelegate(std::function<NodeGraphicsItem*(
                                                     NodeType type,
                                                     QPointF position,
                                                     Node *node,
                                                     QUuid uuid)> function)
    : function(function){}
    
NodeGraphicsItem* GraphicsFactoryDelegate::createNode(NodeType type, QPointF position, Node *node, QUuid uuid) const
{
    return function(type, position, node, uuid);
}

void GraphicsFactory::addNodeDelegate(NodeType type, GraphicsFactoryDelegate *delegate)
{
    delegates.insert(type, delegate);
}

void GraphicsFactory::addNodeDelegate(NodeType type, std::function<NodeGraphicsItem*(NodeType type,
                                                                     QPointF position,
                                                                     Node *node,
                                                                     QUuid uuid)> function)
{
    GraphicsFactoryDelegate *delegate = new GraphicsFactoryDelegate(function);
    addNodeDelegate(type, delegate);
}

NodeGraphicsItem* GraphicsFactory::createNode(NodeType type, QPointF position, Node *node, QUuid uuid)
{
    auto delegate = delegates.value(type);
    if (delegate == nullptr) return nullptr;
    return delegate->createNode(type, position, node, uuid);
}

ConnectionGraphicsItem* GraphicsFactory::createConnection(QUuid uuid,
                                                          NodeGraphicsItem *firstNode,
                                                          NodeGraphicsItem *secondNode,
                                                          PortID firstPort,
                                                          PortID secondPort)
{
    return new ConnectionGraphicsItem(uuid, firstNode, secondNode, firstPort, secondPort);
}
