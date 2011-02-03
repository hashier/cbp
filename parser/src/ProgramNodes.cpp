#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "Variables.h"
#include "AbstractNodes.h"
#include "DirectedAcyclicGraph.h"

SymbolTable::SymbolTable *Node::symbolTable = new SymbolTable::SymbolTable();
extern int yylineno; // from lex/bison


Function::Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Type* type, Statement* statement)
    : Declaration(*identifier, yylineno), abi(abi), arguments(arguments), statement(statement), type(type), gotMark(false)
{
    try
    {
        symbolTable->insertDefinition(this);
    }
    catch (SymbolTable::DefinitionAlreadyExistsException &e)
    {
        std::cerr << "Error in line " << yylineno << ": " << e.what() << std::endl;
    }
}

void Function::dump(int num) {
    indent(num); std::cout << "Function: " << identifier << std::endl;;
    indent(num); std::cout << "Statement: " << std::endl;
    if(statement)
        statement->dump(num+1);
    else
        std::cout << "NULL (only declaration)" << std::endl;
}

Function::~Function() {
    if (arguments) delete arguments; arguments = 0;
    if (statement) delete statement; statement = 0;
}


TypeDecl::TypeDecl(std::string* identifier, Type* type)
    : Declaration(*identifier, yylineno), type(type)
{
    try
    {
        symbolTable->insertDefinition(this);
    }
    catch (SymbolTable::DefinitionAlreadyExistsException &e)
    {
        std::cerr << "Error in line " << yylineno << ": " << e.what() << std::endl;
    }
}

TypeDecl::TypeDecl(TypeDecl *node) 
    : Declaration(node)
{
    type = node->type->clone();
}

void TypeDecl::dump(int num)
{
    indent(num); std::cout << "Type declaration: " << identifier << " of type " << std::endl;
    // type->dump(num+1);
}

void File::add(TypeDecl* type) {
    types.push_back(type);
}

void File::add(Variable* var) {
    variables.push_back(var);
}

void File::add(Function* func) {
    functions.push_back(func);
}

std::vector<Function**> File::getFunctions()
{
    std::vector<Function**> temp;
    for(std::list<Function*>::iterator it=functions.begin(); it!=functions.end(); ++it)
        temp.push_back((Function**)&(*it));
    return temp;
}

void File::dump(int num) {
    indent(num); std::cout << "Type declarations:" << std::endl; {
        std::list<TypeDecl*>::iterator it;
        for ( it = types.begin() ; it != types.end(); it++ ) {
            (*it)->dump(num+1);
        }
    }

    indent(num); std::cout << "Variable declarations:" << std::endl; {
        std::list<Variable*>::iterator it;
        for ( it = variables.begin() ; it != variables.end(); it++ ) {
            (*it)->dump(num+1);
        }
    }

    indent(num); std::cout << "Function declarations:" << std::endl; {
        std::list<Function*>::iterator it;
        for ( it = functions.begin() ; it != functions.end(); it++ ) {
            (*it)->dump(num+1);
        }
    }
}

File::~File() {
    for (std::list<TypeDecl *>::iterator it = types.begin() ; it != types.end(); it++ ) {
        delete (*it);
    }
    for (std::list<Variable *>::iterator it = variables.begin() ; it != variables.end(); it++ ) {
        delete (*it);
    }
    for (std::list<Function *>::iterator it = functions.begin() ; it != functions.end(); it++ ) {
        delete (*it);
    }
}

void Block::add(Statement* st) {

    subs.push_back(st);

    //st->addToDAG(dag); // TODO: call DAG here (when its done)
}

void Block::dump(int num) {
    indent(num); std::cout << "{" << std::endl;

    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ ) {
        (*it)->dump(num+1);
    }

    indent(num); std::cout << "}" << std::endl;
}

Node* Block::clone()
{
    Block* copy = new Block();
    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ ) {
        copy->add((Statement*)(*it)->clone());
    }
    return copy;
}



Block::~Block() {
    //dag->dumpAll();
    for (std::list<Statement*>::iterator it = subs.begin() ; it != subs.end(); it++ ) {
        delete (*it);
    }
}

IfElse::IfElse(Expression* condition, Statement* then, Statement* otherwise) : condition(condition), then(then), otherwise(otherwise) {
}

void IfElse::dump(int num) {
    indent(num); std::cout << "If" << std::endl;
    indent(num); std::cout << "Condition:" << std::endl;
    condition->dump(num+1);
    indent(num); std::cout << "Then:" << std::endl;
    then->dump(num+1);
    if(otherwise) {
        indent(num); std::cout << "ELSE" << std::endl;
        otherwise->dump(num+1);
    }
}

