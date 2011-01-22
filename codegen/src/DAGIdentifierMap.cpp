#include "DAGIdentifierMap.h"

namespace DAG {

    void IdentifierMap::insertNode(int value, Node *node)
    {
        constantIntMap.insert(std::pair<int, Node *>(value, node));
    }

    void IdentifierMap::insertNode(std::string &name, Node *node)
    {
        identifierMap.insert(std::pair<std::string, Node *>(name, node));
        initialIdentifiers.push_back(node);
    }

    Node *IdentifierMap::getNode(std::string &identifier) {
        std::map<std::string, Node *>::iterator it;
        it = identifierMap.find(identifier);
        
        if (it == identifierMap.end()) {
            return NULL;
        }
        else {
            return it->second;
        }
    }

    void IdentifierMap::moveIdentifier(std::string &identifier, Node *node) {
        identifierMap.erase (identifier);
        identifierMap.insert(std::pair<std::string, Node *>(identifier, node));
    }

    Node *IdentifierMap::getNode(int value) {
        std::map<int, Node *>::iterator it;
        it = constantIntMap.find(value);
        
        if (it == constantIntMap.end()) {
            return NULL;
        }
        else {
            return it->second;
        }
    }

    void IdentifierMap::dumpAll()
    {
        for (std::map<int, Node *>::iterator it = constantIntMap.begin(); it != constantIntMap.end(); it++)
        {
            (*it).second->dump();
        }
        for (std::map<std::string, Node *>::iterator it = identifierMap.begin(); it != identifierMap.end(); it++)
        {
            (*it).second->dump();
        }
        for (std::list<Node *>::iterator it = initialIdentifiers.begin(); it != initialIdentifiers.end(); it++)
        {
            (*it)->dump();
        }

    }

    std::string IdentifierMap::getMarks(Node *node)
    {
        std::string result = "";
        for (std::map<std::string, Node *>::iterator it = identifierMap.begin(); it != identifierMap.end(); it++)
        {
            if ((*it).second == node)
                result.append((*it).first).append(" ");
        }
        return result;
    }
}
