#include<list>
#include<vector>
#include "ExprNodes.h"
#include <iostream>
#include <fstream>

#pragma once

namespace DAG {


    class InnerNode;
    class IdentifierMap;


    class Node {
    protected:
        std::list<InnerNode *> children;
        Node *leftParent;
        Node *rightParent;
        Expression *expr;
        static std::fstream dumpStream;
        static IdentifierMap *iMap;

        //bool alreadyGenerated;
        bool dumped;

    public:
        Node(Node *leftParent, Node *rightParent, Expression *expr)
            : leftParent(leftParent), rightParent(rightParent), expr(expr), dumped(false) { }

        std::vector<InnerNode *> getOperatorNodes(Expression *expr);    // kopie von liste?
        void addNewOperatorNode(InnerNode *opNode);
        Expression *getExpression() { return expr; }


        static void setIndentifierMap(IdentifierMap *iMap) { Node::iMap = iMap; }
        virtual void dump() = 0;

        void gen(CodeGen* out);

    };


    class InnerNode : public Node {
    private:
        

    public:
        InnerNode(Node *leftParent, Node *rightParent, Expression *expr);

        //Expression getExpression() { return expr; }
        Node *getLeftOperand() { return leftParent; }
        Node *getRightOperand() { return rightParent; }

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
