#pragma once

#define YYDEBUG 1

#include<list>
#include<iostream>
#include<typeinfo>

#include"Exceptions.h"
#include"SymbolTable.h"
#include"CodeGen.h"

// Abstract superclasses a

class Node {
    public:
        virtual void dump(int num = 0) = 0;

        virtual void gen(CodeGen* out) {
            (*out) << Nothing(typeid(this).name());
        }

        static void indent(int num) {
            for(int i = 0; i < num; i++)
                std::cout << "\t";
        }

        virtual ~Node() { };

        static SymbolTable::SymbolTable *symbolTable;
};

class Statement : public Node {
    public:
        /** Recursively calculates stack offsets.
         * @param offset Current position on stack.
         * @return Total size required in entire subtree.
         */
        virtual int calcStackOffset(int offset) {
            return 0;
        }
};

class Type;
// Expressions, as in things that can be evaluated to numbers (at runtime)
class Expression : public Statement {
    public:
        virtual Type* getType() = 0;

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
};

// Declarations can be stored in the symbol table and then retrieved by identifier
class Declaration : public Node {
public:
    Declaration(std::string &identifier)
        : identifier(identifier) { }
    std::string &getIdentifier() { return identifier; }

    virtual ~Declaration() { }

protected:
    std::string identifier;
};

