#pragma once

#include"AbstractNodes.h"

#include<string>
#include<iostream>

using namespace std;

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
  string identifier;
  Type type;

  public:
    Variable(string* identifier, Type type) : identifier(*identifier), type(type) {
    }

    void dump(int num) {
        indent(num); cout << "Variable: " << endl;
    }
};

class Function : public Node {
  string identifier;
  Func_abi abi;
  list<Variable*>* arguments;
  Statement* statement;

  public:
      Function(string* identifier, Func_abi abi, list<Variable*>* arguments, Statement* statement) : abi(abi), arguments(arguments), statement(statement) {
          this->identifier = *identifier;
      }

      void dump(int num = 0) {
          indent(num); cout << "Function: " << identifier << endl;;
          indent(num); cout << "Statement: ";
          if(statement)
              statement->dump(num+1);
          else
              cout << "NULL" << endl;
      }
};

class TypeDecl : public Node {
	public:
		TypeDecl(string* identifier, Type type)
			: identifier(identifier), type(type) { }

	void dump(int num = 0)
	{
		indent(num); cout << "Type declaration: " << *identifier << " of type " << type << endl;;
	}

	private:
		string *identifier;
		Type type;
};

/** A file holds exactly one Program.
 * It consists of global definitions for types, variables
 * and functions.
 */
class File {
  list<Type*> types;
  list<Variable*> variables;
  list<Function*> functions;
};

/** Groups a number of statements. */
class Block : public Statement {
  list<Statement*> subs;

  public:
    void add(Statement* st) {
      subs.push_back(st);
    }

    void dump(int num = 0) {
      indent(num); cout << "{" << endl;

      list<Statement*>::iterator it;
      for ( it = subs.begin() ; it != subs.end(); it++ ) {
        (*it)->dump(num+1);
      }

      indent(num); cout << "}" << endl;
    }
};

/** Classic IfElse block. */
class IfElse : public Statement {
  Expression* condition;
  Statement* then;
  Statement* otherwise;
};
/** Classic While block. */
class WhileLoop : public Statement {
  Expression* condition;
  Statement* loop;

  public:
    WhileLoop(Expression* condition, Statement* loop) : condition(condition), loop(loop) {
    }

    void dump(int num = 0) {
      indent(num); cout << "While Loop" << endl;
      indent(num); cout << "Condition:" << endl;
      condition->dump(num+1);
      indent(num); cout << "Loop:" << endl;
      loop->dump(num+1);
    }
};
/** Returns from outermost function. */
class Return : public Statement {
  Expression* expr;
};
