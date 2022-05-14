#include "ConnectionAction.hpp"

const ConnectionAction ConnectionAction::Connect{true, false, QUuid()};
const ConnectionAction ConnectionAction::Nothing{false, false, QUuid()};

ConnectionAction ConnectionAction::Disconnect(QUuid connectionToDelete)
{
    return ConnectionAction{false, true, connectionToDelete};
}

ConnectionAction ConnectionAction::Reconnect(QUuid connectionToDelete)
{
    return ConnectionAction{true, true, connectionToDelete};
}

bool ConnectionAction::operator==(const ConnectionAction other) const
{
    if (createNew != other.createNew) return false;
    if (disconnectExisting != other.disconnectExisting) return false;
    if (disconnectExisting == true)
    {
        return connectionToDelete == other.connectionToDelete;
    }
    return true;
}

bool ConnectionAction::operator!=(const ConnectionAction other) const
{
    return !(*this == other);
}
