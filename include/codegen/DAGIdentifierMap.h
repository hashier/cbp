#include <map>
#include "DAGNodes.h"

#pragma once

namespace DAG {

    class IdentifierMap {
    private:
        std::map<std::string, Node*> identifierMap;
        std::map<int, Node*> constantIntMap;
        std::list<Node *> initialIdentifiers;

    public:
        void insertNode(int value, Node *node);
        void insertNode(std::string &name, Node *node);

        Node *getNode(int value);
        Node *getNode(std::string &identifier);
        void moveIdentifier(std::string &identifier, Node *node);

        std::map<std::string, Node*> *getIdentifierMap() { return &identifierMap; }

        void dumpAll();

        // horrible runtime, only for dumps!
        std::string getMarks(Node *node);
        
    };

}
