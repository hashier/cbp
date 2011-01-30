#include "Inlining.h"

#include <vector>
#include <list>
#include <iostream>
#include <utility>
#include <algorithm>
#include "ExprNodes.h"
#include "ProgramNodes.h"
#include "Variables.h"

//get function calls from a certain function
//if call=NULL get all function calls
std::list<FuncCall*> getFunctionCalls(Function* func, Function* call)
{
    //result list
    std::list<FuncCall*> calls;

    //is valid function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block==NULL) return calls;

    //get all statements of current function
    std::list<Statement*> statements = block->getSubStatements();

    //for each function
    for(std::list<Statement*>::iterator it=statements.begin(); it!=statements.end(); ++it)
    {
        //statement to explore
        Statement* statement = *it;

        //check if statement is function
        FuncCall* call_ = dynamic_cast<FuncCall*>(statement);

        if( ((call==NULL)&&(call_!=NULL)) || ((call_!=NULL)&&(call_->getFunction()==call)) )
        {
            calls.push_back(call_);
        }
    }

    return calls;
}


std::list<Statement*> getStatementsByList(Function* func)
{
    std::list<Statement*> result;

    //get all statements of host function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block!=NULL)
    {
        result = block->getSubStatements();
    }
   
    return result;
}

std::vector<Statement*> getStatementsByVector(Function* func)
{
    std::vector<Statement*> result;

    //get all statements of host function
    Block* block = dynamic_cast<Block*>(func->getStatement());
    if(block==NULL) return result;

    std::list<Statement*> list = block->getSubStatements();

    for(std::list<Statement*>::iterator it=list.begin(); it!=list.end(); ++it)
    {
        result.push_back(*it);
    }

    return result;  
}

void optimizeTree_traverseTree_ReplaceArguments(Statement* statement, Variable* arg, Expression* exp)
{
    //get all child nodes
    std::vector<Node*> childs = statement->getChildNodes();

    for(unsigned int i=0; i<childs.size(); i++)
    {
        //if child is a variable
        Expr_Identifier* var = dynamic_cast<Expr_Identifier*>(childs[i]);
        if(var!=NULL)
        {
            //its the argument we are going to replace?
            if(var->getRef()==arg)
            {
                std::cout << "    -replace var '" << var->getRef()->getIdentifier() << "' at line " << var->getLineNumber() << std::endl; 
                statement->replaceChild(var, exp);
            }

            continue;
        }

        //if childs is a Statement
        Statement* child = dynamic_cast<Statement*>(childs[i]);
        if(child==NULL) continue;
        optimizeTree_traverseTree_ReplaceArguments(child, arg, exp);
    }
}

