#include "Inlining.h"

#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <algorithm>
#include "ExprNodes.h"
#include "ProgramNodes.h"
#include "Variables.h"


std::vector<Statement*> getStatements(Function* func)
{
    std::vector<Statement*> result;

    //get all statements of host function
    Block* block = dynamic_cast<Block*>(*func->getChildren()[0]);
    if(block==NULL) return result;

    std::vector<Node**> list = block->getChildren();

    for(unsigned int i=0; i<list.size(); i++)
    {
        Statement* statement = dynamic_cast<Statement*>(*list[i]);
        if(statement!=NULL)
            result.push_back(statement);
    }

    return result;
}

void optimizeTree_traverseTree_ReplaceArguments(Statement* statement, Variable* arg, Expression* exp)
{
    //get all child nodes
    std::vector<Node**> childs = statement->getChildren();

    for(unsigned int i=0; i<childs.size(); i++)
    {
        //if child is a variable
        Expr_Identifier* var = dynamic_cast<Expr_Identifier*>(*childs[i]);
        if(var!=NULL)
        {
            //its the argument we are going to replace?
            if(var->getRef()==arg)
            {
                std::cout << "    -replace var '" << var->getRef()->getIdentifier() << "' at line " << var->getLineNumber() << std::endl; 
                //statement->replaceChild(var, exp);
                delete childs[i];
                *childs[i] = exp;
            }
            continue;
        }

        //if childs is a Statement
        Statement* child = dynamic_cast<Statement*>(*childs[i]);
        if(child==NULL) continue;
        optimizeTree_traverseTree_ReplaceArguments(child, arg, exp);
    }
}

void optimizeTree_traverseTree_ReplaceReturn(Block* block, Statement* parentStatement, GotoLabel* label, Expr_Identifier* var)
{
    //get all child nodes
    std::vector<Node**> childs = parentStatement->getChildren();

    for(unsigned int i=0; i<childs.size(); i++)
    {
        //if child is a return
        Return* ret = dynamic_cast<Return*>(*childs[i]);
        if(ret!=NULL)
        {
            std::cout << "    -replace return at line " << ret->getLineNumber() << std::endl;

            Expr_Assign* assignResult;

            //any return value here?
            if(ret->getExpr()!=NULL)
            {
                assignResult = new Expr_Assign(var, ret->getExpr());
            }else{
                assignResult = new Expr_Assign(var, new ConstInt(0));
            }
            assignResult->setLineNumber(-1);
            assignResult->getRight()->setLineNumber(-1);

            //replace return with assign result statement
            delete ret;
            *childs[i] = assignResult;

            //insert jump to end
            Goto* goto_ = new Goto(label);
            goto_->setLineNumber(-1);
            block->insertAfter(i+1, goto_);

            continue;
        }

        //if childs is a Statement
        Statement* child = dynamic_cast<Statement*>(*childs[i]);
        if(child==NULL) continue;

        //enter new block?
        Block* newBlock = dynamic_cast<Block*>(*childs[i]); 
        if(newBlock==NULL)
        {
            optimizeTree_traverseTree_ReplaceReturn(block, child, label, var);
        }else{
            optimizeTree_traverseTree_ReplaceReturn(newBlock, child, label, var);
        }
    }
}

