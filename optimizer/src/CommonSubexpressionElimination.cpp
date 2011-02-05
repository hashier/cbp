#include "CommonSubexpressionElimination.h"
#include "ProgramNodes.h"

void optimizeTree_CommonSubexpressionElimination(File* file)
{
    std::cout << " -Elimination" << std::endl;

    //get list of all function in source file
    std::vector<Function**> functions = file->getFunctions();

    for (std::vector<Function**>::iterator it = functions.begin(); it != functions.end(); it++)
    {
        (**it)->searchAndReplaceBlocks();
    }

}
