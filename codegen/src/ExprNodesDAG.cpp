#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

#include <vector>

DAG::Node *Binary::addToDAG(DAG::Container *graph) {
    return graph->addToDAG(left->addToDAG(graph), right->addToDAG(graph), this);
}


DAG::Node *ConstInt::addToDAG(DAG::Container *graph)
{
    return graph->addToDAG(this->val(), this);
}

DAG::Node *Expr_Identifier::addToDAG(DAG::Container *graph)
{
    return graph->addToDAG(this->getRef()->getIdentifier(), this);
}

void IfElse::searchAndReplaceBlocks()
{
    if (dynamic_cast<Block *>(then))
        then = dynamic_cast<Block *>(then)->replaceBlock();
    else
        then->searchAndReplaceBlocks();

    if (dynamic_cast<Block *>(otherwise))
        otherwise = dynamic_cast<Block *>(otherwise)->replaceBlock();
    else
        otherwise->searchAndReplaceBlocks();
}

void WhileLoop::searchAndReplaceBlocks()
{
    if (dynamic_cast<Block *>(body))
        body = dynamic_cast<Block *>(body)->replaceBlock();
    else
        body->searchAndReplaceBlocks();
}

void SwitchCase::searchAndReplaceBlocks()
{
    for (std::list<SwitchCase::Case *>::iterator it = cases->begin(); it != cases->end(); it++)
    {
        if (dynamic_cast<Block *>((*it)->action))
            (*it)->action = dynamic_cast<Block *>((*it)->action)->replaceBlock();
        else
            (*it)->action->searchAndReplaceBlocks();
    }
}

void ForLoop::searchAndReplaceBlocks()
{
    if (dynamic_cast<Block *>(body))
        body = dynamic_cast<Block *>(body)->replaceBlock();
    else
        body->searchAndReplaceBlocks();
}

