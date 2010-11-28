#include"ProgramNodes.h"

using namespace std;

void Block::gen(CodeGen* out) {
    // Only do this once: Calculate offsets for variables, and total required stack space
    int offset = calcStackOffsets();
    // Do we even have local variables?
    if(!offset) {
        // Align to something divisible by 4.
        if(offset % 4 != 0)
            offset += offset % 4;
        *out << "pushl %ebp" << endl;
        *out << "movl %esp, %ebp" << endl;
        *out << "subl $" << offset << ", %esp" << endl;

        std::list<Statement*>::iterator it;
        for ( it = subs.begin() ; it != subs.end(); it++ ) {
            // Try casting..
            Local* local = dynamic_cast<Local*>(*it);
            // ..is this a local variable?
            if(local)
                // If so, trade some information: offset for size
                offset -= local->setStackOffset(offset);
        }
    }

    // Ok, now just output all the substatements one after another
    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ )
        (*it)->gen(out);

    // Tidy up the cellar.
    if(!offset) {
        *out << "movl %ebp, %esp" << endl;
        *out << "pop %ebp" << endl;
    }

}

int Block::calcStackOffsets() {
    int offset = 0;
    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ ) {
        // Try casting..
        Local* local = dynamic_cast<Local*>(*it);
        // ..is this a local variable?
        if(local)
            // If so, trade some information: offset for size
            offset -= local->setStackOffset(offset);
    }
    // Return the (absolute'd) offset
    return -offset;
}
