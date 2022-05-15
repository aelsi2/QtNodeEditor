#pragma once

#include <QUuid>

struct ConnectAction{
    struct Pair;
    bool createNew;
    bool disconnectExisting;
    QUuid connectionToDelete;
    static const ConnectAction Connect;
    static const ConnectAction Nothing;
    static ConnectAction Disconnect(QUuid connectionToDelete);
    static ConnectAction Reconnect(QUuid connectionToDelete);
    
    bool operator==(const ConnectAction other) const;
    bool operator!=(const ConnectAction other) const;
};

struct ConnectAction::Pair
{
    ConnectAction first;
    ConnectAction second;
};
