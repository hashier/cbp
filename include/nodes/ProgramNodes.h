#pragma once

#include"AbstractNodes.h"
#include"Types.h"
// #include"ExprNodes.h"

#include<string>
#include<iostream>
#include <memory>

enum Func_abi {
    Abi_c,
    Abi_default
};

class Variable : public Declaration {

    public:
        Variable(std::string* identifier, Type* type) : Declaration(*identifier), type(type), offset(-1) {
        }

        virtual void dump(int num = 0) {
            indent(num); std::cout << "Variable '" << identifier << "': " << std::endl;
            // type->dump(num+1);
        }

        Type* getType() {
            return type;
        }

        virtual int setStackOffset(int offset) {
            this->offset = offset -getSize();
            return -getSize();
        }

        virtual int getMemoryOffset() {
            return offset;
        }

        virtual int getSize() {
            return type->getSize();
        }

        virtual void gen(CodeGen* out);

    protected:
        Type* type;
        int offset;
};

class GlobalVariable : public Variable {
    public:
        GlobalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
            try {
                symbolTable->insertGlobalDefinition(this);
            } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
                std::cerr << "Error: Variable already declared in current scope: " << *identifier << " " << e.what() << std::endl;
            }
        }
};

class LocalVariable : public Variable {
    public:
        LocalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
            try {
                symbolTable->insertDefinition(this);
            } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
                std::cerr << "Error: Variable already declared in current scope: " << *identifier << " " << e.what() << std::endl;
            }
        }
};

class StructVariable : public Variable {
    public:

        StructVariable(std::string* identifier, Type* type) : Variable(identifier, type), explicitOffset(-1) {
            this->offset = offset;
        }

        inline void setExplicitOffset(int explicitOffset) {
            this->explicitOffset = explicitOffset;
        }

        virtual int setStackOffset(int offset) {
            if(explicitOffset < 0)
                this->offset = offset -getSize();
            return -getSize();
        }

        virtual int getMemoryOffset() {
            return explicitOffset < 0 ? offset : explicitOffset;
        }

        void dump(int num = 0) {
            indent(num); std::cout << "StructVariable '" << identifier  << "' @ " << offset << " : " << std::endl;
            // type->dump(num+1);
        }

    private:
        int explicitOffset;
};

class Function : public Declaration {

    public:
        Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Type* type, Statement* statement = NULL)
            : Declaration(*identifier), abi(abi), arguments(arguments), statement(statement), type(type), gotMark(false)
        {
            try
            {
                symbolTable->insertDefinition(this);
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                std::cerr << "Error: Function already declared in current Scope: " << *identifier << " " << e.what() << std::endl;
            }
        }

        void dump(int num = 0) {
            indent(num); std::cout << "Function: " << identifier << std::endl;;
            indent(num); std::cout << "Statement: " << std::endl;
            if(statement)
                statement->dump(num+1);
            else
                std::cout << "NULL (only declaration)" << std::endl;
        }

        virtual void gen(CodeGen* out);

        Type* getType() {
            return type;
        }

        Label getMark(CodeGen* out) {
            if(!gotMark) {
                gotMark = true;
                mark = out->newMark(identifier);
            }
            return mark;
        }

        virtual ~Function() {
            if (arguments) delete arguments; arguments = 0;
            if (statement) delete statement; statement = 0;
        }

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
        TypeDecl(std::string* identifier, Type* type)
            : Declaration(*identifier), type(type)
        {
            try
            {
                symbolTable->insertDefinition(this);
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                // TODO: the existing definition can also be something else
                std::cerr << "Error: Definition already exisits: " << " " << e.what() << std::endl;
                //std::cerr << "'" << *identifier << "' is redefined from '" <<
                //    dynamic_cast<SymbolTable::TypeDef *>(symbolTable->GetDefinition(*identifier))->getType()->getString()
                //    << "' to '" << type->getType()->getString() << "'." << std::endl;
            }
        }

        void dump(int num = 0)
        {
            indent(num); std::cout << "Type declaration: " << identifier << " of type " << std::endl;
            // type->dump(num+1);
        }

        Type* getType() {
            return type;
        }

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
        File() {
        }

        void add(TypeDecl* type) {
            types.push_back(type);
        }
        void add(Variable* var) {
            variables.push_back(var);
        }
        void add(Function* func) {
            functions.push_back(func);
        }

        void dump(int num = 0) {
            indent(num); std::cout << "Type declarations:" << std::endl; {
                std::list<TypeDecl*>::iterator it;
                for ( it = types.begin() ; it != types.end(); it++ ) {
                    (*it)->dump(num+1);
                }
            }

            indent(num); std::cout << "Variable declarations:" << std::endl; {
                std::list<Variable*>::iterator it;
                for ( it = variables.begin() ; it != variables.end(); it++ ) {
                    (*it)->dump(num+1);
                }
            }

            indent(num); std::cout << "Function declarations:" << std::endl; {
                std::list<Function*>::iterator it;
                for ( it = functions.begin() ; it != functions.end(); it++ ) {
                    (*it)->dump(num+1);
                }
            }
        }

        void gen(CodeGen* gen);

        virtual ~File() {
            for (std::list<TypeDecl *>::iterator it = types.begin() ; it != types.end(); it++ ) {
                delete (*it);
            }
            for (std::list<Variable *>::iterator it = variables.begin() ; it != variables.end(); it++ ) {
                delete (*it);
            }
            for (std::list<Function *>::iterator it = functions.begin() ; it != functions.end(); it++ ) {
                delete (*it);
            }
        }

    private:
        std::list<TypeDecl*> types;
        std::list<Variable*> variables;
        std::list<Function*> functions;
};

