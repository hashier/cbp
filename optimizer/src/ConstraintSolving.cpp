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
    return Interval::world();
}

void Function::solveConstraints(/*SymbolTable*/){
    std::cout << "function" << std::endl;
    statement->solveConstraints();
}

void Block::solveConstraints(/*SymbolTable*/){
    std::list<Statement*>::iterator subStatementIter = subs.begin();
    std::list<Statement*>::iterator subStatementEnd = subs.end();
    for(; subStatementIter != subStatementEnd; ++subStatementIter){
        (*subStatementIter)->solveConstraints();
    }
}

void Declaration::solveConstraints(/*SymbolTable*/){
    std::cout << "declaration: " << getInterval() << std::endl;
}

void Expr_Assign::solveConstraints(/*SymbolTable*/){
    Interval rhsConstraint = getRight()->constraints();
    std::cout << "assign: " << rhsConstraint;
    
    Variable* lhs = dynamic_cast<Variable*>(getLeft());
    if(lhs != 0){
        std::cout << " to " << lhs->getIdentifier() << "." << std::endl;
    }
    else {
        std::cout << " lhs unknown." << std::endl;
    }
}

Interval ConstInt::constraints(/*SymbolTable*/){
    return Interval(val());
}
