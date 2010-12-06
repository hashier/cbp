#include"ProgramNodes.h"

using namespace std;

void Function::gen(CodeGen* out) {
    Mark mark = getMark(out);

    if (identifier == "main") {
        *out << ".globl " << (out->isWithUnderscore() ? "_" : "") << "_cbp_main" << endl;
        *out << (out->isWithUnderscore() ? "_" : "") << "_cbp_main:" << endl;
    }

    *out << mark << ":" << endl;

    if(statement) {

        // Only do this once: Calculate offsets for variables, and total required stack space
        int offset = statement->calcStackOffset(0);
        offset = offset > 0 ? offset : -offset;

        // Do we even have local variables?
        if(offset > 0) {
            // Align to something divisible by 4.
            if(offset % 4 != 0)
                offset += offset % 4;
            *out << "\tpushq\t%rbp" << endl;
            *out << "\tmovq\t%rsp, %rbp" << endl;
            *out << "\tsubq\t$" << offset << ", %rsp" << endl;

            statement->gen(out);

            // TODO use this?
            // *out << "leave" << endl;
            *out << "\tmovq\t%rbp, %rsp" << endl;
            *out << "\tpopq\t%rbp" << endl;

        } else {
            statement->gen(out);
        }


    } else
        *out << "NULL function!" << endl;

}
