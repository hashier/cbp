#include <list>
#include "ProgramNodes.h"
#include "DAGNodes.h"
#include "DAGIdentifierMap.h"

#pragma once

namespace DAG {

class DirectedAcyclicGraph : public Block {
public:
    DirectedAcyclicGraph(std::list<Statement*> *statements)
        : statements(statements) 
    {  
    }

    DirectedAcyclicGraph() 
    { 

    }

    // Add node for expression
    DAG::Node *addToDAG(DAG::Node *left, DAG::Node *right, Expression *expr);

    // Add node for constant int value
    DAG::Node *addToDAG(int value, Expression *expr); 

    // Add node for identifier
    DAG::Node *addToDAG(std::string &name, Expression *expr); 

    void dumpAll();

    void gen(CodeGen* out, bool outermost);
    
private:
    std::list<Statement*> *statements;
    IdentifierMap map;
};


class Container : public Block
{
public:
    Container() : Block(), current(NULL)
    { 
        
    }

    // Add node for expression
    DAG::Node *addToDAG(DAG::Node *left, DAG::Node *right, Expression *expr)
    {
        if (!current)
        {
            current = new DirectedAcyclicGraph();
            statements.push_back(current);
        }
        return current->addToDAG(left, right, expr);
    }

    // Add node for constant int value
    DAG::Node *addToDAG(int value, Expression *expr)
    {
        if (!current)
        {
            current = new DirectedAcyclicGraph();
            statements.push_back(current);
        }
        return current->addToDAG(value, expr);
    }

    // Add node for identifier
    DAG::Node *addToDAG(std::string &name, Expression *expr)
    {
        if (!current)
        {
            current = new DirectedAcyclicGraph();
            statements.push_back(current);
        }
        return current->addToDAG(name, expr);
    }

    void addToDAGContainer(Statement *statement)
    {
        current = NULL;
        statements.push_back(statement);
    }

    void gen(CodeGen* out, bool outermost)
    {
        for (std::list<Statement*>::iterator it = statements.begin(); it != statements.end(); it++)
        {
            (*it)->gen(out);
        }
    }
    
private:
    std::list<Statement*> statements;
    DirectedAcyclicGraph *current;
    
};


}
