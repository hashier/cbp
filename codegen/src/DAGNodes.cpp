#include "DAGNodes.h"

namespace DAG {

    InnerNode *Node::GetOperatorNode(Operator op, Node *operand) {
        for (std::list<InnerNode *>::iterator it = children.begin(); it != children.end(); it++)
        {
            if ((*it)->GetOperator() == op && operand == (*it)->Get2ndOperand())
            {
                return (*it);
            }
        }

        // operator node mit entsprechenden operanden existiert noch nicht -> anlegen
        return AddNewOperatorNode(op, operand);

    }
}
