#pragma once

#include <QHash>

enum class PortDirection : int
{
    IN, OUT, INOUT
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

inline uint qHash(const PortID &key, uint seed)
{
    return qHash(static_cast<int const>(key.direction), seed) ^ qHash(key.index, seed);
}

typedef int NodeType;
