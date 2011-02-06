#include"Variables.h"
#include"ProgramNodes.h"
#include"Message.h"

using namespace std;

void Function::gen(CodeGen* out) {
    *out << Message(DEBUG, "Function::gen()", this);
    // ignore forward declarations
    if(statement) {
        Label mark = getMark(out);

        if (identifier == "main") {
            *out << (Directive(".globl") << ' ' << (out->isWithUnderscore() ? "_" : "") << "_cbp_main");
            *out << Label((out->isWithUnderscore() ? "_" : "") + string("_cbp_main"));
        }

        *out << mark;

        if(arguments->size() > 0) {
            std::list<Variable*>::iterator it;
            int offset = 8;
            for ( it = arguments->begin(); it != arguments->end(); it++ ) {
                  // Since we push all our variables using pushq,
                  // the offsets have to be of fixed size 8 (quadword)
                  offset += 8;
                  // And we need to negate the builtin size-offset
                  (*it)->setStackOffset(offset, false);
            }
        }

        // Only do this once: Calculate offsets for variables, and total required stack space
        int offset = statement->calcStackOffset(0);
        offset = offset > 0 ? offset : -offset;

        //debug helper
        //std::cout << " -" << getIdentifier() << " : stackframe=" << offset << std::endl;

        *out << Command("pushq")("%rbp");
        *out << Command("movq")("%rsp")("%rbp");

        // Do we even have local variables?
        if(offset > 0) {
            // Align to something divisible by 4.
#ifndef __APPLE__
            if(offset % 4 != 0)
                offset += 4 - offset % 4;
#else
            //Align to something divisible by 16.
            if(offset % 16 != 0)
                offset += 16 - offset % 16;
#endif
            *out << Command("subq")(offset)("%rsp");
        }

        Block* tmp = dynamic_cast<Block*>(statement);
        if(tmp)
            tmp->gen(out, true);
        else
            statement->gen(out);

        *out << Command("leave");

        // Return from this function
        *out << Command("ret");

    }
}
