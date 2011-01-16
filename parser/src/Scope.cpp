#include "Scope.h"
#include "AbstractNodes.h"

namespace SymbolTable
{

void Scope::insertNewDefinition(const std::string &identifier, Declaration *definition)
{
    std::pair<std::map<std::string,Declaration *>::iterator,bool> insertTest;
    insertTest = definitions.insert(std::pair<std::string, Declaration *>(identifier, definition));

    if (insertTest.second == false)
    {
        throw DefinitionAlreadyExistsException(identifier, getDefinition(identifier)->getLineDefined());
    }
}

Declaration *Scope::getDefinition(const std::string &identifier) 
{
    std::map<std::string, Declaration *>::iterator it;
    it = definitions.find(identifier);
    
    if (it == definitions.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

}
