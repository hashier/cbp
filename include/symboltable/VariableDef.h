#ifndef VARIABLEDEF_H_INCLUDED
#define VARIABLEDEF_H_INCLUDED

#include "Defintion.h"
#include "Type.h"

class VariableDef : public Definition
{
public:
	VariableDef(const std::string &identifier, bool initialized, Type &type)
		: Definition(identifier, initialized), type(type) { }

private:
	Type type;

};

#endif // VARIABLEDEF_H_INCLUDED