void optimizeTree_traverseTree_ReplaceReturn(Block* block, Statement* parentStatement, GotoLabel* label, Expr_Identifier* var)
{
    //get all child nodes
    std::vector<Node*> childs = parentStatement->getChildNodes();

    for(unsigned int i=0; i<childs.size(); i++)
    {
        //if child is a return
        Return* ret = dynamic_cast<Return*>(childs[i]);
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
            parentStatement->replaceChild(ret, assignResult);

            //insert jump to end
            Goto* goto_ = new Goto(label);
            goto_->setLineNumber(-1);
            block->insertAfter(assignResult, goto_);
            
            continue;
        }

        //if childs is a Statement
        Statement* child = dynamic_cast<Statement*>(childs[i]);
        if(child==NULL) continue;

        //enter new block?
        Block* newBlock = dynamic_cast<Block*>(childs[i]); 
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
    std::list<Statement*>& parentBlockStatements = parentBlock->getSubStatementsRef();
    std::list<Statement*>::iterator itParentStatement = find(parentBlockStatements.begin(), parentBlockStatements.end(), parentStatement);

//-----------------------------------------------------------------------------
//create new local variable for result

    //create unique name
    std::stringstream ssName;
    ssName << "InlineRet_" << parentBlock->getSubStatements().size(); //TODO: create unique pre/suffix

    //create local var
    std::string* varRetName = new std::string(ssName.str()); 
    LocalVariable* varRet = new LocalVariable(varRetName, call->getFunction()->getType());
    varRet->setLineNumber(-1); //means lines was created by optimizer
    Local* localVarRet = new Local(varRet);
    localVarRet->setLineNumber(-1); //means lines was created by optimizer

    //insert local var before function call
    parentBlockStatements.insert(itParentStatement, localVarRet);

//-----------------------------------------------------------------------------
//create copy of inline function

    //create new block
    Block* inlineBlock = new Block();
    inlineBlock->setLineNumber(-1);

    //get statements from inline function
    std::list<Statement*> inlineBlockStatements = ((Block*)call->getFunction()->getStatement())->getSubStatements();

    //add statements to block
    for(std::list<Statement*>::iterator it=inlineBlockStatements.begin(); it!=inlineBlockStatements.end(); ++it)
    {
        inlineBlock->add(*it);
    }

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
    parentBlockStatements.insert(itParentStatement, inlineBlock);

//-----------------------------------------------------------------------------
//remove parent statement, if function call

    //if parent statement is valid function call
    FuncCall* parentCall = dynamic_cast<FuncCall*>(parentStatement);
    if(parentCall!=NULL)
    {
        //remove old call
        delete parentStatement;
        parentBlockStatements.erase(itParentStatement);
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

    return block->getSubStatements().size();
}

void optimizeTree_traverseTree(Function* parentFunc, Statement* parentStatement, Statement* statement)
{
    //if statement is function call from source code
    FuncCall* call = dynamic_cast<FuncCall*>(statement);
    if( (call!=NULL) && (call->getFunction()->getAbi()==Abi_default) )
    {
        std::cout << "   -inline function call '" << call->getFunction()->getIdentifier() << "' in line '" << parentStatement->getLineNumber() << "'" << std::endl;

        //do inlining on FuncCall
        doInlining(parentFunc, parentStatement, call);
        
        //finish current node
        return;
    }

    //get all child nodes
    std::vector<Node*> childs = statement->getChildNodes();
    for(unsigned int i=0; i<childs.size(); i++)
    {
        Statement* childStatement = dynamic_cast<Statement*>(childs[i]);
        if(childStatement==NULL) continue;
       
        optimizeTree_traverseTree(parentFunc, parentStatement, childStatement);
    }
}

void optimizeTree_Inlining(File* file)
{
    std::cout << " -Inlining" << std::endl;

    //get list of all function in source file
    std::list<Function*>& functions = file->getFunctions();

    //calculate statement count for each function  
    std::vector<unsigned int> statementCounts;
    for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
    {
        statementCounts.push_back(getStatementCount(*it));
    }

    //process each function and search for function calls to inline
    for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
    {
        //current function
        Function* func = *it;

        //skip external functions
        if(func->getAbi()!=Abi_default) continue;

        //get all statements of function
        std::vector<Statement*> statements= getStatementsByVector(func);

        std::cout << "  -analyze function '" << func->getIdentifier() << "'" << std::endl;
        
        //for each statement
        for(unsigned int i=0; i<statements.size(); i++)
        {
            //explore statement recursive - try to find function calls
            optimizeTree_traverseTree(func, statements[i], statements[i]);
        }
    }
}

std::vector<Node*> Block::getChildNodes()
{
    std::vector<Node*> result;
    for(std::list<Statement*>::iterator it=subs.begin(); it!=subs.end(); ++it)
        result.push_back(*it);
    return result;
}

void Block::replaceChild(Node* currentChild, Node* newChild)
{
    replace(subs.begin(), subs.end(), (Statement*)currentChild, (Statement*)newChild);
}

std::vector<Node*> WhileLoop::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->condition);
    result.push_back(this->body);
    return result;
}