/** Groups a number of statements. */
class Block : public Statement {

    public:
        void add(Statement* st) {
            subs.push_back(st);
        }

        void dump(int num = 0) {
            indent(num); std::cout << "{" << std::endl;

            std::list<Statement*>::iterator it;
            for ( it = subs.begin() ; it != subs.end(); it++ ) {
                (*it)->dump(num+1);
            }

            indent(num); std::cout << "}" << std::endl;
        }

        virtual void gen(CodeGen* out);

        virtual int calcStackOffset(int offset);

        virtual ~Block() {
            for (std::list<Statement*>::iterator it = subs.begin() ; it != subs.end(); it++ ) {
                delete (*it);
            }
        }

    private:
        std::list<Statement*> subs;

};

/** Classic IfElse block. */
class IfElse : public Statement {

    public:
        IfElse(Expression* condition, Statement* then, Statement* otherwise) : condition(condition), then(then), otherwise(otherwise) {
        }

        void dump(int num = 0) {
            indent(num); std::cout << "If" << std::endl;
            indent(num); std::cout << "Condition:" << std::endl;
            condition->dump(num+1);
            indent(num); std::cout << "Then:" << std::endl;
            then->dump(num+1);
            if(otherwise) {
                indent(num); std::cout << "ELSE" << std::endl;
                otherwise->dump(num+1);
            }
        }

        virtual void gen(CodeGen* out);

        virtual int calcStackOffset(int offset);

    private:
        Expression* condition;
        Statement* then;
        Statement* otherwise;
};

class ConstInt;
/** Switch-Case-Block. */
class SwitchCase : public Statement {

    public:
        struct Case {
            Case(ConstInt* condition_, Statement* action_)
                : condition(condition_),
                action(action_) {}

            std::auto_ptr<ConstInt> condition;
            std::auto_ptr<Statement> action;
        };

        SwitchCase(Expression* which_, std::list<Case*>* cases_)
            : which(which_),
            cases(cases_) { }

        ~SwitchCase(){
            std::list<Case*>::iterator caseIter = cases->begin();
            for(; caseIter != cases->end(); ++caseIter ){
                delete (*caseIter);
            }
        }

        void dump(int num = 0);

        void gen(CodeGen* out);

    private:
        std::auto_ptr<Expression> which;
        std::auto_ptr<std::list<Case*> > cases;

};

