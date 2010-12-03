#include"ProgramNodes.h"

using namespace std;

void Function::gen(CodeGen* out) {
    Mark mark = getMark(out);

    *out << mark << ":" << endl;

    if (identifier == "main")
	*out << "__cbp_main:" << endl;

    if(statement)
        statement->gen(out);
    else
        *out << "NULL function!" << endl;

}
