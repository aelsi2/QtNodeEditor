#pragma once

#include <QUuid>

struct ConnectionAction{
    bool createNew;
    bool disconnectExisting;
    QUuid connectionToDelete;
    //std::optional<QVector<QUuid>> connectionsToDelete;
    static const ConnectionAction Connect;
    static const ConnectionAction Nothing;
    static ConnectionAction Disconnect(QUuid connectionToDelete);
    static ConnectionAction Reconnect(QUuid connectionToDelete);
    
    bool operator==(const ConnectionAction other) const;
    bool operator!=(const ConnectionAction other) const;
};

