#include "SymbolTable.h"
#include "AbstractNodes.h"

namespace SymbolTable
{

SymbolTable::SymbolTable()
{
    scopes.push_back(Scope());
}

void SymbolTable::enterNewScope()
{
    scopes.push_back(Scope());
}

void SymbolTable::leaveCurrentScope()
{
    scopes.pop_back();
}

Declaration *SymbolTable::getDefinition(const std::string &identifier)
{
    Declaration *result = NULL;
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

void SymbolTable::insertDefinition(Declaration *definition)
{
    scopes.back().InsertNewDefinition(definition->getIdentifier(), definition);
}

void SymbolTable::insertGlobalDefinition(Declaration *definition)
{
    scopes.front().InsertNewDefinition(definition->getIdentifier(), definition);
}

}
