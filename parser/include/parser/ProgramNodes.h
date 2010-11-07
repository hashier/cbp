#pragma once

#include"AbstractNodes.h"

#include<string>
#include<iostream>

enum Type {
  Type_uint8,
  Type_int8,
  Type_uint16,
  Type_int16,
  Type_uint32,
  Type_int32,
  Type_float32,
  Type_float64,
  Type_void
};

enum Func_abi {
  Abi_c,
  Abi_default
};

class Variable : public Node {
  std::string identifier;
  Type type;

  public:
    Variable(std::string* identifier, Type type) : identifier(*identifier), type(type) {
    }

    void dump(int num) {
        indent(num); std::cout << "Variable: " << std::endl;
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
		TypeDecl(std::string* identifier, Type type)
			: identifier(identifier), type(type) { }

	void dump(int num = 0)
	{
		indent(num); std::cout << "Type declaration: " << *identifier << " of type " << type << std::endl;;
	}

	private:
		std::string *identifier;
		Type type;
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
      indent(num); std::cout << "IfElse" << std::endl;
      indent(num); std::cout << "Condition:" << std::endl;
      condition->dump(num+1);
      indent(num); std::cout << "Then:" << std::endl;
      then->dump(num+1);
      if(otherwise) {
        indent(num); std::cout << "Otherwise:" << std::endl;
        otherwise->dump(num+1);
      }
    }
};
/** Classic While block. */
class WhileLoop : public Statement {
  Expression* condition;
  Statement* loop;

  public:
    WhileLoop(Expression* condition, Statement* loop) : condition(condition), loop(loop) {
    }

    void dump(int num = 0) {
      indent(num); std::cout << "While Loop" << std::endl;
      indent(num); std::cout << "Condition:" << std::endl;
      condition->dump(num+1);
      indent(num); std::cout << "Loop:" << std::endl;
      loop->dump(num+1);
    }
};
/** Returns from outermost function. */
class Return : public Statement {
  Expression* expr;
};
