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

class Variable : public Node {
  std::string identifier;
  Type *type;

  public:
    
    Variable(){
    }

    Variable(std::string* identifier, Type *type) : identifier(*identifier), type(type) {
    }

    virtual void dump(int num = 0) {
        indent(num); std::cout << "Variable '" << identifier << "': " << std::endl;
        type->dump(num+1);
    }

    std::string getIdentifier(){
        return identifier;
    }
    
    Type* getType(){
        return type;
    }

};

class VariableInStruct : public Variable {
  std::string identifier;
  Type *type;
  int offset;

  public:
    VariableInStruct(Variable* variable, int offset = -1) : identifier(variable->getIdentifier()), type(variable->getType()) {
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
		TypeDecl(std::string* identifier, Type *type)
			: identifier(identifier), type(type) { }

	void dump(int num = 0)
	{
		indent(num); std::cout << "Type declaration: " << *identifier << " of type " << std::endl;
		type->dump(num+1);
	}

	private:
		std::string *identifier;
		Type *type;
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

class TypeStruct : public Type {
	std::list<Variable*>* typeStruct;

	public:
	TypeStruct(std::list<Variable*>* typeStruct) : typeStruct(typeStruct) {
	}
	void dump(int num = 0) {
		indent(num); dumpStructsMembers(num);
	}

	private:
	void dumpStructsMembers(int num) {
		std::cout << "Struct" << std::endl;
		std::list<Variable*>::iterator it;
		if ( typeStruct ) {
			for ( it = typeStruct->begin() ; it != typeStruct->end(); it++ ) {
				(*it)->dump(num+1);
			}
		}
	}
};

/** Wrapper class for the Type Enum */
class TypeSimple : public Type {
	SimpleTypeEnum simpleType;

	public:
	TypeSimple(SimpleTypeEnum simpleType) : simpleType(simpleType) {
	}
	void dump(int num = 0) {
		indent(num); std::cout << printSimpleTypeEnum(simpleType) << std::endl;
	}

	static std::string printSimpleTypeEnum(SimpleTypeEnum x) {
		switch(x) {
			case Type_uint8:
				return "uint8";
			case Type_int8:
				return "int8";
			case Type_uint16:
				return "uint16";
			case Type_int16:
				return "int16";
			case Type_uint32:
				return "uint32";
			case Type_int32:
				return "int32";
			case Type_float32:
				return "float32";
			case Type_float64:
				return "float64";
			case Type_void:
				return "void";
		}
		return "SHOULDNTHAPPEN";
	}

};

class TypeArray : public Type {
	Type* type;

	public:
		TypeArray(Type *type): type(type) {}

	    void dump(int num = 0) {
	        indent(num); std::cout << "TypeArray: " << std::endl;
	        type->dump(num+1);
	    }
};

class TypeId : public Type {
	std::string identifier;

	public:
		TypeId(std::string *identifier) {
			this->identifier = *identifier;
		}

		void dump(int num = 0) {
			indent(num); std::cout << "TypeId: " << identifier << std::endl;
		}
};

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