/** Classic While block. */
class WhileLoop : public Statement {

    public:
        WhileLoop(Expression* condition, Statement* body) : condition(condition), body(body) {
        }

        void dump(int num = 0) {
            indent(num); std::cout << "While Loop" << std::endl;
            indent(num); std::cout << "Condition:" << std::endl;
            if(body==NULL) return;
            indent(num); std::cout << "Body:" << std::endl;
            body->dump(num+1);
        }

        virtual ~WhileLoop() {
            if (condition) delete condition; condition = 0;
            if (body) delete body; body = 0;
        }

        virtual void gen(CodeGen* out);

        virtual int calcStackOffset(int offset);

    private:
        Expression* condition;
        Statement* body;

};
/** Returns from outermost function. */
class Return : public Statement {

    public:
        Return(Expression* expr) : expr(expr) {
        }

        void dump(int num = 0) {
            indent(num); std::cout << "Return" << std::endl;
            if(expr==NULL) return;
            indent(num); std::cout << "expr:" << std::endl;
            expr->dump(num+1);
        }

        virtual ~Return() {
            if (expr) delete expr; expr = 0;
        }

        virtual void gen(CodeGen* out);

    private:
        Expression* expr;

};

class Local : public Statement {

    public:
        Local(Variable* var) : var(var) {
        }

        void dump(int num = 0) {
            indent(num); std::cout << "Local Variable:" << std::endl;
            var->dump(num+1);
        }

        /** Sets the memory offset of the wrapped Variable and returns its size. */
        inline int calcStackOffset(int offset) {
            return var->setStackOffset(offset);
        }

        virtual void gen(CodeGen* out);

        virtual ~Local() {
            if (var) delete var; var = 0;
        }

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
        ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Statement* body) :
                init_value(init_value),final_value(final_value),step(0),body(body)
                {
                    try
                    {
                        iterator = dynamic_cast<Variable *>(symbolTable->getDefinition(*iteratorname));
                        if (iterator == NULL)
                        {
                            std::cerr << "Error: Iterator '" << *iteratorname << "'in For-Loop is not a variable." << std::endl;
                        }
                    }
                    catch (SymbolTable::DefinitionNotFoundException &e)
                    {
                        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << " " << e.what() << std::endl;
                    }
                }
        //ForLoop with step expression
        ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Expression* step, Statement* body) :
                init_value(init_value),final_value(final_value),step(step),body(body)
                {
                    try
                    {
                        iterator = dynamic_cast<Variable *>(symbolTable->getDefinition(*iteratorname));
                        if (iterator == NULL)
                        {
                            std::cerr << "Error: Iterator '" << *iteratorname << "'in For-Loop is not a variable." << std::endl;
                        }
                    }
                    catch (SymbolTable::DefinitionNotFoundException &e)
                    {
                        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << " " << e.what() << std::endl;
                    }
                }

        void dump(int num = 0) {
            indent(num); std::cout << "For Loop" << std::endl;
            indent(num); std::cout << "Iterator: " << iterator->getIdentifier() << std::endl;

            indent(num); std::cout << "InitValue:" << std::endl;
            init_value->dump(num+1);

            indent(num); std::cout << "FinalValue:" << std::endl;
            final_value->dump(num+1);

            if(step!=NULL) {
              indent(num); std::cout << "Step:" << std::endl;
              step->dump(num+1);
            }

            if(body==NULL) return;
            indent(num); std::cout << "Body:" << std::endl;
            body->dump(num+1);
        }

        virtual ~ForLoop() {
            // don't delete Variable* iterator here!
            if (init_value) delete init_value; init_value = 0;
            if (final_value) delete final_value; final_value = 0;
            if (step) delete step; step = 0;
            if (body) delete body; init_value = 0;
        }

        virtual void gen(CodeGen* out);

        virtual int calcStackOffset(int offset);

    private:
        Variable* iterator;
        Expression* init_value;
        Expression* final_value;
        Expression* step;
        Statement* body;

};
