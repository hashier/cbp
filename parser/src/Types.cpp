#include "Types.h"

#include "Variables.h"
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
    int maxSize = 0;
    for (std::map<std::string, StructVariable*>::const_iterator it = members->begin(); it != members->end(); it++) {
        offset += (*it).second->setStackOffset(offset, false);
        if (maxSize < (*it).second->getExplicitOffset() + (*it).second->getSize())
        {
            maxSize = (*it).second->getExplicitOffset() + (*it).second->getSize();
        }
    }

    // Not sure how this behavs with fixed offset..
    size = offset;
    if (size < maxSize)
        size = maxSize;
}

TypeStruct::TypeStruct(TypeStruct *type)
{
    members = new std::map<std::string, StructVariable *>();

    int offset = 0;
    int maxSize = 0;
    for (std::map<std::string, StructVariable*>::iterator it = type->members->begin(); it != type->members->end(); it++) {
        StructVariable *newVar = static_cast<StructVariable *>((*it).second->clone());

        offset += newVar->setStackOffset(offset, false);
        if (maxSize < newVar->getExplicitOffset() + newVar->getSize())
        {
            maxSize = newVar->getExplicitOffset() + newVar->getSize();
        }
        members->insert(std::make_pair((*it).first, newVar));
    }

    // Not sure how this behavs with fixed offset..
    size = offset;
    if (size < maxSize)
        size = maxSize;
}

TypeVoid* TypeVoid::singleton;
