#include "DirectedAcyclicGraph.h"
#include "Variables.h"

namespace DAG {

    Node *DirectedAcyclicGraph::addToDAG(Node *left, Node *right, Operator op, Expression *expr)
    {
        assert (left != 0 && right != 0);
        std::vector<InnerNode *> leftOps = left->getOperatorNodes(op);
        std::vector<InnerNode *> rightOps = right->getOperatorNodes(op);

        Node *resultNode = NULL;

        // test if there is already a InnerNode with left, right and op
        for (unsigned int i = 0; i < leftOps.size(); ++i)
        {
            for (unsigned int j = 0; j < rightOps.size(); ++j)
            {
                if (leftOps[i] == rightOps[j])
                {
                    resultNode = leftOps[i];
                    break;
                }
            }
            if (resultNode != NULL)
                break;
        }

        // don't create a new node if we have an assign expression
        if (resultNode == NULL && op != ASSIGN)
            resultNode = new InnerNode(left, right, op, expr);

        // if we have an assign, set the identifier of the left part to the right node
        if (op == ASSIGN)
        {
            Expr_Assign * assign = dynamic_cast<Expr_Assign *>(expr);
            assert (assign != NULL);
            Expr_Identifier * identifier = dynamic_cast<Expr_Identifier *>(assign->getLeft());

            // we need an assignment with a single identifier on the left side!
            assert(identifier != NULL); 

            map.moveIdentifier(identifier->getRef()->getIdentifier(), right);

            return right;
        }

        // if not, create a new one
        return resultNode;
    }

    Node *DirectedAcyclicGraph::addToDAG(int value, Expression *expr)
    {
        Node *node = NULL;
        if (!(node = map.getNode(value)))
        {
            node = new ConstLeafNode(value, expr);
            map.insertNode(value, node);
        }
        return node;
    }

    Node *DirectedAcyclicGraph::addToDAG(std::string &name, Expression *expr)
    {
        Node *node = NULL;
        if (!(node = map.getNode(name)))
        {
            node = new IdentifierLeafNode(name, expr);
            map.insertNode(name, node);
        }
        return node;
    }

    void DirectedAcyclicGraph::dumpAll()
    {
        Node::setIndentifierMap(&map);
        map.dumpAll();
    }

}
