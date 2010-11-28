#include"ProgramNodes.h"

using namespace std;

void Function::gen(CodeGen* out) {
    Mark mark = getMark(out);

    *out << mark << ":" << endl;

    if(statement)
        statement->gen(out);
    else
        *out << "NULL function!" << endl;

}
