#include "Optimizer.h"

#include "Inlining.h"
#include "ConstantPropergation.h"
#include "ConstraintSolving.h"

void optimizeTree(File* file)
{
    //do constant propergation
    //optimizeTree_ConstantPropergation(file);
    
    //do inlining
    optimizeTree_Inlining(file);
    
    // after inlining to have variables on lhs
    optimizeTree_ConstraintSolving(file);
}

