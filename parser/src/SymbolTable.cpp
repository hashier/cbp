#include "SymbolTable.h"
#include "Definition.h"

namespace SymbolTable
{

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

Definition *SymbolTable::GetDefinition(const std::string &identifier)
{
	return scopeStack.top().GetDefinition(identifier);
}

void SymbolTable::InsertDefinition(Definition *definition)
{
	scopeStack.top().InsertNewDefinition(definition->getIdentifier(), definition);
}

}
