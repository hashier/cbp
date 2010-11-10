#include "ProgramNodes.h"
#include "ExprNodes.h"

NodeType::NodeType(std::list<Variable *>* members)
{
    std::cout << "I'm sad, so I will segfault soon." << std::endl;
    // todo sth.
    std::list<SymbolTable::VariableDef *> *defMembers = new std::list<SymbolTable::VariableDef *>();
    for (std::list<Variable *>::iterator it; it != members->end(); it++)
    {
        SymbolTable::VariableDef *v = new SymbolTable::VariableDef(
            (*it)->getIdentifier(), 
            false, 
            (*it)->getType()->getType()
        );
        defMembers->push_back(v);
    }
    type = new TypeStruct(*defMembers);
}

void Expr_Cast::dump(int num)
{
      indent(num); std::cout << "Cast as:" << std::endl;
      if(castType != NULL) {
        castType->dump(num + 1);
      }
}

