#include "ConstraintSolving.h"

#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "Variables.h"
#include "AbstractNodes.h"

#include <iostream>

void optimizeTree_ConstraintSolving(File* file) {
    std::cout << " - Constraint Solving" << std::endl;
    file->solveConstraints();
}

void File::solveConstraints(){
    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->solveConstraints();
        }
    }

    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->solveConstraints();
        }
    }
}

void Statement::solveConstraints(/*SymbolTable*/){
    std::cout << "statement: " << typeid(*this).name() << std::endl;
}

void Expression::solveConstraints(/*SymbolTable*/){
    std::cout << "expression" << std::endl;
}

Interval Expression::constraints(/*SymbolTable*/){
    std::cout << "expression " << typeid(*this).name() << std::endl;
    return Interval::world();
}

void Function::solveConstraints(/*SymbolTable*/){
    std::cout << "function" << std::endl;
    // check if forward decl
    if(statement != 0){
        statement->solveConstraints();
    }
}

void Block::solveConstraints(/*SymbolTable*/){
    std::list<Statement*>::iterator subStatementIter = subs.begin();
    std::list<Statement*>::iterator subStatementEnd = subs.end();
    for(; subStatementIter != subStatementEnd; ++subStatementIter){
        (*subStatementIter)->solveConstraints();
    }
}

void Variable::solveConstraints(/*SymbolTable*/){
    std::cout << "variable: " << getInterval() << std::endl;
}

void Expr_Assign::solveConstraints(/*SymbolTable*/){
    Interval rhsConstraint = getRight()->constraints();
    std::cout << "assign: " << rhsConstraint;

    Expr_Identifier* lhs = dynamic_cast<Expr_Identifier*>(getLeft());
    if(lhs != 0){
        Variable* lhsVar = lhs->getRef();
        std::cout << " to " << lhsVar->getIdentifier() << "." << std::endl;
        lhsVar->setInterval(rhsConstraint);
    }
    else {
        std::cout << " lhs unknown." << std::endl;
    }
}

Interval ConstInt::constraints(/*SymbolTable*/){
    return Interval(val());
}

Interval Expr_Add::constraints(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->constraints();
    Interval rhsConstraint = getRight()->constraints();
    Interval res = lhsConstraint + rhsConstraint;
    std::cout << "add: " << res << std::endl;
    return res;
}

Interval Expr_Sub::constraints(/*SymbolTable*/){
    return Interval::world();
}

Interval Expr_Mul::constraints(/*SymbolTable*/){
    return Interval::world();
}

Interval Expr_Div::constraints(/*SymbolTable*/){
    return Interval::world();
}

