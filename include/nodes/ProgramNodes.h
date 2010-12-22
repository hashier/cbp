#pragma once

#include "AbstractNodes.h"
#include "Types.h"

#include <string>
#include <iostream>
#include <memory>

class ConstInt;

enum Func_abi {
    Abi_c,
    Abi_default
};

class Variable : public Declaration {
    public:
        Variable(std::string* identifier, Type* type);
        virtual void dump(int num = 0);
        Type* getType() { return type; }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getMemoryOffset();
        virtual int getSize();
        virtual void gen(CodeGen* out);
    protected:
        Type* type;
        int offset;
};

class GlobalVariable : public Variable {
    public:
        GlobalVariable(std::string* identifier, Type* type);
};

class LocalVariable : public Variable {
    public:
        LocalVariable(std::string* identifier, Type* type);
};

class StructVariable : public Variable {
    public:
        StructVariable(std::string* identifier, Type* type);
        inline void setExplicitOffset(int explicitOffset) {
            this->explicitOffset = explicitOffset;
        }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getMemoryOffset();
        void dump(int num = 0);
    private:
        int explicitOffset;
};

class Function : public Declaration {
    public:
        Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Type* type, Statement* statement = NULL);
        void dump(int num = 0);
        virtual void gen(CodeGen* out);
        Type* getType() { return type; }
        Label getMark(CodeGen* out);
        virtual ~Function();
    private:
        Func_abi abi;
        std::list<Variable*>* arguments;
        Statement* statement;
        Type* type;
        Label mark;
        bool gotMark;
};

class TypeDecl : public Declaration {
    public:
        TypeDecl(std::string* identifier, Type* type);
        void dump(int num = 0);
        Type* getType() { return type; }
        static Type* getDeclaredType(std::string *identifier);
    protected:
        Type* type;
};

/** A file holds exactly one Program.
 * It consists of global definitions for types, variables
 * and functions.
 */
class File : public Node {
    public:
        File() {}
        void add(TypeDecl* type);
        void add(Variable* var);
        void add(Function* func);
        void dump(int num = 0);
        void gen(CodeGen* gen);
        virtual ~File();
    private:
        std::list<TypeDecl*> types;
        std::list<Variable*> variables;
        std::list<Function*> functions;
};

/** Groups a number of statements. */
class Block : public Statement {
    public:
        Block() {};
        void add(Statement* st);
        void dump(int num = 0);
        inline void gen(CodeGen* out) {
            this->gen(out, false);
        }
        void gen(CodeGen* out, bool outermost);
        virtual int calcStackOffset(int offset);
        virtual ~Block();
    private:
        std::list<Statement*> subs;
};

/** Classic IfElse block. */
class IfElse : public Statement {
    public:
        IfElse(Expression* condition, Statement* then, Statement* otherwise);
        void dump(int num = 0);
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
    private:
        Expression* condition;
        Statement* then;
        Statement* otherwise;
};

/** Switch-Case-Block. */
class SwitchCase : public Statement {
    public:
        struct Case {
            Case(ConstInt* condition_, Statement* action_)
                : condition(condition_),
                action(action_) {}

            ~Case();
            ConstInt* condition;
            Statement* action;
        };
        SwitchCase(Expression* which_, std::list<Case*>* cases_);
        ~SwitchCase();
        void dump(int num = 0);
        void gen(CodeGen* out);
    private:
        Expression* which;
        std::list<Case*>* cases;
};

/** Classic While block. */
class WhileLoop : public Statement {
    public:
        WhileLoop(Expression* condition, Statement* body);
        void dump(int num = 0);
        virtual ~WhileLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
    private:
        Expression* condition;
        Statement* body;
};

/** Returns from outermost function. */
class Return : public Statement {
    public:
        Return(Expression* expr);
        void dump(int num = 0);
        virtual ~Return();
        inline virtual void gen(CodeGen* out) {
            this->gen(out, false);
        }
        void gen(CodeGen* out, bool outermost);
    private:
        Expression* expr;
};

class Local : public Statement {
    public:
        Local(Variable* var);
        void dump(int num = 0);
        /** Sets the memory offset of the wrapped Variable and returns its size. */
        inline int calcStackOffset(int offset) {
            return var->setStackOffset(offset);
        }
        virtual void gen(CodeGen* out);
        virtual ~Local();
    private:
        Variable* var;
};


/*
   class TypeId : public NodeType {
   std::string identifier;

   public:
   TypeId(std::string *identifier) {
   this->identifier = *identifier;
   }

   void dump(int num = 0) {
   indent(num); std::cout << "TypeId: " << identifier << std::endl;
   }
   };
   */

/** Classic For Loop */
class ForLoop : public Statement {
    public:
        //ForLoop without step expression
        ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Statement* body);
        //ForLoop with step expression
        ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Expression* step, Statement* body);
        void dump(int num = 0);
        virtual ~ForLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
    private:
        Variable* iterator;
        Expression* init_value;
        Expression* final_value;
        Expression* step;
        Statement* body;
};

