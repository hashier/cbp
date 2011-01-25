#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

#include <vector>

DAG::Node *Expr_Assign::addToDAG(DAG::DirectedAcyclicGraph *graph) {
    return graph->addToDAG(left->addToDAG(graph), right->addToDAG(graph), DAG::ASSIGN, this);
}

DAG::Node *Expr_Add::addToDAG(DAG::DirectedAcyclicGraph *graph) {
    return graph->addToDAG(left->addToDAG(graph), right->addToDAG(graph), DAG::PLUS, this);
}

DAG::Node *ConstInt::addToDAG(DAG::DirectedAcyclicGraph *graph)
{
    return graph->addToDAG(this->val(), this);
}

DAG::Node *Expr_Identifier::addToDAG(DAG::DirectedAcyclicGraph *graph)
{
    return graph->addToDAG(this->getRef()->getIdentifier(), this);
}



