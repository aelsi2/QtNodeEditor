#pragma once

enum class PortDirection
{
    IN, OUT
};

typedef int PortDataType;

struct PortID
{
    PortID(PortDirection direction, int index) : direction(direction), index(index) {}
    PortDirection direction;
    int index;
    
    bool operator ==(const PortID &other) const{
        return direction == other.direction && index == other.index;
    }
    bool operator !=(const PortID &other) const{
        return direction != other.direction || index != other.index;
    }
};
