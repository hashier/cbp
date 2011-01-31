#pragma once

#include "Mnemonics.h" // for class Label
#include "AbstractNodes.h"
#include "Types.h"

#include <string>
#include <iostream>
#include <memory>

class ConstInt;
class Variable;

enum Func_abi {
    Abi_c,
    Abi_default
};

class Function : public Declaration {
    public:
        Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Type* type, Statement* statement = NULL);
        void dump(int num = 0);
        void constProp();
		virtual void solveConstraints(/*SymbolTable*/);
        virtual void gen(CodeGen* out);
        Type* getType() { return type; }
        Label getMark(CodeGen* out);
        Statement* getStatement();
        std::list<Variable*>* getArguments();
        const Func_abi &getAbi() const { return abi; }
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
        void constProp() { };
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
        std::list<Function*>& getFunctions();
        void dump(int num = 0);
        void constProp();
		virtual void solveConstraints();
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
        Block();
        void add(Statement* st);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual void solveConstraints(/*SymbolTable*/);
        inline void gen(CodeGen* out) {
            this->gen(out, false);
        }
        void gen(CodeGen* out, bool outermost);
        std::list<Statement*> getSubStatements();
        std::list<Statement*>& getSubStatementsRef();
        virtual int calcStackOffset(int offset);
        virtual DAG::Node *addToDAG(DAG::DirectedAcyclicGraph *graph);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
        void insertAfter(Statement* where, Statement* item);
        virtual ~Block();
    private:
        std::list<Statement*> subs;
        DAG::DirectedAcyclicGraph *dag;
};

/** Classic IfElse block. */
class IfElse : public Statement {
    public:
        IfElse(Expression* condition, Statement* then, Statement* otherwise);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
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
        void constProp();
        bool isConst();
        constant* getConstant();
        void gen(CodeGen* out);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
    private:
        Expression* which;
        std::list<Case*>* cases;
};

/** Classic While block. */
class WhileLoop : public Statement {
    public:
        WhileLoop(Expression* condition, Statement* body);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual ~WhileLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
    private:
        Expression* condition;
        Statement* body;
};

/** Returns from outermost function. */
class Return : public Statement {
    public:
        Return(Expression* expr);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual ~Return();
        inline virtual void gen(CodeGen* out) {
            this->gen(out, false);
        }
        void gen(CodeGen* out, bool outermost);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
        Expression* getExpr() { return expr; };
    private:
        Expression* expr;
};

class Local : public Statement {
    public:
        Local(Variable* var);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        /** Sets the memory offset of the wrapped Variable and returns its size. */
        int calcStackOffset(int offset);
        virtual void gen(CodeGen* out);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
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
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual ~ForLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node*> getChildNodes();
        virtual void replaceChild(Node* currentChild, Node* newChild);
    private:
        Variable* iterator;
        Expression* init_value;
        Expression* final_value;
        Expression* step;
        Statement* body;
};

class GotoLabel : public Statement {
public:
    GotoLabel();
    void dump(int num = 0);
    void constProp();
    bool isConst();
    constant* getConstant();
    virtual ~GotoLabel();
    virtual void gen(CodeGen* out);
    virtual int calcStackOffset(int offset);
    virtual std::vector<Node*> getChildNodes();
    virtual void replaceChild(Node* currentChild, Node* newChild);
    Label* getLabel() { return label; };
    void genLabel(CodeGen* out);
private:
    Label* label;
};

class Goto : public Statement {
public:
    explicit Goto(GotoLabel* gotoLabel);
    void dump(int num = 0);
    void constProp();
    bool isConst();
    constant* getConstant();
    virtual ~Goto();
    virtual void gen(CodeGen* out);
    virtual int calcStackOffset(int offset);
    virtual std::vector<Node*> getChildNodes();
    virtual void replaceChild(Node* currentChild, Node* newChild);
    GotoLabel* getGotoLabel() {return gotoLabel;}

private:
    GotoLabel* gotoLabel;
};
