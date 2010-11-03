#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include "Definition.h"
#include<map>
#include<string>

class Scope
{
public:
	Scope();
	~Scope();

	Definition &GetDefinition(const std::string &identifier);
	void InsertNewDefinition(const std::string &identifier, const Definition &definition);

private:
	std::map<std::string, Definition> definitions;
};

#endif // SCOPE_H_INCLUDED
