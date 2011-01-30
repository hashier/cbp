#include "DirectedAcyclicGraph.h"
#include "Variables.h"

namespace DAG {

    Node *DirectedAcyclicGraph::addToDAG(Node *left, Node *right, Expression *expr)
    {
        assert (left != 0 && right != 0);
        std::vector<InnerNode *> leftOps = left->getOperatorNodes(expr);
        std::vector<InnerNode *> rightOps = right->getOperatorNodes(expr);

        Node *resultNode = NULL;

        // test if there is already a InnerNode with left, right and op
        for (unsigned int i = 0; i < leftOps.size(); ++i)
        {
            for (unsigned int j = 0; j < rightOps.size(); ++j)
            {
                // found common InnerNode
                if (leftOps[i] == rightOps[j])
                {
                    // okay, we have a InnerNode with both operands, but we have to check 
                    // if they are in the right order! 23 / 42 != 42 / 23
                    if (leftOps[i]->getLeftOperand() == left)
                    {
                        resultNode = leftOps[i];
                        break;
                    }
                    // maybe we have a funny commutative operator, like + and *,
                    // then we can ignore the order. 23 + 42 == 42 + 23
                    else if(typeid(*expr) == typeid(Expr_Add) || typeid(*expr) == typeid(Expr_Mul))
                    {
                        resultNode = leftOps[i];
                        break;
                    }
                }
            }
            if (resultNode != NULL)
                break;
        }

        // don't create a new node if we have an assign expression
        if (resultNode == NULL && typeid(*expr) != typeid(Expr_Assign))
            resultNode = new InnerNode(left, right, expr);

        // if we have an assign, set the identifier of the left part to the right node
        if (typeid(*expr) == typeid(Expr_Assign))
        {
            Expr_Assign * assign = dynamic_cast<Expr_Assign *>(expr);
            assert (assign != NULL);
            Expr_Identifier * identifier = dynamic_cast<Expr_Identifier *>(assign->getLeft());

            // we need an assignment with a single identifier on the left side! (should always be the case)
            assert(identifier != NULL); 

            map.moveIdentifier(identifier->getRef()->getIdentifier(), right);

            return right;
        }

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
