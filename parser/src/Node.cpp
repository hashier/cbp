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

Expr_Identifier::Expr_Identifier(std::string *identifier)
{
    try
    {
        ref = dynamic_cast<Variable *>(symbolTable->getDefinition(*identifier));
        if (ref == NULL)
        {
            std::cerr << "Error: Identifier '" << *identifier << "' in Expression is not declared as a variable." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Undefined identifier in Expression: '" << *identifier << "'." << std::endl;
    }
}


void Expr_Identifier::dump(int num) {
    std::cout << "Identifier in Expression:" << std::endl;
    ref->dump(num+1);
}
