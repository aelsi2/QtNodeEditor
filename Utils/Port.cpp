#include "Port.hpp"

Port::Port(PortDirection direction, PortPolicy &policy)
    : direction(direction), policy(policy) {}

int Port::getConnectionCount() const
{
    return connections.length();
}

Port* Port::getConnection(int index) const
{
    if (index >= connections.length()) return nullptr;
    return connections.at(index);
}
