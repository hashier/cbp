#include <list>
#include "ProgramNodes.h"
#include "DAGNodes.h"
#include "DAGIdentifierMap.h"

#pragma once

namespace DAG {

class DirectedAcyclicGraph {
public:
    DirectedAcyclicGraph(std::list<Statement*> *statements)
        : statements(statements) { }

    DirectedAcyclicGraph() { }

    void buildDAG();

    Node *addToDAG(Node *left, Node *right, Operator op);
    Node *addToDAG(int value); 
    Node *addToDAG(std::string &name); 
    
private:
    std::list<Statement*> *statements;
    IdentifierMap map;
};

}
