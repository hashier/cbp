#include"ProgramNodes.h"

using namespace std;

void Function::gen(CodeGen* out) {
    Label mark = getMark(out);

    if (identifier == "main") {
        *out << Command(".globl")((out->isWithUnderscore() ? "_" : ""), string("_cbp_main"));
        *out << Label((out->isWithUnderscore() ? "_" : "") + string("_cbp_main"));
    }

    *out << mark;

    if(statement) {

        // Only do this once: Calculate offsets for variables, and total required stack space
        int offset = statement->calcStackOffset(0);
        offset = offset > 0 ? offset : -offset;

        // Do we even have local variables?
        if(offset > 0) {
            // Align to something divisible by 4.
            if(offset % 4 != 0)
                offset += offset % 4;
            *out << Command("pushq")("%rbp");
            *out << Command("movq")("%rsp,")("%rbp");
            *out << Command("subq")("$",offset)(", %rsp");

            statement->gen(out);

            // TODO use this?
            // *out << Command("leave");
            *out << Command("movq")("%rbp,")("%rsp");
            *out << Command("popq")("%rbp");

        } else {
            statement->gen(out);
        }


    } else
        *out << Command("NULL function!");

}
