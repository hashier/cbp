#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"

using namespace std;

Block::Block()
{
    dag = new DAG::DirectedAcyclicGraph();
}

std::list<Statement*> Block::getSubStatements()
{
    return subs;
}

void Block::gen(CodeGen* out, bool outermost) {
    *out << Message(DEBUG, "Block::gen()");
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
