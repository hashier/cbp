#include"ProgramNodes.h"

using namespace std;

void Block::gen(CodeGen* out, bool outermost) {
    // Ok, now just output all the substatements one after another
    std::list<Statement*>::iterator it, it2;
    // Not really content with this double-iterator solution for second-last element..
    for ( it = subs.begin(), it2 = ++subs.begin() ; it != subs.end(); it++, it2++ ) {

        // Is this the last statement of an outermost block in a function?
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
