#ifndef CONSTRAINEDENVIRONMENT_H
#define CONSTRAINEDENVIRONMENT_H

#include "Intervals.h"
#include "Variables.h"
#include <map>
#include <list>

class ConstrainedEnvironment {
    typedef std::map<Variable*, Interval> ConstrainedVariables;
    typedef std::list<ConstrainedVariables> VariableStack;
    
    VariableStack variables;
    
public:
    ConstrainedEnvironment(){
        startBlock();
    }
    
    // starts a new block
    void startBlock() {
        variables.push_back(ConstrainedVariables());
    }
    
    // ends a block
    void endBlock() {
        variables.pop_back();
    }
    
    // Finds the innermost constrain for v
    Interval constrain(Variable* v) const {        
        VariableStack::const_reverse_iterator varIter = variables.rbegin();
        VariableStack::const_reverse_iterator varEnd = variables.rend();
        for(; varIter != varEnd; ++varIter){
            ConstrainedVariables::const_iterator var = varIter->find(v);
            if(var != varIter->end()){
                return var->second;
            }
        }
        
        return Interval();
    }
    
    // Updates the constrains for v in the innermost block
    void update(Variable* v, Interval const& constr){
        ConstrainedVariables::iterator var = variables.back().find(v);
        if(var != variables.back().end()){
            var->second = constr;
        }
        else{
            variables.back().insert(ConstrainedVariables::value_type(v, constr));
        }
    }
};

#endif // CONSTRAINEDENVIRONMENT_H

