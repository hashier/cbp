#ifndef SCOPE_H_INCLUDED
#define SCOPE_H_INCLUDED

#include<map>
#include<string>

namespace SymbolTable
{

class Definition;

class Scope
{
public:
    Scope() { }

    Definition *GetDefinition(const std::string &identifier);
    void InsertNewDefinition(const std::string &identifier, Definition *definition);

private:
    std::map<std::string, Definition *> definitions;
};

}

#endif // SCOPE_H_INCLUDED