void doInlining(Function* parentFunc, Statement* parentStatement, FuncCall* call)
{
    Block* parentBlock = (Block*)parentFunc->getStatement();
    std::vector<Node**> parentBlockChildren = parentBlock->getChildren();
    
    //get parent statement index in parent function block
    unsigned int parentStatementIndex=0;
    for(unsigned int i=0; i<parentBlockChildren.size(); i++)
    {
        Statement* statement = dynamic_cast<Statement*>(*parentBlockChildren[i]);
        if( (statement!=NULL) && (statement==parentStatement) )
        {
            parentStatementIndex = i;
            break;
        }
    }

//-----------------------------------------------------------------------------
//create new local variable for result

    //create unique name
    std::stringstream ssName;
    ssName << "InlineRet_" << parentBlock->getChildren().size(); //TODO: create unique pre/suffix

    //create local var
    std::string* varRetName = new std::string(ssName.str()); 
    LocalVariable* varRet = new LocalVariable(varRetName, call->getFunction()->getType());
    varRet->setLineNumber(-1); //means lines was created by optimizer
    Local* localVarRet = new Local(varRet);
    localVarRet->setLineNumber(-1); //means lines was created by optimizer

    //insert local var before function call
    parentBlock->insertBefore(parentStatementIndex, localVarRet);

//-----------------------------------------------------------------------------
//create copy of inline function

    //create copy of block
    Block* inlineBlock = (Block*)parentFunc->getStatement()->clone();
    inlineBlock->setLineNumber(-1);

    //add additional jump destination at the end for
    GotoLabel* labelEnd = new GotoLabel();
    labelEnd->setLineNumber(-1);
    inlineBlock->add(labelEnd);

//-----------------------------------------------------------------------------
//replace all references to parameters

    std::list<Variable*>* arguments = call->getFunction()->getArguments();
    std::list<Variable*>::iterator it_arg = arguments->begin();
    std::list<Expression*>::iterator it_arg_exp = call->getArguments()->begin();
    for(it_arg=arguments->begin(); it_arg!=arguments->end(); ++it_arg, ++it_arg_exp)
    {
        //current argument
        Variable* arg = *it_arg;

        //current argument value
        Expression* arg_exp = *it_arg_exp;
    
        //replace current argument in block
        optimizeTree_traverseTree_ReplaceArguments(inlineBlock, arg, arg_exp);
    }

//-----------------------------------------------------------------------------
//treat return statements - convert to jump

    Expr_Identifier* retValId = new Expr_Identifier(varRetName);
    retValId->setLineNumber(-1);
    optimizeTree_traverseTree_ReplaceReturn(inlineBlock, inlineBlock, labelEnd, retValId);

//-----------------------------------------------------------------------------
//insert function code after new variable

    //insert block before parent statement
    parentBlock->insertAfter(parentStatementIndex, inlineBlock);

//-----------------------------------------------------------------------------
//remove parent statement, if function call

    //if parent statement is valid function call
    FuncCall* parentCall = dynamic_cast<FuncCall*>(parentStatement);
    if(parentCall!=NULL)
    {
        //remove old call
        delete parentStatement;
        parentBlock->erase(parentStatementIndex+1);
    }
}

unsigned int getStatementCount(Function* func)
{
    //unsigned int counter = 0;

    //skip external functions by zero statements
    if(func->getAbi()!=Abi_default)
        return 0;

    //is valid function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block==NULL) return 0;

    return block->getChildren().size();
}

void optimizeTree_traverseTree(Function* parentFunc, Statement* parentStatement, Statement* statement)
{
    //if statement is function call from source code
    FuncCall* call = dynamic_cast<FuncCall*>(statement);
    if( (call!=NULL) && (call->getFunction()!=parentFunc) && (call->getFunction()->getAbi()==Abi_default) )
    {
        std::cout << "   -inline function call '" << call->getFunction()->getIdentifier() << "' in line '" << parentStatement->getLineNumber() << "'" << std::endl;

        //do inlining on FuncCall
        doInlining(parentFunc, parentStatement, call);

        //finish current node
        return;
    }

    //get all child nodes
    std::vector<Node**> childs = statement->getChildren();
    for(unsigned int i=0; i<childs.size(); i++)
    {
        Statement* childStatement = dynamic_cast<Statement*>(*childs[i]);
        if(childStatement==NULL) continue;
       
        optimizeTree_traverseTree(parentFunc, parentStatement, childStatement);
    }
}

void optimizeTree_Inlining(File* file)
{
    std::cout << " -Inlining" << std::endl;

    //get list of all function in source file
    std::vector<Function**> functions = file->getFunctions();

    //calculate statement count for each function  
    std::vector<unsigned int> statementCounts;
    for(unsigned int i=0; i<functions.size(); i++)
    {
        statementCounts.push_back(getStatementCount(*functions[i]));
    }

    //process each function and search for function calls to inline
    for(unsigned int i=0; i<functions.size(); i++)
    {
        //current function
        Function* func = *functions[i];

        //skip external functions
        if(func->getAbi()!=Abi_default) continue;

        //get all statements of function
        std::vector<Statement*> statements= getStatements(func);

        std::cout << "  -analyze function '" << func->getIdentifier() << "'" << std::endl;
        
        //for each statement
        for(unsigned int i=0; i<statements.size(); i++)
        {
            //explore statement recursive - try to find function calls
            optimizeTree_traverseTree(func, statements[i], statements[i]);
        }
    }
}

