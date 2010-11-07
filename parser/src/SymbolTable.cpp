#include "SymbolTable.h"


SymbolTable::SymbolTable()
{
	scopeStack.push(Scope());
}

void SymbolTable::EnterNewScope()
{
	scopeStack.push(Scope());
}

void SymbolTable::LeaveCurrentScope()
{
	scopeStack.pop();
}

Definition &SymbolTable::GetDefinition(const std::string &identifier)
{
	return scopeStack.top().GetDefinition(identifier);
}

void SymbolTable::InsertDefinition(const std::string &identifier, const Definition &definition)
{
	scopeStack.top().InsertNewDefinition(identifier, definition);
}
