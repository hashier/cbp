#ifndef EXPRESSIONPROPERTIES_H
#define EXPRESSIONPROPERTIES_H

#include "Intervals.h"
#include "Maybe.h"

struct ExpressionProperties {
    ExpressionProperties()
        : interval(Interval::world()), satisfiable(nothing()), tautology(nothing()) {}

    // --- Constraints ---
	// The interval which results from the expression
	// (if the operation may produce absurd results: [-∞,+∞])
    Interval interval;
    explicit ExpressionProperties(Interval const& id)
        : interval(id), satisfiable(nothing()), tautology(nothing()) {}
    
    // --- Logical ---
    // Just if accessible for this expression and
    // true if the predicate is satisfiable.
    Maybe<bool> satisfiable;
    Maybe<bool> tautology;
    ExpressionProperties(bool sat, bool taut)
        : interval(Interval::world()), satisfiable(sat), tautology(taut) {}
        
    ExpressionProperties(Interval const& id, bool sat, bool taut)
        : interval(id), satisfiable(sat), tautology(taut) {}
};

#endif // EXPRESSIONPROPERTIES_H
