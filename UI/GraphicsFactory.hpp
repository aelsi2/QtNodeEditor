#pragma once

class NodeGraphicsItem;

#include "Common/HelperTypes.hpp"
#include "Core/Node.hpp"
#include "UI/NodeGraphicsItem.hpp"
#include "UI/ConnectionGraphicsItem.hpp"

class GraphicsFactoryDelegate
{
public:
    GraphicsFactoryDelegate(std::function<NodeGraphicsItem*(NodeType type,
                                                             QPointF position,
                                                             Node *node,
                                                             QUuid uuid)> function);
    NodeGraphicsItem* createNode(NodeType type, QPointF position, Node *node, QUuid uuid) const;
private:
    std::function<NodeGraphicsItem*(NodeType type,
                                     QPointF position,
                                     Node *node,
                                     QUuid uuid)> function;
};

class GraphicsFactory
{
public:
    void addNodeDelegate(NodeType type, GraphicsFactoryDelegate *delegate);
    void addNodeDelegate(NodeType type, std::function<NodeGraphicsItem*(NodeType type,
                                                                         QPointF position,
                                                                         Node *node,
                                                                         QUuid uuid)> function);
    NodeGraphicsItem* createNode(NodeType type, QPointF position, Node *node, QUuid uuid);
    ConnectionGraphicsItem* createConnection(QUuid uuid,
                                             NodeGraphicsItem *firstNode, NodeGraphicsItem *secondNode,
                                             PortID firstPort, PortID secondPort);
    ~GraphicsFactory();
private:
    QMap<NodeType, GraphicsFactoryDelegate*> delegates;
};

