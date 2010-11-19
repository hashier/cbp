#include"ProgramNodes.h"

void File::gen(CodeGen* out) {

    (*out) << "this is some asm stuff" << std::endl;

    {
        std::list<TypeDecl*>::iterator it;
        for ( it = types.begin() ; it != types.end(); it++ ) {
            (*it)->gen(out);
        }
    }

    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->gen(out);
        }
    }

    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->gen(out);
        }
    }

}

