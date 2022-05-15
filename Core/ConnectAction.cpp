#include "ConnectAction.hpp"

const ConnectAction ConnectAction::Connect{true, false, QUuid()};
const ConnectAction ConnectAction::Nothing{false, false, QUuid()};

ConnectAction ConnectAction::Disconnect(QUuid connectionToDelete)
{
    return ConnectAction{false, true, connectionToDelete};
}

ConnectAction ConnectAction::Reconnect(QUuid connectionToDelete)
{
    return ConnectAction{true, true, connectionToDelete};
}

bool ConnectAction::operator==(const ConnectAction other) const
{
    if (createNew != other.createNew) return false;
    if (disconnectExisting != other.disconnectExisting) return false;
    if (disconnectExisting == true)
    {
        return connectionToDelete == other.connectionToDelete;
    }
    return true;
}

bool ConnectAction::operator!=(const ConnectAction other) const
{
    return !(*this == other);
}
