#pragma once

#include <QJsonValue>
#include <QUndoStack>

#include "UndoCommands.hpp"
#include "Core/Node.hpp"

class NodeChangeContext
{
public:
    NodeChangeContext(NodeGraph * graph, QUndoStack * stack, QUuid nodeId);
    ~NodeChangeContext();
    
    Node * getNode() const;
private:
    QUndoStack * undoStack;
    NodeGraph * graph;
    QUuid nodeId;
    QJsonValue oldState;
};

