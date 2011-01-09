#include "DAGIdentifierMap.h"

namespace DAG {

    Node *IdentifierMap::GetNode(std::string &identifier) {
        std::map<std::string, Node *>::iterator it;
        it = map.find(identifier);
        
        if (it == map.end()) {
            return NULL;
        }
        else {
            return it->second;
        }
    }

    void IdentifierMap::MoveIdentifier(std::string &identifier, Node *node) {
        map.erase (identifier);
        map.insert(std::pair<std::string, Node *>(identifier, node));
    }

}
