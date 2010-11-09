#ifndef VARIABLEDEF_H_INCLUDED
#define VARIABLEDEF_H_INCLUDED

#include "Defintion.h"
#include "Type.h"
#include "Types.h"

class VariableDef : public Definition
{
public:
    VariableDef(const std::string &identifier, bool initialized, TypeST &type)
        : Definition(identifier, initialized), type(type) { }

private:
    TypeST type;

};

#endif // VARIABLEDEF_H_INCLUDED
