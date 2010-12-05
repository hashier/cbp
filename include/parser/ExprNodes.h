#pragma once

#include"AbstractNodes.h"
#include"ProgramNodes.h"

#include<string>
#include<iostream>
#include<typeinfo>

class NodeType;

// Basic types of expressions
class Unary : public Expression {

    public:
        Unary(Expression* sub) : sub(sub) {
        }

        void dump(int num = 0) {
            indent(num); std::cout << "Unary: " << typeid(*this).name() << std::endl;
            sub->dump(num);
        }
    protected:
        Expression* sub;
};

class Binary : public Expression {
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

    protected:
        Expression* left;
        Expression* right;
};

// Precedence 11
class Expr_Assign : public Binary {
    public: 
        Expr_Assign(Expression* left, Expression* right) : Binary(left, right) { 
            // TODO typecheck, left side can only be a single IDENTIFIER node!
        }
};

// Precedence 10
class Expr_Cast : public Expression {
    public:
        Expr_Cast(Type *_castType, Expression* _expr) : castType(_castType), expr(_expr) {}

        virtual void dump(int num = 0);

    private:
        Type* castType;
        Expression* expr;
};

// Precedence 9
class Expr_EQ : public Binary {
    public: Expr_EQ(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_NEQ : public Binary {
    public: Expr_NEQ(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_LT : public Binary {
    public: Expr_LT(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_GT : public Binary {
    public: Expr_GT(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_LE : public Binary {
    public: Expr_LE(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_GE : public Binary {
    public: Expr_GE(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};

// Precedence 8
class Expr_BoolOR : public Binary {
    public: Expr_BoolOR(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_BoolAND : public Binary {
    public: Expr_BoolAND(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_BoolXOR : public Binary {
    public: Expr_BoolXOR(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};

// Precedence 7
class Expr_BitLeft : public Binary {
public:
    Expr_BitLeft(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_BitRight : public Binary {
public:
    Expr_BitRight(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};

// Precedence 6
class Expr_BitOR : public Binary {
public:
    Expr_BitOR(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_BitAND : public Binary {
public:
    Expr_BitAND(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_BitXOR : public Binary {
public:
    Expr_BitXOR(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};

// Precedence 5
class Expr_Add : public Binary {
public:
    Expr_Add(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};
class Expr_Sub : public Binary {
public:
    Expr_Sub(Expression* left, Expression* right) : Binary(left, right) { }
    virtual void gen(CodeGen* out);
};

// Precedence 4
class Expr_Mul : public Binary {
    public:
        Expr_Mul(Expression* left, Expression* right) : Binary(left, right) { }
        virtual void gen(CodeGen* out);
};
class Expr_Div : public Binary {
    public:
        Expr_Div(Expression* left, Expression* right) : Binary(left, right) { }
        virtual void gen(CodeGen* out);
};
class Expr_Mod : public Binary {
    public: 
        Expr_Mod(Expression* left, Expression* right) : Binary(left, right) { }
        virtual void gen(CodeGen* out);
};

// Precedence 3
class Expr_Ptr : public Unary {
    public: Expr_Ptr(Expression* sub) : Unary(sub) { }
};

// Precedence 2
class Expr_Struc : public Expression {
public: 
    Expr_Struc(Expression* sub, std::string *identifier);
    virtual void dump(int num = 0)
    {
        indent(num); std::cout << "Expr_Ptr:";
        // sub->dump(num);
    }
};

class Expr_Arr : public Unary {
private:
    Expression *index;
public:
    Expr_Arr(Expression *sub, Expression *index) : Unary(sub), index(index) { }
};

// struc? No idea..

// Precedence 1
class Atom : public Expression {
};
class Constant : public Atom {
};
class ConstInt : public Constant {
    int value;

    public:
        ConstInt(int value) : value(value) {
        }

        void dump(int num = 0) {
            indent(num);
            std::cout << "Const Int: " << value << std::endl;
        }

        virtual void gen(CodeGen* out);
        
        int val() const {
            return value;
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

class Variable;
class Expr_Identifier : public Atom {
private:
    Variable* ref;
public:
    Expr_Identifier(std::string *identifier);

    virtual void gen(CodeGen* out);

    void dump(int num = 0); 
};

class Function;
class FuncCall : public Atom {
    Function* func;
    std::list<Expression*> exprs;

    public:
        FuncCall(std::string* identifier, std::list<Expression*>* exprs);

        void dump(int num = 0);

};


};

