#pragma once

#include <QVector>

#include "Core/Node.hpp"
#include "Common/HelperTypes.hpp"
#include "PortPolicy.hpp"


class Node;

//Outdated! Needs to be rewritten.
class Port
{

public:
    Port(PortDirection direction, PortPolicy &policy);
    
    PortDataType getType() const;
    PortDirection getDirection() const;
    
    int getConnectionCount() const;
    Port* getConnection(int index) const;
    
    bool acceptsConnection(Port &other) const;
    bool makeConnection(Port &other);

protected:
    PortDirection direction;
    PortPolicy &policy;
    QVector<Port*> connections;
};
