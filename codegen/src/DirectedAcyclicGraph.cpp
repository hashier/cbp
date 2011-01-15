#include "DirectedAcyclicGraph.h"

namespace DAG {
    void DirectedAcyclicGraph::buildDAG() {
        for (std::list<Statement *>::iterator it = statements->begin(); it != statements->end(); it++)
        {
            //(*it)->
        }
    }

    Node *DirectedAcyclicGraph::addToDAG(Node *left, Node *right, Operator op)
    {
        return new InnerNode(left, right, op);
    }

    Node *DirectedAcyclicGraph::addToDAG(int value)
    {
        Node *node = NULL;
        if (!( node = map.getNode(value)))
        {
            node = new ConstLeafNode(value);
            map.insertNode(value, node);
        }
        return node;
    }

    Node *DirectedAcyclicGraph::addToDAG(std::string &name)
    {
        Node *node = NULL;
        if (!( node = map.getNode(name)))
        {
            LeafNode *node = new IdentifierLeafNode(name);
            map.insertNode(name, node);
        }
        return node;
    }


}
