#include "SymbolTable.h"


SymbolTable::SymbolTable()
{
	scopeStack.push(Scope());
}

void SymbolTable::InsertNewScope()
{
	scopeStack.push(Scope());
}

void SymbolTable::DestroyCurrentScope()
{
	scopeStack.pop();
}

Definition &SymbolTable::GetDefinition(const std::string &identifier)
{
	// TODO Stack abarbeiten, nicht nur oberes angucken
	return scopeStack.top().GetDefinition(identifier);
}

void SymbolTable::InsertDefinition(const std::string &identifier, const Definition &definition)
{
	scopeStack.top().InsertNewDefinition(identifier, definition);
}
