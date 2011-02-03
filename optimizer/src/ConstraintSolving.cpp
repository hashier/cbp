#include "ConstraintSolving.h"

#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "Variables.h"
#include "AbstractNodes.h"
#include "ConstrainedEnvironment.h"

#include <iostream>

void optimizeTree_ConstraintSolving(File* file) {
    std::cout << " - Constraint Solving" << std::endl;
    ConstrainedEnvironment env;
    file->solveConstraints(env);
}

void File::solveConstraints(ConstrainedEnvironment& env){
    {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->solveConstraints(env);
        }
    }

    {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->solveConstraints(env);
        }
    }
}

void Statement::solveConstraints(ConstrainedEnvironment& env){
    std::cout << "solving statement: " << typeid(*this).name() << std::endl;
}

void Expression::solveConstraints(ConstrainedEnvironment& env){
    std::cout << "!!! solving expression: " << typeid(*this).name() << std::endl;
}

ExpressionProperties Expression::properties(ConstrainedEnvironment& env){
    std::cout << "expression props: " << typeid(*this).name() << std::endl;
    return ExpressionProperties();
}

void Function::solveConstraints(ConstrainedEnvironment& env){
    std::cout << "function" << std::endl;
    // check if forward decl
    if(statement != 0){
        statement->solveConstraints(env);
    }
}

void Block::solveConstraints(ConstrainedEnvironment& env){
    std::list<Statement*>::iterator subStatementIter = subs.begin();
    std::list<Statement*>::iterator subStatementEnd = subs.end();
    for(; subStatementIter != subStatementEnd; ++subStatementIter){
        (*subStatementIter)->solveConstraints(env);
    }
}

void Variable::solveConstraints(ConstrainedEnvironment& env){
    // here should nothing to be done (global var ???)
    std::cout << "variable: " << env.constrain(this) << std::endl;
}

void Local::solveConstraints(ConstrainedEnvironment& env){
    env.update(var, Interval::world());
}

void Expr_Assign::solveConstraints(ConstrainedEnvironment& env){
    Interval rhsConstraint = getRight()->properties(env).interval;
    std::cout << "assign: " << rhsConstraint;

    Expr_Identifier* lhs = dynamic_cast<Expr_Identifier*>(getLeft());
    if(lhs != 0){
        Variable* lhsVar = lhs->getRef();
        std::cout << " to " << lhsVar->getIdentifier() << "." << std::endl;
        env.update(lhsVar, rhsConstraint);
    }
    else {
        std::cout << " lhs unknown." << std::endl;
    }
}

// --- Constant ---
ExpressionProperties ConstInt::properties(ConstrainedEnvironment& env){
    Interval domain = Interval(static_cast<Interval::Integer>(val()));
    return ExpressionProperties(domain, domain != Interval(0L), !in(0, domain));
}

// --- Variables ---
ExpressionProperties Expr_Identifier::properties(ConstrainedEnvironment& env){
    Interval domain = env.constrain(getRef());
    //std::cout << "var expr (" << getRef()->getIdentifier() << "): " << domain << std::endl;
    return ExpressionProperties(domain, domain != Interval(0L), !in(0, domain));
}

// --- Arithmetic ---
ExpressionProperties Expr_Add::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    return ExpressionProperties(lhsConstraint + rhsConstraint);
}

ExpressionProperties Expr_Sub::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    Interval res = lhsConstraint - rhsConstraint;
    return ExpressionProperties(lhsConstraint - rhsConstraint);
}

ExpressionProperties Expr_Mul::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    return ExpressionProperties(lhsConstraint * rhsConstraint);
}

ExpressionProperties Expr_Div::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    //std::cout << "div: " << lhsConstraint << "/" << rhsConstraint << std::endl;
    return ExpressionProperties(lhsConstraint / rhsConstraint);
}

// --- Comparision ---
ExpressionProperties Expr_EQ::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    // TODO: save new interval for variables on stack (inside corresponding block)
    //ExpressionProperties::ChangedVariables changed;
    // only change if both operands are variables/constants
    if(dynamic_cast<Atom*>(getLeft()) != 0 && dynamic_cast<Atom*>(getRight()) != 0){
        Interval restrictLhs = lhsConstraint & rhsConstraint;
        Interval restrictRhs = restrictLhs;
        std::cout << "equality restriction: " << restrictLhs << std::endl;
    }
    
    return ExpressionProperties(
        ((lhsConstraint & rhsConstraint) != Interval()),   // satisfiable if there are overlaps
        (lhsConstraint.singleton() && (lhsConstraint == rhsConstraint)) // only a tautology if both are [a,a]
    );
}

ExpressionProperties Expr_NEQ::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
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

ExpressionProperties Expr_LT::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    Interval restrictLhs = restrictLeft(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictRight(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "< restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.lower() < rhsConstraint.upper(),
        lhsConstraint.upper() < rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GT::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    Interval restrictLhs = restrictRight(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictLeft(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "> restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.upper() > rhsConstraint.lower(),
        lhsConstraint.lower() > rhsConstraint.upper()
    );
}

ExpressionProperties Expr_LE::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    Interval restrictLhs = restrictLeft(lhsConstraint, rhsConstraint);
    Interval restrictRhs = restrictRight(lhsConstraint, rhsConstraint);
    // TODO: save new interval for variables on stack (inside corresponding block)
    std::cout << "<= restriction left: " << restrictLhs << " right:" << restrictRhs << std::endl;
    return ExpressionProperties(
        lhsConstraint.lower() <= rhsConstraint.upper(),
        lhsConstraint.upper() <= rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GE::properties(ConstrainedEnvironment& env){
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
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
void IfElse::solveConstraints(ConstrainedEnvironment& env){
    // Hier könnte auch ein Vergleich stattfinden. Was dann?
    // Dieser schränkt u.U. nur den Bereich einer Variable ein.
    ExpressionProperties prop = condition->properties(env);
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
