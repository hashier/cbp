#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#include "Type.h"

class Definition
{
public:
	Definition(const Type &type, bool isConstant, bool isInitialized);
	void Initialize();

private:
	Type type;
	bool initialized;
	bool isConstant;
}

#endif // DEFINITION_H_INCLUDED