void WhileLoop::replaceChild(Node* currentChild, Node* newChild)
{
    if(condition==currentChild) condition = (Expression*)newChild;
    if(body==currentChild) body = (Statement*)newChild;
}

std::vector<Node*> IfElse::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->condition);
    result.push_back(this->otherwise);
    result.push_back(this->then);
    return result;
}

void IfElse::replaceChild(Node* currentChild, Node* newChild)
{
    if(currentChild==condition) condition = (Expression*)newChild;
    if(currentChild==otherwise) otherwise = (Statement*)newChild;
    if(currentChild==then) then = (Statement*)newChild;
}

std::vector<Node*> Unary::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->sub);
    return result;
}

void Unary::replaceChild(Node* currentChild, Node* newChild)
{
    if(currentChild==sub) sub = (Expression*)newChild;
}

std::vector<Node*> Binary::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->left);
    result.push_back(this->right);
    return result;
}

void Binary::replaceChild(Node* currentChild, Node* newChild)
{
    if(left==currentChild) left = (Expression*)newChild;
    if(right==currentChild) right = (Expression*)newChild;
}

std::vector<Node*> Return::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->expr);
    return result;
}

void Return::replaceChild(Node* currentChild, Node* newChild)
{
    if(expr==currentChild) expr = (Expression*)newChild;
}

std::vector<Node*> ForLoop::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->init_value);
    result.push_back(this->final_value);
    result.push_back(this->step);
    result.push_back(this->body);
    return result;
}

void ForLoop::replaceChild(Node* currentChild, Node* newChild)
{
    if(init_value==currentChild) init_value = (Expression*)newChild;
    if(final_value==currentChild) final_value = (Expression*)newChild;
    if(step==currentChild) step = (Expression*)newChild;
    if(body==currentChild) body = (Statement*)newChild;
}


std::vector<Node*> SwitchCase::getChildNodes()
{
    std::vector<Node*> result;
    result.push_back(this->which);
    for(std::list<Case*>::iterator it=cases->begin(); it!=cases->end(); ++it)
    {
        Case* c = *it;
        //result.push_back(c->condition); <-- PERVERS
        result.push_back(c->action);
    }
    return result;
}

void SwitchCase::replaceChild(Node* currentChild, Node* newChild)
{
    for(std::list<Case*>::iterator it=cases->begin(); it!=cases->end(); ++it)
    {
        Case* c = *it;
        if(c->action==currentChild) c->action=(Statement*)newChild;
    } 
}

std::vector<Node*> Constant::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Constant::replaceChild(Node* currentChild, Node* newChild)
{

}

std::vector<Node*> Expr_Cast::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Expr_Cast::replaceChild(Node* currentChild, Node* newChild)
{

}

std::vector<Node*> Expr_Identifier::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Expr_Identifier::replaceChild(Node* currentChild, Node* newChild)
{

}

void Expr_Identifier::setRef(Variable* ref)
{
    this->ref = ref;
}

std::vector<Node*> Expr_Struc::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Expr_Struc::replaceChild(Node* currentChild, Node* newChild)
{

}

std::vector<Node*> FuncCall::getChildNodes()
{
    std::vector<Node*> result;
    
    //std::list<Expression*>* arguments = this->getArguments();
    //for(std::list<Expression*>::iterator it=arguments->begin(); it!=arguments->end(); ++it)
    //    result.push_back(*it);

    return result;
}

void FuncCall::replaceChild(Node* currentChild, Node* newChild)
{

}

std::vector<Node*> Local::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Local::replaceChild(Node* currentChild, Node* newChild)
{
    
}

std::vector<Node*> GotoLabel::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void GotoLabel::replaceChild(Node* currentChild, Node* newChild)
{

}

std::vector<Node*> Goto::getChildNodes()
{
    std::vector<Node*> result;
    return result;
}

void Goto::replaceChild(Node* currentChild, Node* newChild)
{

}
