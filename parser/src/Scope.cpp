#include "Scope.h"
#include "Definition.h"

namespace SymbolTable
{

void Scope::InsertNewDefinition(const std::string &identifier, Definition *definition)
{
	std::pair<std::map<std::string,Definition *>::iterator,bool> insertTest;
	insertTest = definitions.insert(std::pair<std::string, Definition *>(identifier, definition));

	if (insertTest.second == false)
	{
		throw new DefinitionAlreadyExistsException();
	}
}

Definition *Scope::GetDefinition(const std::string &identifier) 
{
    std::map<std::string, Definition *>::iterator it;
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
