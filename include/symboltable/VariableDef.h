#ifndef VARIABLEDEF_H_INCLUDED
#define VARIABLEDEF_H_INCLUDED

#include "Definition.h"

class Type;

namespace SymbolTable
{

class VariableDef : public Definition
{
public:
    VariableDef(const std::string &identifier, bool initialized, Type *type)
        : Definition(identifier), type(type) { }
    Type *getType() { return type; }

private:
    Type *type;

};

}
#endif // VARIABLEDEF_H_INCLUDED
