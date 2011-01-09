#include <list>
#include "ProgramNodes.h"

namespace DAG {

class DirectedAcyclicGraph {
public:
    DirectedAcyclicGraph(std::list<Statement*> *statements)
        : statements(statements) { }

    void BuildDAG();

private:
    std::list<Statement*> *statements;
};

}