std::vector<Node**> Block::getChildren()
{
    std::vector<Node**> result;
    for(std::list<Statement*>::iterator it=subs.begin(); it!=subs.end(); ++it)
        result.push_back((Node**)&(*it));
    return result;
}

std::vector<Node**> WhileLoop::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&condition);
    result.push_back((Node**)&body);
    return result;
}

std::vector<Node**> IfElse::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&condition);
    result.push_back((Node**)&then);
    result.push_back((Node**)&otherwise);
    return result;
}

std::vector<Node**> Unary::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&sub);
    return result;
}

std::vector<Node**> Binary::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&left);
    result.push_back((Node**)&right);
    return result;
}

std::vector<Node**> Return::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&expr);
    return result;
}

std::vector<Node**> ForLoop::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&init_value);
    result.push_back((Node**)&final_value);
    result.push_back((Node**)&step);
    result.push_back((Node**)&body);
    return result;
}

std::vector<Node**> SwitchCase::getChildren(){
    std::vector<Node**> children;
    children.reserve(1 + 2 * cases->size());
    
    // uh oh!
    children.push_back(reinterpret_cast<Node**>(&which));
    
    std::list<Case*>::iterator caseIter = cases->begin();
    std::list<Case*>::iterator caseEnd = cases->end();
    for(; caseIter != caseEnd; ++caseIter){
        //children.push_back(reinterpret_cast<Node**>(&((*caseIter)->condition))); <-- PERVERS (???)
        children.push_back(reinterpret_cast<Node**>(&((*caseIter)->action)));
    }
    
    return children;
}

std::vector<Node**> Constant::getChildren()
{
    std::vector<Node**> result;
    return result;
}

std::vector<Node**> Expr_Cast::getChildren()
{
    std::vector<Node**> result;
    return result;
}

std::vector<Node**> Expr_Identifier::getChildren()
{
    std::vector<Node**> result;
    return result;
}

void Expr_Identifier::setRef(Variable* ref)
{
    this->ref = ref;
}

std::vector<Node**> Expr_Struc::getChildren()
{
    std::vector<Node**> result;
    return result;
}

std::vector<Node**> FuncCall::getChildren()
{
    std::vector<Node**> result;
    
    std::list<Expression*>* arguments = this->getArguments();
    for(std::list<Expression*>::iterator it=arguments->begin(); it!=arguments->end(); ++it)
        result.push_back((Node**)&(*it));

    return result;
}

std::vector<Node**> Local::getChildren()
{
    std::vector<Node**> result;
    return result;
}

std::vector<Node**> GotoLabel::getChildren()
{
    std::vector<Node**> result;
    return result;
}

std::vector<Node**> Goto::getChildren()
{
    std::vector<Node**> result;
    return result;
}

//------------------

std::vector<Node**> File::getChildren()
{
    std::vector<Node**> childs;

    for(std::list<TypeDecl*>::iterator it=types.begin(); it!=types.end(); ++it)
        childs.push_back((Node**)&(*it));

    for(std::list<Variable*>::iterator it=variables.begin(); it!=variables.end(); ++it)
        childs.push_back((Node**)&(*it));

    for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
        childs.push_back((Node**)&(*it));

    return childs;
}

std::vector<Node**> TypeDecl::getChildren()
{
    std::vector<Node**> childs;
    childs.push_back((Node**)&identifier);
    childs.push_back((Node**)&type);
    return childs;
}

std::vector<Node**> StructVariable::getChildren()
{
    std::vector<Node**> childs;
    childs.push_back((Node**)&identifier);
    childs.push_back((Node**)&type);
    return childs;
}

std::vector<Node**> LocalVariable::getChildren()
{
    std::vector<Node**> childs;
    childs.push_back((Node**)&identifier);
    childs.push_back((Node**)&type);
    return childs;
}

std::vector<Node**> GlobalVariable::getChildren()
{
    std::vector<Node**> childs;
    childs.push_back((Node**)&identifier);
    childs.push_back((Node**)&type);
    return childs;
}

