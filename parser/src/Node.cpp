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
        std::cerr << "Error: Undefined Type: " << *identifier << " " << e.what() << std::endl;
    }
}

NodeType::NodeType(std::list<Variable *>* members)
{
    type = new TypeStruct(*members);
}
