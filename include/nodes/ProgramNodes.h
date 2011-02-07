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
        virtual void solveConstraints(ConstrainedEnvironment& env);
        virtual void gen(CodeGen* out);
        Type* getType() { return type; }
        Label getMark(CodeGen* out);
        Statement* getStatement();
        std::list<Variable*>* getArguments();
        const Func_abi &getAbi() const { return abi; }
        virtual void searchAndReplaceBlocks();
        virtual ~Function();
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
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
        TypeDecl(TypeDecl *node);
        void dump(int num = 0);
        void constProp() { };
        Type* getType() { return type; }
        static Type* getDeclaredType(std::string *identifier);
        std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
        virtual Node* clone();
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
        std::vector<Function**> getFunctions();
        void dump(int num = 0);
        void constProp();
        virtual void solveConstraints(ConstrainedEnvironment& env);
        void gen(CodeGen* gen);
        std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
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
        virtual void solveConstraints(ConstrainedEnvironment& env);
        inline void gen(CodeGen* out) {
            this->gen(out, false);
        }
        void gen(CodeGen* out, bool outermost);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild);
        void insertBefore(Statement* where, Statement* item);
        void insertAfter(Statement* where, Statement* item);
        void erase(Statement* what);
        virtual Node* clone();
        DAG::Container *replaceBlock();
        virtual void searchAndReplaceBlocks() { assert(false); }
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
        virtual void solveConstraints(ConstrainedEnvironment& env);
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild);
        virtual Node* clone();
        virtual void searchAndReplaceBlocks();
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
                
            Case(Case const& other);
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
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild);
        virtual Node* clone();
        virtual void searchAndReplaceBlocks();
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
        virtual void solveConstraints(ConstrainedEnvironment& env);
        virtual ~WhileLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild);
        virtual Node* clone();
        virtual void searchAndReplaceBlocks();
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
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
        Expression* getExpr() { return expr; };
        virtual Node* clone();
        virtual void searchAndReplaceBlocks() { }
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
        virtual void solveConstraints(ConstrainedEnvironment& env);
        /** Sets the memory offset of the wrapped Variable and returns its size. */
        int calcStackOffset(int offset);
        virtual void gen(CodeGen* out);
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
        virtual Node* clone();
        virtual ~Local();
        virtual void searchAndReplaceBlocks() { }
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
        //ForLoop for clone
        ForLoop(Variable* iterator, Expression* init_value, Expression* final_value, Expression* step, Statement* body);
        void dump(int num = 0);
        void constProp();
        bool isConst();
        constant* getConstant();
        virtual ~ForLoop();
        virtual void gen(CodeGen* out);
        virtual int calcStackOffset(int offset);
        virtual std::vector<Node**> getChildren();
        virtual void replaceChild(Node* oldChild, Node* newChild);
        virtual Node* clone();
        virtual void searchAndReplaceBlocks();
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
    virtual std::vector<Node**> getChildren();
    virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
    Label* getLabel() { return label; };
    void genLabel(CodeGen* out);
    virtual Node* clone();
    virtual void searchAndReplaceBlocks() { }
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
    virtual std::vector<Node**> getChildren();
    virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
    GotoLabel* getGotoLabel() {return gotoLabel;}
    virtual Node* clone();
    virtual void searchAndReplaceBlocks() { }

private:
    GotoLabel* gotoLabel;
};
