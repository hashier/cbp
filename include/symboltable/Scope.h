#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include<map>
#include<string>

class Declaration;

namespace SymbolTable
{

class Scope
{
public:
    Scope() { }

    Declaration *GetDefinition(const std::string &identifier);
    void InsertNewDefinition(const std::string &identifier, Declaration *definition);

private:
    std::map<std::string, Declaration *> definitions;
};

}

#endif // SCOPE_H_INCLUDED
