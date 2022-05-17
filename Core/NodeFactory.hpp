#pragma once

#include <QMap>
#include <QUuid>

#include "Core/Node.hpp"
#include "Core/NodeType.hpp"
#include "Core/NodeGraph.hpp"

class Node;
class NodeGraph;

class NodeFactoryDelegate
{
public:
    virtual Node* createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid) const = 0;
};

class AbstractNodeDelegate : public NodeFactoryDelegate
{
    Node* createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid) const override;
};

class NodeFactory
{
public:
    void addDelegate(NodeType type, NodeFactoryDelegate *delegate);
    Node* createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid);
    ~NodeFactory();
private:
    QMap<NodeType, NodeFactoryDelegate*> delegates;
};

