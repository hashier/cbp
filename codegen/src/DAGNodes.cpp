#include "DAGNodes.h"
#include "DAGIdentifierMap.h"

namespace DAG {

    std::fstream Node::dumpStream("dagdump.txt", std::fstream::in | std::fstream::out);
    IdentifierMap *Node::iMap;

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
            dumpStream << "\"" << this << "\" [label=\"" << op2string[op] << "\\n" << iMap->getMarks(this) << "\"]" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << this << "\" -> \"" << *it << "\"" << std::endl;
                (*it)->dump();
            }
        }
        if (parent1) parent1->dump();
        if (parent2) parent2->dump();
    }

    void ConstLeafNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"" << value << "\\n" << iMap->getMarks(this) <<  "\"]" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << this << "\" -> \"" << *it << "\"" << std::endl;
                (*it)->dump();
            }
        }
    }

    void IdentifierLeafNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"" << name << "0\\n" << iMap->getMarks(this) <<  "\"]" << std::endl;

            for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
            {
                dumpStream << "\"" << this << "\" -> \"" << *it << "\"" << std::endl;
                if (!dumped) (*it)->dump();
            }
        }
    }
}
