#pragma once

#define YYDEBUG 1

#include<list>
#include<iostream>
#include<typeinfo>
#include<vector>

#include"Exceptions.h"
#include"SymbolTable.h"

#include"CodeGen.h"

struct ExpressionProperties;
class ConstrainedEnvironment;
namespace DAG { class Container; class DirectedAcyclicGraph; class Node; }

// needed for constant propergation
struct constant {
   int integer;
   float floating;
};

// Abstract superclasses a

class Node {
    public:
        // predefine lineNumber with 0
        Node()
            : lineNumber(0)
        {
        }
        // Use virtual attribute on this base-class destructor:
        // By deleting this class all destructors beginning on leaves are called,
        // also if a children don't have or have an overloaded destructor
        // with or without the virtual attribute.
        virtual ~Node() {
        }
        virtual void dump(int num = 0) = 0;
        
        virtual void constProp() = 0;
        virtual void solveConstraints(ConstrainedEnvironment& env) = 0;

        virtual void gen(CodeGen* out) {
            (*out) << Nothing(typeid(this).name());
        }

        static void indent(int num) {
            for(int i = 0; i < num; i++)
                std::cout << "\t";
        }

        // TODO: return the correct line number here
        int getLineNumber() const {
            return lineNumber;
        }
        
        void setLineNumber(int lineNumber) {
            this->lineNumber = lineNumber;
        }

        virtual std::vector<Node**> getChildren() {
            std::cerr << "getChildren missing in " << typeid(*this).name() << std::endl;
            assert(false);
            std::vector<Node**> a;
            return a;
        }
        
        virtual void replaceChild(Node* oldChild, Node* newChild) = 0;
        
        virtual Node* clone() {
            std::cerr << "clone missing in " << typeid(*this).name() << std::endl;
            assert(false);
            return 0;
        }
        
        Node* getParent() {
            return parentNode;
        }
        
        void setParent(Node* parent){
            parentNode = parent;
        }
        
        void propagateParent(Node* parent){
            this->setParent(parent);
            
            std::vector<Node**> children = getChildren();
            std::vector<Node**>::iterator childIter = children.begin();
            std::vector<Node**>::iterator childEnd = children.end();
            for(; childIter != childEnd; ++childIter){
                Node* child = **childIter;
                if(child != 0){
                    child->propagateParent(this);
                }
            }
        }   

        static SymbolTable::SymbolTable *symbolTable;
private:
        int lineNumber;
        Node* parentNode;
};

template<typename To, typename From>
To polymorphic_cast(From* x){
    To y = dynamic_cast<To>(x);
    assert(y != 0);
    return y;
}

namespace Nodes {
    /// \pre *children[i] == node for some i
    Node** findInChildren(std::vector<Node**> const& children, Node* node);
}

class Variable;
class Expression;

class Statement : public Node {
    public:
        virtual bool isConst() = 0;
        virtual constant* getConstant() = 0;

        virtual void solveConstraints(ConstrainedEnvironment& env);

        /** Recursively calculates stack offsets.
         * @param offset Current position on stack.
         * @return Total size required in entire subtree.
         */
        virtual int calcStackOffset(int offset) {
            return 0;
        }

        virtual void searchAndReplaceBlocks() {}
};

class Type;
// Expressions, as in things that can be evaluated to numbers (at runtime)
class Expression : public Statement {
    public:
        virtual Type* getType() = 0;
        
        virtual void solveConstraints(ConstrainedEnvironment& env);
        virtual ExpressionProperties properties(ConstrainedEnvironment& env);
        
        virtual void replaceChild(Node* oldChild, Node* newChild){
            std::cerr << "replaceChild missing in " << typeid(*this).name() << std::endl;
            assert(false);
        }

        /** This generates an expression's l-value.
         * Note that most expressions do not have an l-value, which is why
         * the default implementation is to throw an exception.
         * (Non-exhaustive) list of expressions which do have an l-value:
         *  - variable references
         *  - array access
         *  - struct access
         *  - pointer resolution
         *  - casts (trivially)
         */
        virtual void genLeft(CodeGen* out) {
            throw InvalidAssignmentException();
        }

        virtual DAG::Node *addToDAG(DAG::Container *graph) /* = 0; */ { std::cout << "todo: unimplemented addtodag-expression!" << std::endl; return NULL; }
};

// Declarations can be stored in the symbol table and then retrieved by identifier
class Declaration : public Node {
public:
    Declaration(std::string &identifier, int lineDefined)
        : identifier(identifier), lineDefined(lineDefined) { }
    Declaration(Declaration *decl)
        : identifier(decl->identifier), lineDefined(decl->lineDefined) { }
        
    std::string &getIdentifier() { return identifier; }
    int getLineDefined() { return lineDefined; }
    virtual void solveConstraints(ConstrainedEnvironment& env){}
    virtual ~Declaration() { }

protected:
    std::string identifier;
    int lineDefined;
};

