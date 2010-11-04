#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED

#include<stack>
#include "Scope.h"

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();
	void InsertNewScope();
	void DestroyCurrentScope();

	Definition &GetDefinition(const std::string &identifier);
	void InsertDefinition(const std::string &identifier, const Definition &definition);

private:
	std::stack<Scope> scopeStack;
};

#endif // SYMBOLTABLE_H_INCLUDED
