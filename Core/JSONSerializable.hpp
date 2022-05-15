#pragma once

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class JSONSerializable
{
    virtual void serialize(QJsonObject &json) const = 0;
    virtual void deserialize(QJsonObject &json)  = 0;
};
