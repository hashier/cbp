#ifndef EXPRESSIONPROPERTIES_H
#define EXPRESSIONPROPERTIES_H

#include "Intervals.h"
#include "Maybe.h"
#include "Variables.h"
#include <map>

struct ExpressionProperties {
    ExpressionProperties()
        : interval(Interval::world()), satisfiable(nothing()), tautology(nothing()) {}

    // --- Constraints ---
	// The interval which results from the expression
	// (if the operation may produce absurd results: [-∞,+∞])
    Interval interval;
    explicit ExpressionProperties(Interval const& id)
        : interval(id), satisfiable(nothing()), tautology(nothing()) {}
        
    // Constraints that we can be sure about if the condition (not) matches
    typedef std::map<Variable*, Interval> ChangedVariables;
    ChangedVariables changesFulfilled;
    ChangedVariables changesNotFulfilled;
    
    // --- Logical ---
    // Just if accessible for this expression and
    // true if the predicate is satisfiable.
    Maybe<bool> satisfiable;
    Maybe<bool> tautology;
    ExpressionProperties(ChangedVariables const& fulfilled, ChangedVariables const& notFulfilled,
        bool sat, bool taut)
        : interval(Interval::world()),
        changesFulfilled(fulfilled), changesNotFulfilled(notFulfilled),
        satisfiable(sat), tautology(taut) {}
        
    ExpressionProperties(Interval const& id, bool sat, bool taut)
        : interval(id), satisfiable(sat), tautology(taut) {}
};

#endif // EXPRESSIONPROPERTIES_H
