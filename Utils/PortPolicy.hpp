#pragma once

#include <QVector>

#include "Core/PortUtils.hpp"

class PortPolicy
{
public:
    PortPolicy(int inCapacity, int outCapacity, QVector<PortDataType> types);
    PortPolicy(int capacity, QVector<PortDataType> types);
    
    int getInputCapacity() const;
    int getOuputCapacity() const;
    int getOutputType() const;
    bool supportsType(PortDataType type) const;

private:
    int inputCapacity;
    int outputCapacity;
    QVector<PortDataType> supportedTypes;
    PortDataType outputType;
};
