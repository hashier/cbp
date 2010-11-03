#include "Scope.h"

void Scope::InsertNewDefinition(const std::string &identifier, const Definition &definition)
{
	definitions.insert(std::pair<std::string, Definition>(identifier, definition));
}


Definition &Scope::GetDefinition(const std::string &identifier) 
{
	return definitions.find(identifier)->second;
}
