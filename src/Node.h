#include<list>
#include<string>
#include<iostream>
#include<typeinfo>

using namespace std;

// Abstract superclasses

class Node {
  public: 
    virtual void dump() = 0;
    virtual void dump(int num) {
      indent(num);
      dump();
    }

    static void indent(int num) {
      for(int i = 0; i < num; i++)
        cout << " ";
    }
};

class Statement : public Node {
};

// Expressions, as in things that can be evaluated to numbers (at runtime)
class Expression : public Statement {
};

class Type : public Node {
  string identifier;

  void dump() {
    cout << "Type: " << identifier << endl;;
  }
};
class Variable : public Node {
  string identifier;
  Type type;
};
class Function : public Node {
  list<Variable> arguments;
  Statement* statement;
};

/** A file holds exactly one Program.
 * It consists of global definitions for types, variables
 * and functions.
 */
class File {
  list<Type> types;
  list<Variable> variables;
  list<Function> functions;
};

/** Groups a number of statements. */
class Block : public Statement {
  list<Statement> substatements;
};

/** Classic IfElse block. */
class IfElse : public Statement {
  Expression* condition;
  Statement* then;
  Statement* otherwise;
};
/** Classic While block. */
class While : public Statement {
  Expression* condition;
  Statement* loop;
};
/** Returns from outermost function. */
class Return : public Statement {
  Expression* expr;
};

// Basic types of expressions
class Unary : public Expression {
  Expression* sub;

  void dump(int num) {
    indent(num); cout << "Unary: " << typeid(*this).name() << endl;
    sub->dump(num);
  }
};
class Binary : public Expression {
  Expression* left;
  Expression* right;

  void dump(int num) {
    indent(num); cout << "Binary: " << typeid(*this).name() << endl;

    indent(num); cout << "Left:" << endl;
    left->dump(num+1);

    indent(num); cout << "Right:" << endl;
    right->dump(num+1);
  }
};

// Precedence 9
class Expr_EQ : public Binary {
};
class Expr_NEQ : public Binary {
};
class Expr_LT : public Binary {
};
class Expr_GT : public Binary {
};
class Expr_LE : public Binary {
};
class Expr_GE : public Binary {
};

// Precedence 8
class Expr_BoolOR : public Binary {
};
class Expr_BoolAND : public Binary {
};
class Expr_BoolXOR : public Binary {
};

// Precedence 7
class Expr_BitLeft : public Binary {
};
class Expr_BitRight : public Binary {
};

// Precedence 6
class Expr_BitOR : public Binary {
};
class Expr_BitAND : public Binary {
};
class Expr_BitXOR : public Binary {
};

// Precedence 5
class Expr_Add : public Binary {
};
class Expr_Sub : public Binary {
};

// Precedence 4
class Expr_Mul : public Binary {
};
class Expr_Div : public Binary {
};
class Expr_Mod : public Binary {
};

// Precedence 3
class Expr_Ptr : public Unary {
};

// Precedence 2
// struc? No idea..

// Precedence 1
class Atom : public Expression {
};
class Constant : public Atom {
};
class ConstInt : public Constant {
  int value;

  void dump() {
    cout << "Const Int: " << value << endl;
  }
};
class ConstFloat : public Constant {
  float value;

  void dump() {
    cout << "Const Float: " << value << endl;
  }
};
class Identifier : public Atom {
  Variable* ref;

  void dump(int num) {
    cout << "Const Identifier:" << endl;
    ref->dump(num+1);
  }
};
