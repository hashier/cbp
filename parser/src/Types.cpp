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

TypeStruct::TypeStruct(std::map<std::string, StructVariable*>* members) : members(members) {
    int offset = 0;
    for (std::map<std::string, StructVariable*>::const_iterator it = members->begin(); it != members->end(); it++) {
        offset += (*it).second->setStackOffset(offset, false);
    }

    // Not sure how this behavs with fixed offset..
    size = offset;
}

TypeVoid* TypeVoid::singleton;
