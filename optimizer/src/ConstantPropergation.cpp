#include "ConstantPropergation.h"

void optimizeTree_ConstantPropergation(File* file) {
    file->constProp();
}

void calcConstExpr(Expression* expr) {
    TypeSimple* type = dynamic_cast<TypeSimple*>(expr->getType());

	if(type->isInteger() && expr->getConstant()!=NULL) {
        int i = expr->getConstant()->integer;
        delete expr;
		expr = new ConstInt(i);
    } else if(type->isFloating() && expr->getConstant()!=NULL) {
        float f = expr->getConstant()->floating;
        delete expr;
        expr = new ConstFloat(f);
    }
}

void calcConstExpr(Statement* stmt) {
    Expression* expr = (Expression*) stmt;

    calcConstExpr(expr);
}
