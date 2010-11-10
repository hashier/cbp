#ifndef DEFINITION_H_INCLUDED
#define DEFINITION_H_INCLUDED

#include<string>
#include<exception>

class Type;

namespace SymbolTable
{

class Definition
{
public:
    //bool isInitialized() const { return initialized; }
    //void initialize();

    const std::string &getIdentifier() const { return identifier; }
    
    Definition(const std::string &identifier) 
        : identifier(identifier) { }

protected:


    std::string identifier;
};

class TypeDef : public Definition
{
public:
     TypeDef(const std::string &identifier, Type *type)
         : Definition(identifier), type(type) { }

protected:
    Type *type;
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

}

#endif // DEFINITION_H_INCLUDED
