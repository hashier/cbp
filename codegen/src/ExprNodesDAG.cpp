#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

#include <vector>

DAG::Node *Binary::addToDAG(DAG::DirectedAcyclicGraph *graph) {
    return graph->addToDAG(left->addToDAG(graph), right->addToDAG(graph), this);
}


DAG::Node *ConstInt::addToDAG(DAG::DirectedAcyclicGraph *graph)
{
    return graph->addToDAG(this->val(), this);
}

DAG::Node *Expr_Identifier::addToDAG(DAG::DirectedAcyclicGraph *graph)
{
    return graph->addToDAG(this->getRef()->getIdentifier(), this);
}



