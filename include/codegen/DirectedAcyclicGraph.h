#include <list>
#include "ProgramNodes.h"
#include "DAGNodes.h"
#include "DAGIdentifierMap.h"

#pragma once

namespace DAG {

class DirectedAcyclicGraph {
public:
    DirectedAcyclicGraph(std::list<Statement*> *statements)
        : statements(statements) 
    {  
    }

    DirectedAcyclicGraph() 
    { 

    }

    // Add node for expression
    Node *addToDAG(Node *left, Node *right, Expression *expr);

    // Add node for constant int value
    Node *addToDAG(int value, Expression *expr); 

    // Add node for identifier
    Node *addToDAG(std::string &name, Expression *expr); 

    void dumpAll();

    void gen(CodeGen* out, bool outermost);
    
private:
    std::list<Statement*> *statements;
    IdentifierMap map;
};

}
