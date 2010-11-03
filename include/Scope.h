#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include "Definition.h"
#include<map>

class Scope
{
public:
	Definition &GetDefinition(const std::string &identifier);

private:
	std::map<Definition> definitions;
}

#endif // SCOPE_H_INCLUDED
