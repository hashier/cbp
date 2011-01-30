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

    InnerNode::InnerNode(Node *leftParent, Node *rightParent, Operator op, Expression *expr)        
        : Node(leftParent, rightParent, expr),  op(op)
    {
        leftParent->addNewOperatorNode(this);
        rightParent->addNewOperatorNode(this);
    }

    std::vector<InnerNode *> Node::getOperatorNodes(Operator op) {
        std::vector<InnerNode *> opVector;

        for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
        {
            if ((*it)->getOperator() == op)
            {
                opVector.push_back(*it);
            }
        }

        return opVector;
    }


    void Node::addNewOperatorNode(InnerNode *opNode)
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
        if (leftParent) leftParent->dump();
        if (rightParent) rightParent->dump();
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
