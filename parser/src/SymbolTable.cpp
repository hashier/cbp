#include "SymbolTable.h"
#include "Definition.h"

namespace SymbolTable
{

SymbolTable::SymbolTable()
{
    scopes.push_back(Scope());
}

void SymbolTable::EnterNewScope()
{
    scopes.push_back(Scope());
}

void SymbolTable::LeaveCurrentScope()
{
    scopes.pop_back();
}

Definition *SymbolTable::GetDefinition(const std::string &identifier)
{
    Definition *result = NULL;
    for (std::list<Scope>::iterator it = scopes.begin(); it != scopes.end(); it++)
    {
        result = (*it).GetDefinition(identifier);
        if (result != NULL)
        {
            return result;
        }
    }

    throw DefinitionNotFoundException();
    return NULL;
}

void SymbolTable::InsertDefinition(Definition *definition)
{
    scopes.back().InsertNewDefinition(definition->getIdentifier(), definition);
}

}
