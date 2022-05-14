#pragma once

#include <QJsonObject>

template <typename Context>
class JSONSerializable
{
    virtual ~JSONSerializable() = default;
    virtual QJsonObject Serialize() = 0;
    virtual void Deserialize(QJsonObject, Context context) = 0;
};
