#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

using namespace std;

Block::Block()
{
    dag = new DAG::DirectedAcyclicGraph();
}

void Block::insertBefore(unsigned int where, Statement* item)
{
    std::list<Statement*>::iterator it = subs.begin();
    for(unsigned int i=0; i<where; i++)
        it++;
    subs.insert(it, item);
}

void Block::insertAfter(unsigned int where, Statement* item)
{
    std::list<Statement*>::iterator it = subs.begin();
    for(unsigned int i=0; i<where; i++)
        it++;
    it++;
    subs.insert(it, item);
}

void Block::erase(unsigned int where)
{
    std::list<Statement*>::iterator it = subs.begin();
    for(unsigned int i=0; i<where; i++)
        it++;
    subs.erase(it);
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
    for ( it = subs.begin(); it != subs.end(); it++ ) {
          // If so, trade some information: offset for size
          offset += (*it)->calcStackOffset(offset);
    }
    // Return the offset
    return offset;
}

DAG::Node *Block::addToDAG(DAG::DirectedAcyclicGraph *graph)
{
     for (std::list<Statement*>::iterator it = subs.begin(); it != subs.end(); it++) {
         (*it)->addToDAG(graph);
     }
     // TODO return?
     return NULL;
}
