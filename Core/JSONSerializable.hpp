#pragma once

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class JSONSerializable
{
    virtual QJsonObject serialize() const = 0;
    virtual void deserialize(QJsonObject &json)  = 0;
};
