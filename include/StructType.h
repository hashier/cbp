#ifndef STRUCTTYPE_H_INCLUDED
#define STRUCTTYPE_H_INCLUDED

#include "Type.h"
#include <vector>

class StructType : public Type
{
public:


private:
	std::vector<Type> elements;

};

#endif // STRUCTTYPE_H_INCLUDED
