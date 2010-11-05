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
  list<Variable*>* arguments;
  Statement* statement;
  Func_abi abi;

  public:
      Function(string* identifier, Func_abi abi, list<Variable*>* arguments, Statement* statement) : arguments(arguments), abi(abi), statement(statement) {
          this->identifier = *identifier;
      }

      void dump(int num) {
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
	
	void dump(int num)
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
  list<Statement*> substatements;
};

/** Classic IfElse block. */
class IfElse : public Statement {
  Expression* condition;
  Statement* then;
  Statement* otherwise;
};
/** Classic While block. */
class While : public Statement {
  Expression* condition;
  Statement* loop;
};
/** Returns from outermost function. */
class Return : public Statement {
  Expression* expr;
};

