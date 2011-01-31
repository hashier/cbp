#include "Optimizer.h"

#include "Inlining.h"
#include "ConstantPropergation.h"
#include "ConstraintSolving.h"

void optimizeTree(File* file)
{
    //do constant propergation
    //optimizeTree_ConstantPropergation(file);
    optimizeTree_ConstraintSolving(file);

    //do inlining
    optimizeTree_Inlining(file);
}

