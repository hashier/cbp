#pragma once

#include"AbstractNodes.h"
#include"Types.h"
#include "ExprNodes.h"

#include "VariableDef.h"

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

    NodeType(std::string *identifier)
    {
        SymbolTable::TypeDef *tdef = NULL;
        try
        {
            if (tdef = dynamic_cast<SymbolTable::TypeDef *>(symbolTable->GetDefinition(*identifier)))
            {
                this->type = tdef->getType();
            }
        }
        catch (SymbolTable::DefinitionNotFoundException &e)
        {
            std::cerr << "Error: Undefined Type: " << *identifier << std::endl;
        }
    }

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
            indent(num); std::cout << "TODO" << std::endl;
        }
    }

    Type *getType() { return type; }

protected:
    Type *type;
};

class Variable : public Node {
protected:
  std::string identifier;
  NodeType *type;

  public:
    
    Variable(){
    }

    Variable(std::string* identifier, NodeType *type) : identifier(*identifier), type(type) {
    }

    virtual void dump(int num = 0) {
        indent(num); std::cout << "Variable '" << identifier << "': " << std::endl;
        type->dump(num+1);
    }

    std::string &getIdentifier() { return identifier; }
    NodeType *getType() { return type; }
};

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

class Function : public Node {
  std::string identifier;
  Func_abi abi;
  std::list<Variable*>* arguments;
  Statement* statement;

  public:
      Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Statement* statement) : abi(abi), arguments(arguments), statement(statement) {
          this->identifier = *identifier;
      }

      void dump(int num = 0) {
          indent(num); std::cout << "Function: " << identifier << std::endl;;
          indent(num); std::cout << "Statement: ";
          if(statement)
              statement->dump(num+1);
          else
              std::cout << "NULL" << std::endl;
      }
};

class TypeDecl : public Node {
	public:
    TypeDecl(std::string* identifier, NodeType *type)
            : identifier(identifier), type(type) 
        {
            try
            {
                symbolTable->InsertDefinition(new SymbolTable::TypeDef(*identifier, type->getType())); 
            }
            catch (SymbolTable::DefinitionAlreadyExistsException &e)
            {
                // TODO: the existing definition can also be a VariableDef or a FunctionDef
                std::cerr << "Error: Definition already exisits: " << std::endl;
                std::cerr << "'" << *identifier << "' is redefined from '" << 
                    dynamic_cast<SymbolTable::TypeDef *>(symbolTable->GetDefinition(*identifier))->getType()->getString()
                    << "' to '" << type->getType()->getString() << "'." << std::endl;
            }
        }

	void dump(int num = 0)
	{
		indent(num); std::cout << "Type declaration: " << *identifier << " of type " << std::endl;
		type->dump(num+1);
	}

	private:
		std::string *identifier;
		NodeType *type;
};

/** A file holds exactly one Program.
 * It consists of global definitions for types, variables
 * and functions.
 */
class File {
  std::list<Type*> types;
  std::list<Variable*> variables;
  std::list<Function*> functions;
};

/** Groups a number of statements. */
class Block : public Statement {
  std::list<Statement*> subs;

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
};

/** Classic IfElse block. */
class IfElse : public Statement {
  Expression* condition;
  Statement* then;
  Statement* otherwise;

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
    
private:

    std::auto_ptr<Expression> condition;
    std::auto_ptr<std::list<Case*> > cases;

public:
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
};

/** Classic While block. */
class WhileLoop : public Statement {
  Expression* condition;
  Statement* body;

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
};
/** Returns from outermost function. */
class Return : public Statement {
  Expression* expr;

  public:
    Return(Expression* expr) : expr(expr) {
    }

    void dump(int num = 0) {
      indent(num); std::cout << "Return" << std::endl;
      if(expr==NULL) return;
	  indent(num); std::cout << "expr:" << std::endl;
      expr->dump(num+1);      
    }
};

class Local : public Statement {
	Variable* var;

	public:
  	  Local(Variable* var) : var(var) {
  	  }

  	  void dump(int num = 0) {
    	indent(num); std::cout << "Local Variable:" << std::endl;
    	var->dump(num+1);
  	  }
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
	std::string* iterator;
	Expression* init_value;
	Expression* final_value;
	Statement* body;

public:
	ForLoop(std::string* iterator, Expression* init_value, Expression* final_value, Statement* body) : iterator(iterator),init_value(init_value),final_value(final_value),body(body) {
	}

	void dump(int num = 0) {
		indent(num); std::cout << "For Loop" << std::endl;
		indent(num); std::cout << "Iterator: " << *iterator << std::endl;

		indent(num); std::cout << "InitValue:" << std::endl;
		init_value->dump(num+1);

		indent(num); std::cout << "FinalValue:" << std::endl;
		final_value->dump(num+1);

		if(body==NULL) return;
		indent(num); std::cout << "Body:" << std::endl;
		body->dump(num+1);
	}
};
