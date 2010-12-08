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

        virtual Type* getType();

    protected:
        Expression* left;
        Expression* right;
};

// Precedence 11
class Expr_Assign : public Binary {
    public: 
        Expr_Assign(Expression* left, Expression* right) : Binary(left, right) { 
        }

        virtual void gen(CodeGen* out);
};

// Precedence 10
class Expr_Cast : public Expression {
    public:
        Expr_Cast(Type *_castType, Expression* _expr) : castType(_castType), expr(_expr) {}

        virtual void dump(int num = 0);

        virtual void genLeft(CodeGen* out) {
            expr->genLeft(out);
        }
        virtual void gen(CodeGen* out) {
            expr->gen(out);
        }

        virtual Type* getType() {
            // Fun fact: Returning this is pretty much the entire functionality of Expr_Cast!
            return castType;
        }

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
class Expr_Ref : public Unary {
    public:
        Expr_Ref(Expression* sub) : Unary(sub) {
            type = new TypePointer(sub->getType());
        }

        virtual Type* getType() {
            return type;
        }

        virtual void gen(CodeGen* out);

    private:
        Type* type;
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

    virtual Type* getType() {
        // TODO implement this
        return NULL;
    }
};

class Expr_Deref : public Unary {
private:
    Expression* index;
public:
    Expr_Deref(Expression *sub) : Unary(sub), index(0) {
        if(typeid(*sub->getType()) != typeid(TypePointer))
            std::cerr << "Warning: Dereferencing non-pointer variable!" << std::endl;
    }
    Expr_Deref(Expression *sub, Expression *index) : Unary(sub), index(index) {
        if(typeid(*sub->getType()) != typeid(TypePointer))
            std::cerr << "Warning: Dereferencing non-pointer variable!" << std::endl;
    }

    virtual Type* getType() {
        TypePointer* pointedType = dynamic_cast<TypePointer*>(sub->getType());
        if(pointedType)
            return pointedType->getType();
        else
            return TypeVoid::getSingleton();
    }

    virtual void genLeft(CodeGen* out);
    virtual void gen(CodeGen* out);
};

// Precedence 1
class Atom : public Expression {
};
class Constant : public Atom {
};
class ConstInt : public Constant {
    int value;
    Type* type;

    public:
        ConstInt(int value) : value(value) {
            // TODO always 32 bit?
            type = new TypeSimple(Type_int32);
        }

        void dump(int num = 0) {
            indent(num);
            std::cout << "Const Int: " << value << std::endl;
        }

        virtual void gen(CodeGen* out);

        int val() const {
            return value;
        }

        virtual Type* getType() {
            return type;
        }

};
class ConstFloat : public Constant {
    float value;
    Type* type;

    public:
        ConstFloat(float value) : value(value) {
            // TODO always 32 bit?
            type = new TypeSimple(Type_float32);
        }

        void dump(int num = 0) {
            indent(num);
            std::cout << "Const Float: " << value << std::endl;
        }

        virtual Type* getType() {
            return type;
        }

};

class Expr_Identifier : public Atom {
private:
    Variable* ref;
public:
    Expr_Identifier(std::string *identifier);

    virtual void genLeft(CodeGen* out);
    virtual void gen(CodeGen* out);

    virtual Type* getType() {
        return ref->getType();
    }

    void dump(int num = 0); 
};

class FuncCall : public Atom {
    Function* func;
    std::list<Expression*>* arguments;

    public:
        FuncCall(std::string* identifier, std::list<Expression*>* arguments);

        virtual void gen(CodeGen *out);

        void dump(int num = 0);

        virtual Type* getType() {
            return func->getType();
        }

};
