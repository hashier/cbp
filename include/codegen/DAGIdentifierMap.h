#include <map>
#include "DAGNodes.h"

namespace DAG {

    class IdentifierMap {
    private:
        std::map<std::string, Node*> map;

    public:
        Node *GetNode(std::string &identifier);
        void MoveIdentifier(std::string &identifier, Node *node);

    };

}
