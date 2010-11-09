#pragma once

#define YYDEBUG 1

#include<list>
#include<iostream>

#include"SymbolTable.h"

// Abstract superclasses a

class Node {
public:
    virtual void dump(int num = 0) = 0;

    static void indent(int num) {
      for(int i = 0; i < num; i++)
        std::cout << "\t";
    }

    static void initSymbolTable() {
        symbolTable = new SymbolTable();
    }

protected:
    static SymbolTable *symbolTable;
};

class Statement : public Node {
};

// Expressions, as in things that can be evaluated to numbers (at runtime)
class Expression : public Statement {
};

class Type : public Node {
};