Node* IfElse::clone()
{
    IfElse* copy = new IfElse(  (Expression*)condition->clone(),
                                (Statement*)then->clone(),
                                otherwise==NULL ? NULL : (Statement*)otherwise->clone());
    return copy;
}

WhileLoop::WhileLoop(Expression* condition, Statement* body) : condition(condition), body(body) {
}

void WhileLoop::dump(int num) {
    indent(num); std::cout << "While Loop" << std::endl;
    indent(num); std::cout << "Condition:" << std::endl;
    condition->dump(num+1);
    if(body==NULL) return;
    indent(num); std::cout << "Body:" << std::endl;
    body->dump(num+1);
}

WhileLoop::~WhileLoop() {
    if (condition) delete condition; condition = 0;
    if (body) delete body; body = 0;
}

Node* WhileLoop::clone()
{
    WhileLoop* copy = new WhileLoop((Expression*)condition->clone(), (Statement*)body->clone());
    return copy;
}

Return::Return(Expression* expr) : expr(expr) {
}

void Return::dump(int num) {
    indent(num); std::cout << "Return" << std::endl;
    if(expr==NULL) return;
    indent(num); std::cout << "expr:" << std::endl;
    expr->dump(num+1);
}

Node* Return::clone()
{
    Return* copy = new Return(expr==NULL ? NULL : (Expression*)expr->clone());
    return copy;
}

Return::~Return() {
    if (expr) delete expr; expr = 0;
}

Local::Local(Variable* var) : var(var) {
}

void Local::dump(int num) {
    indent(num); std::cout << "Local Variable:" << std::endl;
    var->dump(num+1);
}

Local::~Local() {
    if (var) delete var; var = 0;
}

Node* Local::clone()
{
    return new Local((Variable*)var->clone());
}

//ForLoop without step expression
ForLoop::ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Statement* body) :
    init_value(init_value),final_value(final_value),step(0),body(body)
{
    try
    {
        iterator = dynamic_cast<Variable *>(symbolTable->getDefinition(*iteratorname));
        if (iterator == NULL)
        {
            std::cerr << "Error: Iterator '" << *iteratorname << "'in For-Loop is not a variable." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << " " << e.what() << std::endl;
    }
}

//ForLoop with step expression
ForLoop::ForLoop(std::string* iteratorname, Expression* init_value, Expression* final_value, Expression* step, Statement* body) :
init_value(init_value),final_value(final_value),step(step),body(body)
{
    try
    {
        iterator = dynamic_cast<Variable *>(symbolTable->getDefinition(*iteratorname));
        if (iterator == NULL)
        {
            std::cerr << "Error: Iterator '" << *iteratorname << "'in For-Loop is not a variable." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Iterator '" << *iteratorname << "' is not defined." << " " << e.what() << std::endl;
    }
}

void ForLoop::dump(int num) {
    indent(num); std::cout << "For Loop" << std::endl;
    indent(num); std::cout << "Iterator: " << iterator->getIdentifier() << std::endl;

    indent(num); std::cout << "InitValue:" << std::endl;
    init_value->dump(num+1);

    indent(num); std::cout << "FinalValue:" << std::endl;
    final_value->dump(num+1);

    if(step!=NULL) {
        indent(num); std::cout << "Step:" << std::endl;
        step->dump(num+1);
    }

    if(body==NULL) return;
    indent(num); std::cout << "Body:" << std::endl;
    body->dump(num+1);
}

ForLoop::~ForLoop() {
    // don't delete Variable* iterator here!
    if (init_value) delete init_value; init_value = 0;
    if (final_value) delete final_value; final_value = 0;
    if (step) delete step; step = 0;
    if (body) delete body; init_value = 0;
}

Node* ForLoop::clone()
{
    ForLoop* copy = new ForLoop(    new std::string(iterator->getIdentifier()),
                                    (Expression*)init_value->clone(),
                                    (Expression*)final_value->clone(),
                                    step==NULL ? NULL : (Expression*)step->clone(),
                                    body==NULL ? NULL : (Statement*)body->clone());
    return copy;
}


GotoLabel::GotoLabel():label(NULL) {

}
GotoLabel::~GotoLabel() {
    delete label;
}
void GotoLabel::dump(int num) {
    indent(num); std::cout << "GotoLabel" << std::endl;
}
bool GotoLabel::isConst() {
    return false;
}
void GotoLabel::constProp() {

}
constant* GotoLabel::getConstant() {
    return NULL;
}
int GotoLabel::calcStackOffset(int offset) {
    return 0;
}
Node* GotoLabel::clone()
{
    GotoLabel* copy = new GotoLabel();
    return copy;
}

Goto::Goto(GotoLabel* gotoLabel):gotoLabel(gotoLabel) {

}
Goto::~Goto() {

}
void Goto::dump(int num) {
    indent(num); std::cout << "Goto" << std::endl;
}
bool Goto::isConst() {
    return false;
}
void Goto::constProp() {

}
constant* Goto::getConstant() {
    return NULL;
}
int Goto::calcStackOffset(int offset) {
    return 0;
}
Node* Goto::clone()
{
    Goto* copy = new Goto(this->getGotoLabel());
    return copy;
}











StructVariable::StructVariable(std::string* identifier, Type* type) : Variable(identifier, type), explicitOffset(-1) {
    this->offset = offset;
}

StructVariable::StructVariable(StructVariable *node) : Variable(node), explicitOffset(node->getExplicitOffset()) {
    this->offset = node->getMemoryOffset();
}

void StructVariable::dump(int num) {
    indent(num); std::cout << "StructVariable '" << identifier  << "' @ " << offset << " : " << std::endl;
    // type->dump(num+1);
}

Node *StructVariable::clone(){
    StructVariable *copy = new StructVariable(this);
    return copy;
}


GlobalVariable::GlobalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
    try {
        symbolTable->insertGlobalDefinition(this);
    } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
        std::cerr << "Error in line " << yylineno << ": " << e.what() << std::endl;
    }
}

