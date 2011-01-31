#include "ProgramNodes.h"
#include "ConstantPropergation.h"
#include "Variables.h"

void Function::constProp() {
    if(statement)
        (statement->isConst()) ? calcConstExpr(&statement) : statement->constProp();
}

void File::constProp() {
    std::list<TypeDecl*>::iterator it1;
    for (it1 = types.begin(); it1 != types.end(); it1++) {
        (*it1)->constProp();
    }

    std::list<Variable*>::iterator it2;
    for (it2 = variables.begin(); it2 != variables.end(); it2++) {
        (*it2)->constProp();
    }

    std::list<Function*>::iterator it3;
    for (it3 = functions.begin(); it3 != functions.end(); it3++) {
        (*it3)->constProp();
    }
}

void Block::constProp() {
    std::list<Statement*>::iterator it;
    for (it = subs.begin(); it != subs.end(); it++) {
        ((*it)->isConst()) ? calcConstExpr(&(*it)) : (*it)->constProp();
    }
}

bool Block::isConst() {
    return false;
}

constant* Block::getConstant() {
    return NULL;
}

void IfElse::constProp() {
    (condition->isConst()) ? calcConstExpr(&condition) : condition->constProp();
    (then->isConst()) ? calcConstExpr(&then) : then->constProp();
    if(otherwise) {
        (otherwise->isConst()) ? calcConstExpr(&otherwise) : otherwise->constProp();
    }
}

bool IfElse::isConst() {
    return false;
}

constant* IfElse::getConstant() {
    return NULL;
}

void WhileLoop::constProp() {
    (condition->isConst()) ? calcConstExpr(&condition) : condition->constProp();
    if(body==NULL) return;
    (body->isConst()) ? calcConstExpr(&body) : body->constProp();
}

bool WhileLoop::isConst() {
    return false;
}

constant* WhileLoop::getConstant() {
    return NULL;
}

void Return::constProp() {
    if(expr==NULL) return;
    (expr->isConst()) ? calcConstExpr(&expr) : expr->constProp();
}

bool Return::isConst() {
    return false;
}

constant* Return::getConstant() {
    return NULL;
}

void Local::constProp() {
    var->constProp();
}

bool Local::isConst() {
    return false;
}

constant* Local::getConstant() {
    return NULL;
}

void ForLoop::constProp() {
    (init_value->isConst()) ? calcConstExpr(&init_value) : init_value->constProp();

    (final_value->isConst()) ? calcConstExpr(&final_value) : final_value->constProp();

    if(step!=NULL) {
        (step->isConst()) ? calcConstExpr(&step) : step->constProp();
    }

    if(body==NULL) return;
    (body->isConst()) ? calcConstExpr(&body) : body->constProp();
}

bool ForLoop::isConst() {
    return false;
}

constant* ForLoop::getConstant() {
    return NULL;
}

void SwitchCase::constProp() {
    (which->isConst()) ? calcConstExpr(&which) : which->constProp();
    std::list<Case*>::const_iterator caseIter = cases->begin();
    for(; caseIter != cases->end(); ++caseIter) {
        ((*caseIter)->action->isConst()) ? calcConstExpr(&((*caseIter)->action)) : (*caseIter)->action->constProp();
    }
}

bool SwitchCase::isConst() {
    return false;
}

constant* SwitchCase::getConstant() {
    return NULL;
}
