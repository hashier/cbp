#ifndef SIMPLETYPE_H_INCLUDED
#define SIMPLETYPE_H_INCLUDED

#include "Type.h"
#include "Types.h"

class SimpleType : public Type
{
public:
    SimpleType(BaseType baseType) 
        : baseType(baseType) { }

private:
    SimpleTypeEnum baseType;

};

#endif // SIMPLETYPE_H_INCLUDED
