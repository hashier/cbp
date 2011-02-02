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

ExpressionProperties Expression::properties(/*SymbolTable*/){
    std::cout << "expression " << typeid(*this).name() << std::endl;
    return ExpressionProperties();
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
    Interval rhsConstraint = getRight()->properties().interval;
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

ExpressionProperties ConstInt::properties(/*SymbolTable*/){
    Interval domain = Interval(val());
    return ExpressionProperties(domain, domain != Interval(0), !in(0, domain));
}

ExpressionProperties Expr_Identifier::properties(/*SymbolTable*/){
    Interval domain = getRef()->getInterval();
    return ExpressionProperties(domain, domain != Interval(0), !in(0, domain));
}

ExpressionProperties Expr_Add::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    return ExpressionProperties(lhsConstraint + rhsConstraint);
}

ExpressionProperties Expr_Sub::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval res = lhsConstraint - rhsConstraint;
    return ExpressionProperties(lhsConstraint - rhsConstraint);
}

ExpressionProperties Expr_Mul::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    return ExpressionProperties(lhsConstraint * rhsConstraint);
}

ExpressionProperties Expr_Div::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    return ExpressionProperties(lhsConstraint / rhsConstraint);
}

void IfElse::solveConstraints(/*SymbolTable*/){
    // Hier könnte auch ein Vergleich stattfinden. Was dann?
    // Dieser schränkt u.U. nur den Bereich einer Variable ein.
    ExpressionProperties prop = condition->properties();
    if(isJust(prop.satisfiable)){
        if(fromJust(prop.satisfiable)){
            std::cout << "if accessible" << std::endl;
            if(fromJust(prop.tautology)){
                std::cout << "if always fulfilled" << std::endl; // move then-block up
            }
        }
        else{
            std::cout << "if not accessible" << std::endl; // move else-block up or eleminate completely
        }
    }
    else{
        std::cout << "if: " << "condition has no logic properties!" << std::endl;
    }
}
