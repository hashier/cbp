#pragma once

#include"AbstractNodes.h"
#include"Types.h"
#include "ExprNodes.h"

#include<string>
#include<iostream>
#include <memory>

enum Func_abi {
    Abi_c,
    Abi_default
};

class Variable;


class NodeType : public Node
{
    public:
        NodeType(BaseType baseType)
        {
            this->type = new TypeSimple(baseType);
        }

        NodeType(std::string *identifier);

        NodeType(NodeType *type_)
        {
            this->type = new TypeArray(type_->getType());
        }

        NodeType(std::list<Variable *>* members);

        void dump(int num = 0) {
            indent(num); std::cout << "Type: " << std::endl;
            if (type != NULL)
            {
                indent(num); std::cout << type->getString() << std::endl;
            }
            else
            {
                indent(num); std::cout << "TODO (undefined type)" << std::endl;
            }
        }

        Type *getType() { return type; }

        int getSize() {
            return type->getSize();
        }

        virtual ~NodeType() {
            //if (type) delete type; type = 0; // TODO: wird mehrfach referenziert...
        }

    protected:
        Type *type;
};

class Variable : public Declaration {

    public:
        Variable(std::string* identifier, NodeType *type) : Declaration(*identifier), type(type), offset(-1) {
            try 
            {
                symbolTable->insertDefinition(this);
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                std::cerr << "Error: Variable already declared in current scope: " << *identifier << std::endl;
            }
        }

        virtual void dump(int num = 0) {
            indent(num); std::cout << "Variable '" << identifier << "': " << std::endl;
            type->dump(num+1);
        }

        NodeType *getType() { return type; }

        int setStackOffset(int offset) {
            this->offset = offset -type->getSize();
            return type->getSize();
        }

        int getSize() {
            return type->getSize();
        }

        int getMemoryOffset() {
            return offset;
        }

        virtual void gen(CodeGen* out);

        virtual ~Variable() {
            //if (type) delete type; type = 0;
        }

    protected:
        NodeType *type;
        int offset;
};

/*
class VariableInStruct : public Variable {
    int offset;

    public:
    VariableInStruct(Variable* variable, int offset = -1) : Variable(&variable->getIdentifier(), variable->getType()) {
        this->offset = offset;
    }

    void dump(int num = 0) {
        indent(num); std::cout << "StructVariable '" << identifier  << "' @ " << offset << " : " << std::endl;
        type->dump(num+1);
    }
};
*/

class Function : public Declaration {

    public:
        Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Statement* statement = NULL) 
            : Declaration(*identifier), abi(abi), arguments(arguments), statement(statement), gotMark(false)
        { 
            try
            {
                symbolTable->insertDefinition(this);
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                std::cerr << "Error: Function already declared in current Scope: " << *identifier << std::endl;
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

        Mark getMark(CodeGen* out) {
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
        Mark mark;
        bool gotMark;

};

class TypeDecl : public Declaration {

    public:
        TypeDecl(std::string* identifier, NodeType *type)
            : Declaration(*identifier), type(type) 
        {
            try
            {
                symbolTable->insertDefinition(this); 
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                // TODO: the existing definition can also be something else
                std::cerr << "Error: Definition already exisits: " << std::endl;
                //std::cerr << "'" << *identifier << "' is redefined from '" << 
                //    dynamic_cast<SymbolTable::TypeDef *>(symbolTable->GetDefinition(*identifier))->getType()->getString()
                //    << "' to '" << type->getType()->getString() << "'." << std::endl;
            }
        }

        void dump(int num = 0)
        {
            indent(num); std::cout << "Type declaration: " << identifier << " of type " << std::endl;
            type->dump(num+1);
        }

        NodeType *getType() { return type; }

        virtual ~TypeDecl() {
            if (type) delete type; type = 0;
        }
    private:
        NodeType *type;
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

        virtual ~Block() {
            for (std::list<Statement*>::iterator it = subs.begin() ; it != subs.end(); it++ ) {
                delete (*it);
            }
        }

    protected:
        /** Calculates the total required */
        int calcStackOffsets();

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

            std::auto_ptr<ConstInt> condition;
            std::auto_ptr<Statement> action;
        };

        SwitchCase(Expression* condition_, std::list<Case*>* cases_)
            : condition(condition_),
            cases(cases_) { }

        ~SwitchCase(){
            std::list<Case*>::iterator caseIter = cases->begin();
            for(; caseIter != cases->end(); ++caseIter ){
                delete (*caseIter);
            }
        }

        void dump(int num = 0) {
            indent(num); std::cout << "SwitchCase {" << std::endl;
            num += 1;
            indent(num); std::cout << "Condition:" << std::endl;
            condition->dump(num + 1);
            std::list<Case*>::const_iterator caseIter = cases->begin();
            for(; caseIter != cases->end(); ++caseIter ){
                indent(num); std::cout << "Case" << std::endl;
                (*caseIter)->condition->dump(num + 1);
                indent(num); std::cout << "Action:" << std::endl;
                (*caseIter)->action->dump(num + 1);
            }
            num -= 1;
            indent(num); std::cout << "}" << std::endl;
        }

    private:
        std::auto_ptr<Expression> condition;
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
        inline int setStackOffset(int offset) {
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
                init_value(init_value),final_value(final_value),body(body) 
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
                        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << std::endl; 
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
                        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << std::endl; 
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

    private:
        Variable* iterator;
        Expression* init_value;
        Expression* final_value;
        Expression* step;
        Statement* body;

};
