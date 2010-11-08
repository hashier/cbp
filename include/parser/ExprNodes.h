#pragma once

#include"AbstractNodes.h"
#include"ProgramNodes.h"

#include<string>
#include<iostream>
#include<typeinfo>

// Basic types of expressions
class Unary : public Expression {
  Expression* sub;

  public:
    Unary(Expression* sub) : sub(sub) {
    }

    void dump(int num = 0) {
      indent(num); std::cout << "Unary: " << typeid(*this).name() << std::endl;
      sub->dump(num);
    }
};

class Binary : public Expression {
  Expression* left;
  Expression* right;

  public:
    Binary(Expression* left, Expression* right) : left(left), right(right) {
    }

    void dump(int num = 0) {
      indent(num); std::cout << "Binary: " << typeid(*this).name() << std::endl;

      indent(num); std::cout << "Left:" << std::endl;
      left->dump(num+1);

      indent(num); std::cout << "Right:" << std::endl;
      right->dump(num+1);
    }
};

class Expr_Assign : public Expression {
  public:
    Expr_Assign(Expression *expr) : assigned(expr) {}

    virtual void dump(int num = 0) {
      indent(num); std::cout << "Assigned:" << std::endl;
      if(assigned != NULL) {
        assigned->dump(num + 1);
      }
    }
  private:
    Expression *assigned;
};

class Expr_Cast : public Expression {
  public:
    Expr_Cast(Type *_castType) : castType(_castType) {}

    virtual void dump(int num = 0) {
      indent(num); std::cout << "Cast as:" << std::endl;
      if(castType != NULL) {
        castType->dump(num + 1);
      }
    }
  private:
    Type *castType;
};

// Precedence 9
class Expr_EQ : public Binary {
  public: Expr_EQ(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_NEQ : public Binary {
  public: Expr_NEQ(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_LT : public Binary {
  public: Expr_LT(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_GT : public Binary {
  public: Expr_GT(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_LE : public Binary {
  public: Expr_LE(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_GE : public Binary {
  public: Expr_GE(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 8
class Expr_BoolOR : public Binary {
  public: Expr_BoolOR(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_BoolAND : public Binary {
  public: Expr_BoolAND(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_BoolXOR : public Binary {
  public: Expr_BoolXOR(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 7
class Expr_BitLeft : public Binary {
  public: Expr_BitLeft(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_BitRight : public Binary {
  public: Expr_BitRight(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 6
class Expr_BitOR : public Binary {
  public: Expr_BitOR(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_BitAND : public Binary {
  public: Expr_BitAND(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_BitXOR : public Binary {
  public: Expr_BitXOR(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 5
class Expr_Add : public Binary {
  public: Expr_Add(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_Sub : public Binary {
  public: Expr_Sub(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 4
class Expr_Mul : public Binary {
  public: Expr_Mul(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_Div : public Binary {
  public: Expr_Div(Expression* left, Expression* right) : Binary(left, right) { }
};
class Expr_Mod : public Binary {
  public: Expr_Mod(Expression* left, Expression* right) : Binary(left, right) { }
};

// Precedence 3
class Expr_Ptr : public Unary {
  public: Expr_Ptr(Expression* sub) : Unary(sub) { }
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

  public:
    ConstInt(int value) : value(value) { }

    void dump(int num = 0) {
      indent(num);
      std::cout << "Const Int: " << value << std::endl;
    }
};
class ConstFloat : public Constant {
  float value;

  public:
    ConstFloat(float value) : value(value) { }

    void dump(int num = 0) {
      indent(num);
      std::cout << "Const Float: " << value << std::endl;
    }
};
class Identifier : public Atom {
  Variable* ref;

  void dump(int num = 0) {
    indent(num); std::cout << "Const Identifier:" << std::endl;
    ref->dump(num+1);
  }
};
class FuncCall : public Atom {
  std::string identifier;
  Function* func;
  std::list<Expression*> exprs;

  public:

    FuncCall(std::string* identifier, std::list<Expression*>* exprs) {
      // is this safe?
      this->identifier = *identifier;
    }

    void dump(int num = 0) {
      indent(num); std::cout << "Function Call: " << identifier << std::endl;
    }

};
