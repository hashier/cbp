#include"ProgramNodes.h"

using namespace std;

void Function::gen(CodeGen* out) {
    Mark mark = getMark(out);

    *out << mark << ":" << endl;

    if (identifier == "main")
        *out << "__cbp_main:" << endl;

    if(statement) {

        // Only do this once: Calculate offsets for variables, and total required stack space
        int offset = statement->calcStackOffset(0);
        offset = offset > 0 ? offset : -offset;

        // Do we even have local variables?
        if(offset > 0) {
            // Align to something divisible by 4.
            if(offset % 4 != 0)
                offset += offset % 4;
            *out << "pushq %rbp" << endl;
            *out << "movq %rsp, %rbp" << endl;
            *out << "subq $" << offset << ", %rsp" << endl;

            statement->gen(out);

            // TODO use this?
            // *out << "leave" << endl;
            *out << "movq %rbp, %rsp" << endl;
            *out << "popq %rbp" << endl;

        } else {
            statement->gen(out);
        }


    } else
        *out << "NULL function!" << endl;

}
