#include "Optimizer.h"

#include "Inlining.h"
#include "ConstantPropergation.h"
#include "ConstraintSolving.h"

void optimizeTree(File* file)
{
    //do constant propergation
    //optimizeTree_ConstantPropergation(file);

#ifndef NOINLINE
    //do inlining
    optimizeTree_Inlining(file);
#else
    std::cout << " inlining disabled" << std::endl;
#endif

#ifndef NOCONST
    // after inlining to have variables on lhs
    //optimizeTree_ConstraintSolving(file);
#else
    std::cout << " Const Prop disabled" << std::endl;
#endif
}

