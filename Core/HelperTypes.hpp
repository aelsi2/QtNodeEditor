#pragma once

enum class PortDirection
{
    IN, OUT
};

typedef int PortDataType;

struct PortID
{
    PortID(PortDirection direction, int index);
    PortID();
    
    PortDirection direction;
    int index;
    
    bool operator ==(const PortID &other) const;
    bool operator !=(const PortID &other) const;
};

typedef int NodeType;
