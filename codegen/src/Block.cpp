#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

using namespace std;

Block::Block()
{
}

void Block::insertBefore(Statement* where, Statement* item)
{
    for(std::list<Statement*>::iterator it=subs.begin(); it!=subs.end(); ++it)
    {
        if((*it)==where)
        {
            subs.insert(it, item);
            return;
        }
    }
}

void Block::insertAfter(Statement* where, Statement* item)
{
    for(std::list<Statement*>::iterator it=subs.begin(); it!=subs.end(); ++it)
    {
        if((*it)==where)
        {
            it++;
            subs.insert(it, item);
            return;
        }
    }
}

void Block::erase(Statement* what)
{
    for(std::list<Statement*>::iterator it=subs.begin(); it!=subs.end(); ++it)
    {
        if((*it)==what)
        {
            subs.erase(it);
            return;
        }
    }
}

void Block::gen(CodeGen* out, bool outermost) {
    *out << Message(DEBUG, "Block::gen()", this);

    /*
    if (dag)
    {
        dag->gen(out, outermost);
        return;
    }
    */


    // Ok, now just output all the substatements one after another
    for (std::list<Statement*>::iterator it = subs.begin(); it != subs.end(); it++) {
        // Is this the last statement of an outermost block in a function?
        std::list<Statement*>::iterator it2 = it; it2++;
        if(outermost && it2 == subs.end()) {
            // Neat. Is this a return statement?
            Return* tmp = dynamic_cast<Return*>(*it);
            // If it is, pass down the info
            if(tmp)
                tmp->gen(out, true);
            // Otherwise, proceed as usual.
            else
                (*it)->gen(out);
        } else
            (*it)->gen(out);
    }
}

int Block::calcStackOffset(int offset) {
    std::list<Statement*>::iterator it;
    int mySize = 0;
    int tmp;
    for ( it = subs.begin(); it != subs.end(); it++ ) {
          // If so, trade some information: offset for size
            tmp = (*it)->calcStackOffset(offset);
            offset += tmp;
            mySize += tmp;
    }
    // Return the offset
    return mySize;
}

DAG::Container *Block::replaceBlock()
{
    DAG::Container *container = new DAG::Container();
    for (std::list<Statement *>::iterator it = subs.begin(); it != subs.end(); it++)
    {
        if (dynamic_cast<Block *>(*it))
        {
            container->addToDAGContainer(dynamic_cast<Block *>(*it)->replaceBlock());
        }
        else if (dynamic_cast<Expression *>(*it))
        {
            dynamic_cast<Expression *>(*it)->addToDAG(container);
        }
        else
        {
            (*it)->searchAndReplaceBlocks();
            container->addToDAGContainer(*it);
        }
    }
    return container;
}
