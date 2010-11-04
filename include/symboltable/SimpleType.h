#ifndef SIMPLETYPE_H_INCLUDED
#define SIMPLETYPE_H_INCLUDED

#include "Type.h"

class SimpleType : public Type
{
public:
	enum BaseType 
	{
		uint8,
		int8,
		uint16,
		int16,
		uint32,
		int32,
		float32,
		float64,
		voidType // extra?
	};

	SimpleType(BaseType baseType) 
		: baseType(baseType) { }

private:
	BaseType baseType;

};

#endif // SIMPLETYPE_H_INCLUDED
