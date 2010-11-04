#pragma once

#include"AbstractNodes.h"

#include<string>
#include<iostream>

using namespace std;

class Type : public Node {
  string identifier;

  Type(string identifier) : identifier(identifier) {
  }

  void dump() {
	cout << "Type: " << identifier << endl;;
  }
};
class Variable : public Node {
  string identifier;
  Type* type;

  Variable(string identifier, Type* type) : identifier(identifier), type(type) {
  }
};
class Function : public Node {
  string identifier;
  list<Variable*>* arguments;
  Statement* statement;

  public:
      Function(string* identifier, list<Variable*>* arguments, Statement* statement) : arguments(arguments), statement(statement) {
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