GlobalVariable::GlobalVariable(GlobalVariable *node) : Variable(new std::string(node->getIdentifier()), node->getType()->clone()) {
}

Node *GlobalVariable::clone(){
    GlobalVariable *copy = new GlobalVariable(this);
    return copy;
}


LocalVariable::LocalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
    try {
        symbolTable->insertDefinition(this);
    } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
        std::cerr << "Error in line " << yylineno << ": " << e.what() << std::endl;
    }
}

LocalVariable::LocalVariable(LocalVariable *node) : Variable(new std::string(node->getIdentifier()), node->getType()->clone()) {
}

Node *LocalVariable::clone(){
    LocalVariable *copy = new LocalVariable(this);
    return copy;
}


Variable::Variable(std::string* identifier, Type* type)
    : Declaration(*identifier, yylineno), type(type), offset(-1) {
}

Variable::Variable(Variable *node)
: Declaration(node->getIdentifier(), yylineno), type(node->getType()->clone()), offset(node->getMemoryOffset()) {
}

void Variable::dump(int num) {
    indent(num); std::cout << "Variable '" << identifier << "': " << std::endl;
    // type->dump(num+1);
}

Type* TypeDecl::getDeclaredType(std::string *identifier)
{
    TypeDecl *tdef = NULL;
    try
    {
        if ((tdef = dynamic_cast<TypeDecl *>(symbolTable->getDefinition(*identifier))))
        {
            return tdef->getType();
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Undefined Type: " << *identifier << " " << e.what() << std::endl;
    }
    return NULL;
}

Node *TypeDecl::clone()
{
    TypeDecl *copy = new TypeDecl(this);
    return copy;
}

SwitchCase::SwitchCase(Expression* which_, std::list<Case*>* cases_): which(which_),cases(cases_) { }

SwitchCase::~SwitchCase(){
    delete which;
    std::list<Case*>::iterator caseIter = cases->begin();
    for(; caseIter != cases->end(); ++caseIter ){
        delete (*caseIter);
    }
}

SwitchCase::Case::~Case() {
    delete condition;
    delete action;
}

void SwitchCase::dump(int num) {
    indent(num); std::cout << "SwitchCase {" << std::endl;
    num += 1;
    indent(num); std::cout << "Condition:" << std::endl;
    which->dump(num + 1);
    std::list<Case*>::const_iterator caseIter = cases->begin();
    for(; caseIter != cases->end(); ++caseIter ){
        indent(num); std::cout << "Case" << std::endl;
        (*caseIter)->condition->dump(num + 1);
        indent(num); std::cout << "Action:" << std::endl;
        (*caseIter)->action->dump(num + 1);
    }
    num -= 1;
    indent(num); std::cout << "}" << std::endl;
}

SwitchCase::Case::Case(Case const& other)
    : condition(static_cast<ConstInt*>(other.condition->clone())),
    action(static_cast<Statement*>(other.action->clone()))
    {}

Node* SwitchCase::clone(){
    Expression* which_ = static_cast<Expression*>(which->clone());
    
    std::list<Case*>* cases_ = new std::list<Case*>;
    std::list<Case*>::iterator caseIter = cases->begin();
    std::list<Case*>::iterator caseEnd = cases->end();
    for(; caseIter != caseEnd; ++caseIter){
        cases_->push_back(new Case(**caseIter));
    }
    
    return new SwitchCase(which_, cases_);
}

