#include "Types.h"
#include "ProgramNodes.h"

std::string TypeStruct::getString() const
{
    std::string result = "Struct of (";
    for (std::map<std::string, StructVariable*>::const_iterator it = members->begin(); it != members->end(); it++)
    {
        if (it != members->begin())
        {
            result.append(", ");
        }
        result.append((*it).second->getType()->getString());

    }
    result.append(")");
    return result;
}

int TypeStruct::getSize()
{
    // TODO: structs mit festen offsets beachten(sowas wie hoehster offset + size)
    int sum = 0;
    for (std::map<std::string, StructVariable*>::const_iterator it = members->begin(); it != members->end(); it++)
    {
        sum += (*it).second->getSize();
    }

    return sum;
}

TypeVoid* TypeVoid::singleton;
