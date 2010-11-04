#pragma once

#include<list>
#include<iostream>

// Abstract superclasses

class Node {
  public:
    virtual void dump() {
      dump(0);
    }
    virtual void dump(int num) = 0;

    static void indent(int num) {
      for(int i = 0; i < num; i++)
        std::cout << "\t";
    }
};

class Statement : public Node {
};

// Expressions, as in things that can be evaluated to numbers (at runtime)
class Expression : public Statement {
};
