#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#include "Type.h"
#include<string>

class Definition
{
public:
	bool IsInitialized() const { return initialized; }
	void Initialize();
	
protected:
	Definition(std::string &identifier, bool initialized) 
		: identifier(identifier), initialized(initialized) { }

private:
	std::string identifier;
	bool initialized;
};

#endif // DEFINITION_H_INCLUDED
