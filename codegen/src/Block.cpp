#include"ProgramNodes.h"

using namespace std;

void Block::gen(CodeGen* out) {
    // Ok, now just output all the substatements one after another
    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ ) {
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
