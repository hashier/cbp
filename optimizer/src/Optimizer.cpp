#include "Optimizer.h"

#include "Inlining.h"
#include "ConstantPropergation.h"

void optimizeTree(File* file)
{
    //do constant propergation
    //optimizeTree_ConstantPropergation(file);

    //do inlining
    optimizeTree_Inlining(file);
}

