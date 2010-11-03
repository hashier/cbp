#ifndef TYPE_H_INCLUDED
#define TYPE_H_INCLUDED

#include <list>

class Type
{
public:
	enum BaseType 
	{
		INT,
		FLOAT,
		STRUCT,
		VOID,
		// ...
		FUNCTION // ?
	};

private:
	BaseType baseType;
	std::list<Definition> parts;

}

#endif // TYPE_H_INCLUDED
