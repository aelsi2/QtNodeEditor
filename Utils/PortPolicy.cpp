#include "PortPolicy.hpp"

PortPolicy::PortPolicy(int inCapacity, int outCapacity, QVector<PortDataType> types) 
    : inputCapacity(inCapacity), outputCapacity(outCapacity), supportedTypes(types) {}
PortPolicy::PortPolicy(int capacity, QVector<PortDataType> types) 
    : inputCapacity(capacity), outputCapacity(capacity), supportedTypes(types) {}


int PortPolicy::getInputCapacity() const
{
    return inputCapacity;    
}

int PortPolicy::getOuputCapacity() const
{
    return outputCapacity;    
}

int PortPolicy::getOutputType() const
{
    return outputType;
}

bool PortPolicy::supportsType(PortDataType type) const
{
    for (int sType : supportedTypes)
    {
        if (sType == type) return true;
    }
    return false;
}
