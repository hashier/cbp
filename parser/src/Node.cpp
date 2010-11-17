#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "AbstractNodes.h"

SymbolTable::SymbolTable *Node::symbolTable = new SymbolTable::SymbolTable(); // wtf


NodeType::NodeType(std::string *identifier)
{
    TypeDecl *tdef = NULL;
    try
    {
        if ((tdef = dynamic_cast<TypeDecl *>(symbolTable->getDefinition(*identifier))))
        {
            this->type = tdef->getType()->getType();
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Undefined Type: " << *identifier << std::endl;
    }
}

NodeType::NodeType(std::list<Variable *>* members)
{
    type = new TypeStruct(*members);
}

void Expr_Cast::dump(int num)
{
      indent(num); std::cout << "Cast as:" << std::endl;
      if(castType != NULL) {
        castType->dump(num + 1);
      }
}



FuncCall::FuncCall(std::string* identifier, std::list<Expression*>* exprs) 
{
    try 
    {
        func = NULL;
        func = dynamic_cast<Function *>(Node::symbolTable->getDefinition(*identifier));
        if (func == NULL)
        {
            std::cerr << "Error: Identifier '" << *identifier << "' is not a function, but is used as a function." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Call to undeclared Function: " << *identifier << std::endl;
    }
}

void FuncCall::dump(int num) 
{
    indent(num); std::cout << "Function Call: " << func->getIdentifier() << std::endl;
}
