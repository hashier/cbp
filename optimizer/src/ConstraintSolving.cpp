#include "ConstraintSolving.h"

#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "Variables.h"
#include "AbstractNodes.h"
#include "ExpressionProperties.h"
#include "ConstrainedEnvironment.h"

#include <iostream>

void optimizeTree_ConstraintSolving(File* file) {
    std::cout << " - Constraint Solving" << std::endl;
    ConstrainedEnvironment env;
    file->propagateParent(0);
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
    assert(false);
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
        // avoid iterator invalidation -> dereference and go back
        Statement* st = *subStatementIter;
        --subStatementIter;
        st->solveConstraints(env);
        ++subStatementIter; // go forth
        // check if child has changed and restart iteration if changed
        // or go back if already at the end so that increment does not
        // exceed the range.
        if(subStatementIter == subStatementEnd || *subStatementIter != st){
            --subStatementIter;
        }
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
    return ExpressionProperties(domain, domain != Interval(0ULL), !in(0, domain));
}

// --- Variables ---
ExpressionProperties Expr_Identifier::properties(ConstrainedEnvironment& env){
    Interval domain = env.constrain(getRef());
    //std::cout << "var expr (" << getRef()->getIdentifier() << "): " << domain << std::endl;
    return ExpressionProperties(domain, domain != Interval(0ULL), !in(0, domain));
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

// Helper for calculating comparison constraints
bool atom(Expression* node){
    return dynamic_cast<Atom*>(node) != 0;
}

typedef std::pair<Variable*, Interval> ChangedVariable;

void insertIfVariable(ExpressionProperties::ChangedVariables& changes,
    Expression* node, Interval const& i){
    Expr_Identifier* var = dynamic_cast<Expr_Identifier*>(node);
    if(var != 0){
        changes.insert(ChangedVariable(var->getRef(), i));
    }
}

ExpressionProperties::ChangedVariables
restrictEquality(ConstrainedEnvironment& env, Expression* lhs, Expression* rhs, bool invert = false){
    ExpressionProperties::ChangedVariables changes;

    // only changes if both operands are variables/constants
    if(atom(lhs) && atom(rhs)){
        Interval lhsConstraint = lhs->properties(env).interval;
        Interval rhsConstraint = rhs->properties(env).interval;
        Interval restrictLhs;
        Interval restrictRhs;
        if(invert){
            // nothing to do for !=
            restrictLhs = lhsConstraint;
            restrictRhs = rhsConstraint;
        }
        else{
            restrictLhs = lhsConstraint & rhsConstraint;
            restrictRhs = restrictLhs;
            std::cout << "equality restriction: " << restrictLhs << std::endl;
        }
        
        insertIfVariable(changes, lhs, restrictLhs);
        insertIfVariable(changes, rhs, restrictRhs);
    }
    return changes;
}

ExpressionProperties::ChangedVariables
restrictLess(ConstrainedEnvironment& env, Expression* lhs, Expression* rhs, bool invert = false){
    ExpressionProperties::ChangedVariables changes;
    
    // only changes if both operands are variables/constants
    if(atom(lhs) && atom(rhs)){
        Interval lhsConstraint = lhs->properties(env).interval;
        Interval rhsConstraint = rhs->properties(env).interval;
        Interval restrictLhs;
        Interval restrictRhs;
        if(invert){
            restrictLhs = restrictRight(lhsConstraint, rhsConstraint);
            restrictRhs = restrictLeft(lhsConstraint, rhsConstraint);
        }
        else{
            restrictLhs = restrictLeft(lhsConstraint, rhsConstraint);
            restrictRhs = restrictRight(lhsConstraint, rhsConstraint);
            std::cout << "less restriction: " << restrictLhs << std::endl;
        }
        
        insertIfVariable(changes, lhs, restrictLhs);
        insertIfVariable(changes, rhs, restrictRhs);
    }
    return changes;
}

// --- Comparision ---
ExpressionProperties Expr_EQ::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables equalConstraints
        = restrictEquality(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables notEqualConstraints
        = restrictEquality(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
        
    return ExpressionProperties(
        equalConstraints, notEqualConstraints,
        ((lhsConstraint & rhsConstraint) != Interval()),   // satisfiable if there are overlaps
        (lhsConstraint.singleton() && (lhsConstraint == rhsConstraint)) // only a tautology if both are [a,a]
    );
}

ExpressionProperties Expr_NEQ::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables equalConstraints
        = restrictEquality(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables notEqualConstraints
        = restrictEquality(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    // dual to equality
    return ExpressionProperties(
        notEqualConstraints, equalConstraints,
        !(lhsConstraint.singleton() && (lhsConstraint == rhsConstraint)),   // satisfiable whenever intervals are not both [a,a]
        ((lhsConstraint & rhsConstraint) == Interval()) // tautology if intervals are distinct
    );
}

ExpressionProperties Expr_LT::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables lessConstraints
        = restrictLess(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables greaterConstraints
        = restrictLess(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    return ExpressionProperties(
        lessConstraints, greaterConstraints,
        lhsConstraint.lower() < rhsConstraint.upper(),
        lhsConstraint.upper() < rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GT::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables lessConstraints
        = restrictLess(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables greaterConstraints
        = restrictLess(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    return ExpressionProperties(
        greaterConstraints, lessConstraints,
        lhsConstraint.upper() > rhsConstraint.lower(),
        lhsConstraint.lower() > rhsConstraint.upper()
    );
}

ExpressionProperties Expr_LE::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables lessConstraints
        = restrictLess(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables greaterConstraints
        = restrictLess(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    return ExpressionProperties(
        lessConstraints, greaterConstraints,
        lhsConstraint.lower() <= rhsConstraint.upper(),
        lhsConstraint.upper() <= rhsConstraint.lower()
    );
}

ExpressionProperties Expr_GE::properties(ConstrainedEnvironment& env){
    ExpressionProperties::ChangedVariables lessConstraints
        = restrictLess(env, getLeft(), getRight());
    ExpressionProperties::ChangedVariables greaterConstraints
        = restrictLess(env, getLeft(), getRight(), true);
        
    Interval lhsConstraint = getLeft()->properties(env).interval;
    Interval rhsConstraint = getRight()->properties(env).interval;
    
    return ExpressionProperties(
        greaterConstraints, lessConstraints,
        lhsConstraint.upper() >= rhsConstraint.lower(),
        lhsConstraint.lower() >= rhsConstraint.upper()
    );
}

// --- If-Else ---
void IfElse::solveConstraints(ConstrainedEnvironment& env){
    ExpressionProperties prop = condition->properties(env);
    if(isJust(prop.satisfiable)){
        // the predicate may be true
        if(fromJust(prop.satisfiable)){
            std::cout << "if accessible" << std::endl;
            // the predicate is always true
            if(fromJust(prop.tautology)){
                std::cout << "if: then always accessed" << std::endl;
                env.startBlock(prop.changesFulfilled);
                then->solveConstraints(env);
                env.endBlock();
                
                // move then-block up
                std::cout << "moving then-block up and dropping else-block" << std::endl;
                getParent()->replaceChild(this, then);
            }
            else{
                env.startBlock(prop.changesFulfilled);
                then->solveConstraints(env);
                env.endBlock();
                if(otherwise != 0){
                    env.startBlock(prop.changesNotFulfilled);
                    otherwise->solveConstraints(env);
                    env.endBlock();
                }
            }
        }
        // the predicate will never be true
        else{
            std::cout << "if: then not accessible" << std::endl;
            if(otherwise != 0){
                env.startBlock(prop.changesNotFulfilled);
                otherwise->solveConstraints(env);
                env.endBlock();
                
                // move else-block up
                std::cout << "moving else-block up and dropping then-block" << std::endl;
                getParent()->replaceChild(this, otherwise);
            }
            else{
                // eliminate "if" completely
                std::cout << "dropping if-block under " << typeid(*getParent()).name() << std::endl;
                getParent()->replaceChild(this, 0);
            }
        }
    }
    // Should not happen (type error)
    else{
        std::cout << "if: " << "condition has no logic properties!" << std::endl;
    }
}

void WhileLoop::solveConstraints(ConstrainedEnvironment& env){
    ExpressionProperties prop = condition->properties(env);
    if(isJust(prop.satisfiable)){
        // the predicate may be true
        if(fromJust(prop.satisfiable)){
            std::cout << "while accessible" << std::endl;
            
            env.startBlock(prop.changesFulfilled);
            body->solveConstraints(env);
            env.endBlock();
            
            // predicate must be false after while finished
            ExpressionProperties::ChangedVariables::iterator changedIter = prop.changesNotFulfilled.begin();
            for(; changedIter != prop.changesNotFulfilled.end(); ++changedIter){
                env.update(changedIter->first, changedIter->second);
            }
        }
        // the predicate will never be true
        else{
            std::cout << "while not accessible" << std::endl;
            // eliminate while completely
            std::cout << "dropping while-block" << std::endl;
            getParent()->replaceChild(this, 0);
        }
    }
    // Should not happen (type error)
    else{
        std::cout << "while: " << "condition has no logic properties!" << std::endl;
    }
}

