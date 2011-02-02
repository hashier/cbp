#include "ConstantPropergation.h"

std::list<std::list<ConstVar*>* > blocks;

ConstVar::ConstVar(std::string identifier, constant* value) {
    this->identifier = identifier;
    this->value = value;
}

std::string ConstVar::getIdentifier() {
    return identifier;
}
constant* ConstVar::getValue() {
    return value;
}

void ConstVar::setValue(constant* value) {
    this->value = value;
}

void optimizeTree_ConstantPropergation(File* file) {
    std::cout << " -Constant Propergation" << std::endl;
    file->constProp();
}

void calcConstExpr(Expression** expr) {
    TypeSimple* type = dynamic_cast<TypeSimple*>((*expr)->getType());

    if(type->isInteger() && (*expr)->getConstant()!=NULL) {
        int i = (*expr)->getConstant()->integer;
        delete *expr;
        *expr = new ConstInt(i);
    } else if(type->isFloating() && (*expr)->getConstant()!=NULL) {
        float f = (*expr)->getConstant()->floating;
        delete *expr;
        *expr = new ConstFloat(f);
    }
}

void calcConstExpr(Statement** stmt) {
    calcConstExpr((Expression**)stmt);
}