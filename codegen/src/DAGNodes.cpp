#include "DAGNodes.h"

namespace DAG {

    std::fstream Node::dumpStream = std::fstream("D:/Studium/Compilerbaupraktikum/svn/vsbuild/dagdump.txt", std::fstream::in | std::fstream::out);

    std::string op2string[] = {
        "+",
        "-",
        "*",
        "/",
        "=",
        "?"
    };

    InnerNode::InnerNode(Node *parent1, Node *parent2, Operator op, Expression *expr)        
        : Node(parent1, parent2, expr),  op(op)
    {
        parent1->AddNewOperatorNode(this);
        parent2->AddNewOperatorNode(this);
    }

    std::vector<InnerNode *> Node::GetOperatorNode(Operator op) {
        std::vector<InnerNode *> opVector;

        for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
        {
            if ((*it)->GetOperator() == op)
            {
                opVector.push_back(*it);
            }
        }

        return opVector;
    }


    void Node::AddNewOperatorNode(InnerNode *opNode)
    {
        children.push_back(opNode);
    }


    void InnerNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"OP: " << op2string[op] << "\"]" << std::endl;
            if (parent1) dumpStream << "\"" << this->parent1 << "\" -> \"" << this << "\"" << std::endl;
            if (parent2) dumpStream << "\"" << this->parent2 << "\" -> \"" << this << "\"" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << *it << "\" -> \"" << this << "\"" << std::endl;
                (*it)->dump();
            }
        }
        if (parent1) parent1->dump();
        if (parent2) parent2->dump();
        if (assignment) 
        {   
            assignment->dump();
            if (!dumped) dumpStream << "\"" << assignment << "\" -> \"" << this << "\"" << std::endl;
        }
    }

    void ConstLeafNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"" << value << "\"]" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << *it << "\" -> \"" << this << "\"" << std::endl;
                (*it)->dump();
            }
        }
        if (assignment) 
        {
            assignment->dump();
            if (!dumped) dumpStream << "\"" << assignment << "\" -> \"" << this << "\"" << std::endl;
        }
    }

    void IdentifierLeafNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"" << name << "_0\"]" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << *it << "\" -> \"" << this << "\"" << std::endl;
                if (!dumped) (*it)->dump();
            }
        }
        if (assignment) 
        {
            assignment->dump();
            if (!dumped) dumpStream << "\"" << assignment << "\" -> \"" << this << "\"" << std::endl;
        }
    }
}
