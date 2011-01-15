#include<list>
#include "ExprNodes.h"

#pragma once

namespace DAG {


    enum Operator {
        PLUS,
        MINUS,
        MULT,
        DIV,
        TODO
    };

    class InnerNode;

    class Node {

    protected:
        std::list<InnerNode *> children;
        Node *parent1;
        Node *parent2;

    public:
        Node(Node *parent1, Node *parent2)
            : parent1(parent1), parent2(parent2) { }

        InnerNode *GetOperatorNode(Operator op, Node *operand);    // durchsucht die children nach op

        InnerNode *AddNewOperatorNode(Operator op, Node *operand);  // sinnvoll?
        
    };

    class InnerNode : public Node {
    private:
        Operator op;

    public:
        InnerNode(Node *parent1, Node *parent2, Operator op)
            : Node(parent1, parent2),  op(op) { }

        Operator GetOperator() { return op; }
        Node *Get1stOperand() { return parent1; }
        Node *Get2ndOperand() { return parent2; }

    };

    class LeafNode : public Node {
    public:
        LeafNode() : Node(NULL, NULL) { }
    };
    
    class ConstLeafNode : public LeafNode {
    public:
        ConstLeafNode(int value) : value(value) { }
    private:
        int value; 
    };

    class IdentifierLeafNode : public LeafNode {
    public:
        IdentifierLeafNode(std::string &name) : name(name) { }
    private:
        std::string name;
    };
    

}
