#ifndef SYMBOLTABLE_H_INCLUDED
#define SYMBOLTABLE_H_INCLUDED

#include<list>
#include "Scope.h"

class Declaration;

namespace SymbolTable
{

class SymbolTable
{
public:
	SymbolTable();
	~SymbolTable();

	/// Enters a new scope. Should be called every time something like '{' is encountered
	void enterNewScope();

	/// Leaves the current scope. Should be called every time something like '}' is encountered.
	/// Currently all information about the left scope is lost of leaving! TODO: change that?
	void leaveCurrentScope();

	/// Inserts a new Definition into the current scope. 
	/// @throws DefinitionAlreadyExistsException if there is already a definition with that identifier
	void insertDefinition(Declaration *definition);

	/// Inserts a new Definition into the top level (global) scope. 
	/// @throws DefinitionAlreadyExistsException if there is already a definition with that identifier
	void insertGlobalDefinition(Declaration *definition);

	/// Returns the definition with given identifier. Starts the search in the current scope and continues
	/// the search in the parent scope(s)
	/// Throws a DefinitionNotFoundException if definition is not found
	Declaration *getDefinition(const std::string &identifier);

private:
	std::list<Scope> scopes;
};



class DefinitionNotFoundException : public std::exception
{
public:
  virtual const char* what() const throw()
  {
    return "Definition not found TODO";
  }
};

class DefinitionAlreadyExistsException : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Definition already exists TODO";
    }
};

}
#endif // SYMBOLTABLE_H_INCLUDED
