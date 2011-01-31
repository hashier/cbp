#include "Optimizer.h"

#include "Inlining.h"
#include "ConstantPropergation.h"
#include "ConstraintSolving.h"

#include <map>
#include <queue>
#include <cstring>

typedef void (*Optimizer) (File*);
typedef std::pair<unsigned, Optimizer> PrioritizedOptimizer;
typedef std::map<std::string, PrioritizedOptimizer> OptimizerMap;
typedef std::priority_queue<PrioritizedOptimizer> OptimizerQueue;

namespace {
    typedef std::pair<std::string, PrioritizedOptimizer> NamedOptimizer;
    // put new optimizer here
    NamedOptimizer optimizerInit[] = {
        NamedOptimizer("inlining", PrioritizedOptimizer(20, &optimizeTree_Inlining))
        , NamedOptimizer("constprop", PrioritizedOptimizer(10, &optimizeTree_ConstantPropergation))
        , NamedOptimizer("constraint", PrioritizedOptimizer(30, &optimizeTree_ConstraintSolving))
    };
    
    enum { optimizerInitSize = sizeof(optimizerInit) / sizeof(NamedOptimizer) };
    
    OptimizerMap optimizer(optimizerInit, optimizerInit + optimizerInitSize);
}

// selection of single optimizers via -opt <optimizer>
OptimizerQueue parseOptimizer(int argc_, char *argv[]){
    OptimizerQueue opts;
    
    if(argc_ < 0){
        std::cerr << "Invalid argument count" << std::endl;
        return opts;
    }
    
    std::size_t argc = static_cast<std::size_t>(argc_);
    
    for(std::size_t i = 1; i < argc; ++i){
        if(strcmp(argv[i], "-opt") == 0){
            i += 1;
            OptimizerMap::const_iterator opt = optimizer.find(argv[i]);
            if(opt != optimizer.end()){
                std::cout << "optimize: " << argv[i] << std::endl;
                opts.push(opt->second);
            }
            else{
                std::cerr << "Optimizer " << argv[i] << " not availble!" << std::endl;   
            }
        }
    }
    
    return opts;
}

void optimizeTree(File* file, int argc, char *argv[]){
    OptimizerQueue opts = parseOptimizer(argc, argv);
    while(!opts.empty()){
        PrioritizedOptimizer const& curr = opts.top();
        (*curr.second)(file);
        opts.pop();
    }
}

