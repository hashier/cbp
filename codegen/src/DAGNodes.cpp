#include "DAGNodes.h"
#include "DAGIdentifierMap.h"

namespace DAG {

    std::fstream Node::dumpStream("dagdump.txt", std::fstream::in | std::fstream::out);
    IdentifierMap *Node::iMap;

    InnerNode::InnerNode(Node *leftParent, Node *rightParent, Expression *expr)        
        : Node(leftParent, rightParent, expr)
    {
        leftParent->addNewOperatorNode(this);
        rightParent->addNewOperatorNode(this);
    }

    std::vector<InnerNode *> Node::getOperatorNodes(Expression *expr) {
        std::vector<InnerNode *> opVector;

        for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
        {
            if (typeid(*(*it)->getExpression()) == typeid(*expr))
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

    void Node::gen(CodeGen *out)
    {
        if (leftParent)
            leftParent->gen(out);
        
        if (rightParent)
            rightParent->gen(out);
        
        expr->gen(out);
    }

    void InnerNode::dump()
    {
        if (!dumped)
        {
            dumped = true;
            dumpStream << "\"" << this << "\" [label=\"" << typeid(*expr).name() << "\\n" << iMap->getMarks(this) << "\"]" << std::endl;

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
