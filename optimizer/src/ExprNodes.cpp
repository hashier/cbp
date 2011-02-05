#include "ExprNodes.h"
#include "ConstantPropergation.h"
#include "Variables.h"

void Unary::constProp() {
    sub->constProp();
}

bool Unary::isConst() {
    return sub->isConst();
}

constant* Unary::getConstant() {
    return NULL;
}

void Binary::constProp() {
    (right->isConst()) ? calcConstExpr(&right) : right->constProp();
    (left->isConst()) ? calcConstExpr(&left) : left->constProp();
}

bool Binary::isConst() {
    return left->isConst() && right->isConst();
}

constant* Binary::getConstant() {
    return NULL;
}

bool Expr_Assign::isConst() {
    return false;
}

void Expr_Assign::constProp() {
    (right->isConst()) ? calcConstExpr(&right) : right->constProp();

    // if left child is a variable
    if(left->getChildren().size() == 0) {
        bool varExists = false;

        std::string identifier = ((Expr_Identifier*) left)->getRef()->getIdentifier();
        std::list<std::list<ConstVar*>* >::reverse_iterator it1 = blocks.rbegin();
        for(; it1 != blocks.rend(); it1++) {
            std::list<ConstVar*>::iterator it2 = (*it1)->begin();
            for(; it2 != (*it1)->end(); it2++) {
                if((*it2)->getIdentifier() == identifier) {
                    if(right->isConst()) {
                        (*it2)->setValue(right->getConstant());
                    } else {
                        (*it1)->remove(*it2);
                    }
                    varExists = true;
                    break;
                }
            }
        }

        if(!varExists && right->isConst()) {
            std::list<ConstVar*>* con = blocks.back();
            con->push_back(new ConstVar(identifier, right->getConstant()));
        }
    }
}

constant* Expr_Add::getConstant() {
    TypeSimple* typeLeft = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* typeRight = dynamic_cast<TypeSimple*>(right->getType());
    constant* conLeft = left->getConstant();
    constant* conRight = right->getConstant();
    constant* addConst = new constant();
    if(typeLeft->isInteger() && typeRight->isInteger())
        addConst->integer = conLeft->integer + conRight->integer;
    else if(typeLeft->isFloating() && typeRight->isFloating())
        addConst->floating = conLeft->floating + conRight->floating;
    else if(typeLeft->isFloating() && typeRight->isInteger())
        addConst->floating = conLeft->floating + conRight->integer;
    else
        addConst->floating = conLeft->integer + conRight->floating;
    return addConst;
}

constant* Expr_Sub::getConstant() {
    TypeSimple* typeLeft = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* typeRight = dynamic_cast<TypeSimple*>(right->getType());
    constant* conLeft = left->getConstant();
    constant* conRight = right->getConstant();
    constant* subConst = new constant();
    if(typeLeft->isInteger() && typeRight->isInteger())
        subConst->integer = conLeft->integer - conRight->integer;
    else if(typeLeft->isFloating() && typeRight->isFloating())
        subConst->floating = conLeft->floating - conRight->floating;
    else if(typeLeft->isFloating() && typeRight->isInteger())
        subConst->floating = conLeft->floating - conRight->integer;
    else
        subConst->floating = conLeft->integer - conRight->floating;
    return subConst;
}

constant* Expr_Mul::getConstant() {
    TypeSimple* typeLeft = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* typeRight = dynamic_cast<TypeSimple*>(right->getType());
    constant* conLeft = left->getConstant();
    constant* conRight = right->getConstant();
    constant* mulConst = new constant();
    if(typeLeft->isInteger() && typeRight->isInteger())
        mulConst->integer = conLeft->integer * conRight->integer;
    else if(typeLeft->isFloating() && typeRight->isFloating())
        mulConst->floating = conLeft->floating * conRight->floating;
    else if(typeLeft->isFloating() && typeRight->isInteger())
        mulConst->floating = conLeft->floating * conRight->integer;
    else
        mulConst->floating = conLeft->integer * conRight->floating;
    return mulConst;
}

constant* Expr_Div::getConstant() {
    TypeSimple* typeLeft = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* typeRight = dynamic_cast<TypeSimple*>(right->getType());
    constant* conLeft = left->getConstant();
    constant* conRight = right->getConstant();
    constant* mulConst = new constant();
    if(typeLeft->isInteger() && typeRight->isInteger())
        mulConst->integer = conLeft->integer / conRight->integer;
    else if(typeLeft->isFloating() && typeRight->isFloating())
        mulConst->floating = conLeft->floating / conRight->floating;
    else if(typeLeft->isFloating() && typeRight->isInteger())
        mulConst->floating = conLeft->floating / conRight->integer;
    else
        mulConst->floating = conLeft->integer / conRight->floating;
    return mulConst;
}

constant* Expr_Mod::getConstant() {
    TypeSimple* typeLeft = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* typeRight = dynamic_cast<TypeSimple*>(right->getType());
    constant* conLeft = left->getConstant();
    constant* conRight = right->getConstant();
    constant* mulConst = new constant();
    if(typeLeft->isInteger() && typeRight->isInteger())
        mulConst->integer = conLeft->integer % conRight->integer;
    else if(typeLeft->isFloating() && typeRight->isFloating())
        mulConst->floating = (float)( (int) conLeft->floating % (int) conRight->floating );
    else if(typeLeft->isFloating() && typeRight->isInteger())
        mulConst->floating = (float)( (int) conLeft->floating % (int) conRight->floating );
    else
        mulConst->floating = (float)( (int) conLeft->integer % (int) conRight->floating );
    return mulConst;
}

bool Expr_Struc::isConst() {
    return false;
}

constant* Expr_Struc::getConstant() {
    return NULL;
}

void Expr_Struc::constProp() {
    // sub->constProp();
}

bool Constant::isConst() {
    return true;
}

constant* ConstInt::getConstant() {
    constant* intConst = new constant();
    intConst->integer = value;
    return intConst;
}

constant* ConstFloat::getConstant() {
    constant* floatConst = new constant();
    floatConst->floating = value;
    return floatConst;
}

void Expr_Identifier::constProp() {
    ref->constProp();
}

bool Expr_Identifier::isConst() {
    std::string identifier = this->getRef()->getIdentifier();
    std::list<std::list<ConstVar*>* >::reverse_iterator it1 = blocks.rbegin();
    for(; it1 != blocks.rend(); it1++) {
        std::list<ConstVar*>::iterator it2 = (*it1)->begin();
        for(; it2 != (*it1)->end(); it2++) {
            if((*it2)->getIdentifier() == identifier) {
                return true;
            }
        }
    }
    return false;
}

constant* Expr_Identifier::getConstant() {
    std::string identifier = this->getRef()->getIdentifier();
    std::list<std::list<ConstVar*>* >::reverse_iterator it1 = blocks.rbegin();
    for(; it1 != blocks.rend(); it1++) {
        std::list<ConstVar*>::iterator it2 = (*it1)->begin();
        for(; it2 != (*it1)->end(); it2++) {
            if((*it2)->getIdentifier() == identifier) {
                return (*it2)->getValue();
            }
        }
    }
    
    return NULL;
}

void Expr_Cast::constProp() {
    // if(castType != NULL) {
        // castType->constProp();
    // }
}

bool Expr_Cast::isConst() {
    return false;
}

constant* Expr_Cast::getConstant() {
    return expr->getConstant();
}

void FuncCall::constProp() {

}

bool FuncCall::isConst() {
    return false;
}

constant* FuncCall::getConstant() {
    return NULL;
}
