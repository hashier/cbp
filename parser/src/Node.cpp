#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "AbstractNodes.h"

SymbolTable::SymbolTable *Node::symbolTable = new SymbolTable::SymbolTable(); // wtf

Type* TypeDecl::getDeclaredType(std::string *identifier)
{
    TypeDecl *tdef = NULL;
    try
    {
        if ((tdef = dynamic_cast<TypeDecl *>(symbolTable->getDefinition(*identifier))))
        {
            return tdef->getType();
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Undefined Type: " << *identifier << " " << e.what() << std::endl;
    }
    return NULL;
}

void SwitchCase::dump(int num) {
    indent(num); std::cout << "SwitchCase {" << std::endl;
    num += 1;
    indent(num); std::cout << "Condition:" << std::endl;
    which->dump(num + 1);
    std::list<Case*>::const_iterator caseIter = cases->begin();
    for(; caseIter != cases->end(); ++caseIter ){
        indent(num); std::cout << "Case" << std::endl;
        (*caseIter)->condition->dump(num + 1);
        indent(num); std::cout << "Action:" << std::endl;
        (*caseIter)->action->dump(num + 1);
    }
    num -= 1;
    indent(num); std::cout << "}" << std::endl;
}
