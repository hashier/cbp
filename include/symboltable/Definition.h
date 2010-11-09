#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#include "Type.h"
#include<string>
#include<exception>

class Definition
{
public:
    bool isInitialized() const { return initialized; }
    void initialize();
    
protected:
    Definition(std::string &identifier, bool initialized) 
        : identifier(identifier), initialized(initialized) { }

private:
    std::string identifier;
    bool initialized;
};

class DefinitionNotFoundException : public std::exception
{
  virtual const char* what() const throw()
  {
    return "Definition not found TODO";
  }
};

class DefinitionAlreadyExistsException : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Definition already exists TODO";
    }
};

#endif // DEFINITION_H_INCLUDED
