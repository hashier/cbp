#pragma once

#include "ExprNodes.h"
#include "ProgramNodes.h"

class ConstVar {
    public:
        ConstVar(std::string identifier, constant* value);
        std::string getIdentifier();
        constant* getValue();
        void setValue(constant* value);
    private:
        std::string identifier;
        constant* value;
};

extern std::list<std::list<ConstVar*>* > blocks;

void optimizeTree_ConstantPropergation(File* file);

void calcConstExpr(Expression** expr);
void calcConstExpr(Statement** stmt);