#include "HelperTypes.hpp"

PortID::PortID(PortDirection direction, int index) : direction(direction), index(index) {}
PortID::PortID() : direction(PortDirection::IN), index(0) {}

bool PortID::operator ==(const PortID &other) const
{
    return direction == other.direction && index == other.index;
}
bool PortID::operator !=(const PortID &other) const
{
    return direction != other.direction || index != other.index;
}
