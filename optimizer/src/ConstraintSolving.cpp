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
    std::cout << "solving statement: " << typeid(*this).name() << std::endl;
}

void Expression::solveConstraints(/*SymbolTable*/){
    std::cout << "!!! solving expression: " << typeid(*this).name() << std::endl;
}

ExpressionProperties Expression::properties(/*SymbolTable*/){
    std::cout << "expression props: " << typeid(*this).name() << std::endl;
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

// --- Constant ---
ExpressionProperties ConstInt::properties(/*SymbolTable*/){
    Interval domain = Interval(static_cast<Interval::Integer>(val()));
    return ExpressionProperties(domain, domain != Interval(0L), !in(0, domain));
}

// --- Variables ---
ExpressionProperties Expr_Identifier::properties(/*SymbolTable*/){
    Interval domain = getRef()->getInterval();
    return ExpressionProperties(domain, domain != Interval(0L), !in(0, domain));
}

// --- Arithmetic ---
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

// --- Comparision ---
ExpressionProperties Expr_EQ::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval restrictLhs = lhsConstraint & rhsConstraint;
    Interval restrictRhs = restrictLhs;
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "equality restriction: " << restrictLhs << std::endl;
    return ExpressionProperties(
        ((lhsConstraint & rhsConstraint) != Interval()),   // satisfiable if there are overlaps
        (lhsConstraint.singleton() && (lhsConstraint == rhsConstraint)) // only a tautology if both are [a,a]
    );
}

ExpressionProperties Expr_NEQ::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    // nothing to do
    Interval restrictLhs = lhsConstraint;
    Interval restrictRhs = rhsConstraint;
    std::cout << "inequality no restriction" << std::endl;
    // dual to equality
    return ExpressionProperties(
        !(lhsConstraint.singleton() && (lhsConstraint == rhsConstraint)),
        ((lhsConstraint & rhsConstraint) == Interval())
    );
}

ExpressionProperties Expr_LT::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval restrictLhs = restrictLeft(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictRight(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "< restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.lower() < rhsConstraint.upper(),
        lhsConstraint.upper() < rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GT::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval restrictLhs = restrictRight(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictLeft(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "> restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.upper() > rhsConstraint.lower(),
        lhsConstraint.lower() > rhsConstraint.upper()
    );
}

ExpressionProperties Expr_LE::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval restrictLhs = restrictLeft(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictRight(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "<= restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.lower() <= rhsConstraint.upper(),
        lhsConstraint.upper() <= rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GE::properties(/*SymbolTable*/){
    Interval lhsConstraint = getLeft()->properties().interval;
    Interval rhsConstraint = getRight()->properties().interval;
    Interval restrictLhs = restrictRight(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictLeft(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << ">= restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.upper() >= rhsConstraint.lower(),
        lhsConstraint.lower() >= rhsConstraint.upper()
    );
}

// --- If-Else ---
void IfElse::solveConstraints(/*SymbolTable*/){
    // Hier könnte auch ein Vergleich stattfinden. Was dann?
    // Dieser schränkt u.U. nur den Bereich einer Variable ein.
    ExpressionProperties prop = condition->properties();
    // TODO: generate interval stack for variables
    if(isJust(prop.satisfiable)){
        if(fromJust(prop.satisfiable)){
            std::cout << "if accessible" << std::endl;
            if(fromJust(prop.tautology)){
                std::cout << "if: then always accessed" << std::endl; // TODO: move then-block up
                // TODO: populate stack to then branch
            }
            else{
                // TODO: populate stack to then and else branch
            }
        }
        else{
            std::cout << "if: then not accessible" << std::endl; // TODO: move else-block up or eliminate completely
            // TODO: populate stack to else branch
        }
    }
    else{
        std::cout << "if: " << "condition has no logic properties!" << std::endl;
    }
}
