#include "Types.h"
#include "ProgramNodes.h"

std::string TypeStruct::getString() const
{
    std::string result = "Struct of (";
    for (std::list<Variable *>::const_iterator it = members.begin(); it != members.end(); it++)
    {
        if (it != members.begin())
        {
            result.append(", ");
        }
        result.append((*it)->getType()->getType()->getString());

    }
    result.append(")");
    return result;
}
