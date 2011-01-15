#include "SymbolTable.h"
#include "AbstractNodes.h"

namespace SymbolTable
{

SymbolTable::SymbolTable()
{
    scopes.push_back(Scope());
}

SymbolTable::~SymbolTable()
{
    // Ensure balanced scope enters/leaves
    assert(scopes.size() == 1);
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
        result = (*it).getDefinition(identifier);
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
    scopes.back().insertNewDefinition(definition->getIdentifier(), definition);
}

void SymbolTable::insertGlobalDefinition(Declaration *definition)
{
    scopes.front().insertNewDefinition(definition->getIdentifier(), definition);
}

}
