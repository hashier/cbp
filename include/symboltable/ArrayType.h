#ifndef ARRAYTYPE_H_INCLUDED
#define ARRAYTYPE_H_INCLUDED

#include "Type.h"
#include "SimpleType.h"

class ArrayType : public Type
{
public:


private:
	SimpleType::BaseType baseType;
	unsigned size;

};

#endif // ARRAYTYPE_H_INCLUDED
