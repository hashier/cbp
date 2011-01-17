#pragma once

#include "ExprNodes.h"
#include "ProgramNodes.h"

void optimizeTree_ConstantPropergation(File* file);

void calcConstExpr(Expression* expr);
void calcConstExpr(Statement* stmt);
