#include<list>
#include<vector>
#include "ExprNodes.h"
#include <iostream>
#include <fstream>

#pragma once

namespace DAG {


    enum Operator {
        PLUS,
        MINUS,
        MULT,
        DIV,
        ASSIGN,
        TODO
    };



    class InnerNode;


    class Node {
    protected:
        std::list<InnerNode *> children;
        Node *parent1;
        Node *parent2;
        Expression *expr;
        static std::fstream dumpStream;
        bool dumped;
        Node *assignment;

    public:
        Node(Node *parent1, Node *parent2, Expression *expr)
            : parent1(parent1), parent2(parent2), expr(expr), dumped(false), assignment(NULL) { }

        std::vector<InnerNode *> GetOperatorNode(Operator op);    // kopie von liste?
        void AddNewOperatorNode(InnerNode *opNode);
        Expression *getExpression() { return expr; }

        virtual void dump() = 0;

        void SetAssignment(Node *node) { assignment = node; }
    };


    class InnerNode : public Node {
    private:
        Operator op;

    public:
        InnerNode(Node *parent1, Node *parent2, Operator op, Expression *expr);

        Operator GetOperator() { return op; }
        Node *Get1stOperand() { return parent1; }
        Node *Get2ndOperand() { return parent2; }

        void dump();

    };


    class LeafNode : public Node {
    public:
        LeafNode(Expression *expr) : Node(NULL, NULL, expr) { }
    };
    
    class ConstLeafNode : public LeafNode {
    public:
        ConstLeafNode(int value, Expression *expr) : LeafNode(expr), value(value) { }
        void dump();
    private:
        int value; 
    };

    class IdentifierLeafNode : public LeafNode {
    public:
        IdentifierLeafNode(std::string &name, Expression *expr) : LeafNode(expr), name(name) { }
        void dump();
    private:
        std::string name;
    };
    

}
