#pragma once

#include <QMap>
#include <QUuid>

#include "Core/Node.hpp"
#include "Common/HelperTypes.hpp"
#include "Core/NodeGraph.hpp"

class Node;
class NodeGraph;

class NodeFactoryDelegate
{
public:
    NodeFactoryDelegate(std::function<Node*(NodeType,NodeGraph*,QPointF,QUuid)> function);
    Node* createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid) const;
private:
    std::function<Node*(NodeType,NodeGraph*,QPointF,QUuid)> function;
};

class NodeFactory
{
public:
    void addDelegate(NodeType type, NodeFactoryDelegate *delegate);
    void addDelegate(NodeType type, std::function<Node*(NodeType,NodeGraph*,QPointF,QUuid)> function);
    Node* createNode(NodeType type, NodeGraph *graph, QPointF position, QUuid uuid);
private:
    QMap<NodeType, NodeFactoryDelegate*> delegates;
};

