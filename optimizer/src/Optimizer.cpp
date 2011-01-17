#include "Optimizer.h"

#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <algorithm>
#include "ExprNodes.h"

void doInlining(Function* func, Function* subFunc)
{
    std::cout << "  -inline function call '" << subFunc->getIdentifier() << "' in function '" << func->getIdentifier() << "'" << std::endl;

    //TODO
}

//get list of all sub function called by a certain function
std::vector<FuncCall*> getSubFunctions(Function* func)
{
    //result list
    std::vector<FuncCall*> subFunctions;

    //is valid function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block==NULL) return subFunctions;

    //get all statements of current function
    std::list<Statement*> statements = block->getSubStatements();
    
    //for each function
    for(std::list<Statement*>::iterator it=statements.begin(); it!=statements.end(); ++it)
    {
        //statement to explore
        Statement* statement = *it;

        //check if statement is function
        FuncCall* subFunction = dynamic_cast<FuncCall*>(statement);

        if(subFunction)
        {
            subFunctions.push_back(subFunction);
        }
    }

    return subFunctions;
}

unsigned int getStatementCount(Function* func)
{
    unsigned int counter = 0;

    //skip external functions by zero statements
    if(func->getAbi()!=Abi_default)
        return 0;

    //is valid function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block==NULL) return 0;

    return block->getSubStatements().size();
}

void optimizeTree_Inlining(File* file, unsigned int depth)
{
    std::cout << " -Inlining" << std::endl;

    //get function list
    std::list<Function*>& functions = file->getFunctions();

    //calculate statement count for each function  
    std::vector<unsigned int> statementCounts;
    for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
    {
        statementCounts.push_back(getStatementCount(*it));
    }

    //do inlining as long as there are no 
    for(unsigned int depth_i=0; depth_i<depth; depth_i++)
    {
        //for each function
        for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
        {
            //current function
            Function* func = *it;

            //get list of all function calls
            std::vector<FuncCall*> subFunctions = getSubFunctions(func);

            //for each function call
            for(unsigned int i=0; i<subFunctions.size(); i++)
            {
                //get sub function
                FuncCall* subFuncCall = subFunctions[i];
                Function* subFunc = subFuncCall->getFunction();

                //skip external function calls
                if(subFunc->getAbi()!=Abi_default)
                    continue;

                //skip yourself - recursion
                if(subFunc==func)
                    continue;

                //get numbers of statements inside sub function
                //TODO: might crash if file structure is incorrect !!!
                unsigned int index = 0;
                for(std::list<Function*>::iterator it2=functions.begin(); it2!=functions.end(); ++it2)
                {
                    if(*it2==subFunc) break;
                    index++;
                }
                unsigned int statementCount = statementCounts[index];

                //if sub function is not heavy do inlining
                if(statementCount<10)
                {
                    doInlining(func, subFunc);
                }
            }
        }
    }

}

void optimizeTree(File* file)
{
    //do constant propergation
    optimizeTree_ConstantPropergation(file);

    //do inlining
    optimizeTree_Inlining(file, 100);

    //do xxx
    //optimizeTree_xxx(file);
}

