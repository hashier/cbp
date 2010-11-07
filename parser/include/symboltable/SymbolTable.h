#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED

#include<stack>
#include "Scope.h"

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	/// Enters a new scope. Should be called every time something like '{' is encountered
	void EnterNewScope();

	/// Leaves the current scope. Should be called every time something like '}' is encountered.
	/// Currently all information about the left scope is lost of leaving! TODO: change that?
	void LeaveCurrentScope();

	/// Inserts a new Definition into the current scope. 
	/// Throws a DefinitionAlreadyExistsException if there is already a definition with that identifier
	void InsertDefinition(const std::string &identifier, const Definition &definition);

	/// Returns the definition with given identifier. Starts the search in the current scope and continues
	/// the search in the parent scope(s)
	/// Throws a DefinitionNotFoundException if definition is not found
	Definition &GetDefinition(const std::string &identifier);

private:
	std::stack<Scope> scopeStack;
};

#endif // SYMBOLTABLE_H_INCLUDED
