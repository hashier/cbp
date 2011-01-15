#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "Variables.h"
#include "AbstractNodes.h"

SymbolTable::SymbolTable *Node::symbolTable = new SymbolTable::SymbolTable(); // wtf


Function::Function(std::string* identifier, Func_abi abi, std::list<Variable*>* arguments, Type* type, Statement* statement)
    : Declaration(*identifier), abi(abi), arguments(arguments), statement(statement), type(type), gotMark(false)
{
    try
    {
        symbolTable->insertDefinition(this);
    }
    catch (SymbolTable::DefinitionAlreadyExistsException &e)
    {
        std::cerr << "Error: Function already declared in current Scope: " << *identifier << " " << e.what() << std::endl;
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
    : Declaration(*identifier), type(type)
{
    try
    {
        symbolTable->insertDefinition(this);
    }
    catch (SymbolTable::DefinitionAlreadyExistsException &e)
    {
        // TODO: the existing definition can also be something else
        std::cerr << "Error: Definition already exisits: " << " " << e.what() << std::endl;
        //std::cerr << "'" << *identifier << "' is redefined from '" <<
        //    dynamic_cast<SymbolTable::TypeDef *>(symbolTable->getDefinition(*identifier))->getType()->getString()
        //    << "' to '" << type->getType()->getString() << "'." << std::endl;
    }
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
}

void Block::dump(int num) {
    indent(num); std::cout << "{" << std::endl;

    std::list<Statement*>::iterator it;
    for ( it = subs.begin() ; it != subs.end(); it++ ) {
        (*it)->dump(num+1);
    }

    indent(num); std::cout << "}" << std::endl;
}

SwitchCase::Case::~Case()
{
    delete condition;
    delete action;
}

Block::~Block() {
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

SwitchCase::SwitchCase(Expression* which_, std::list<Case*>* cases_): which(which_),cases(cases_) { }

SwitchCase::~SwitchCase(){
    std::list<Case*>::iterator caseIter = cases->begin();
    for(; caseIter != cases->end(); ++caseIter ){
        delete (*caseIter);
    }
}

WhileLoop::WhileLoop(Expression* condition, Statement* body) : condition(condition), body(body) {
}

void WhileLoop::dump(int num) {
    indent(num); std::cout << "While Loop" << std::endl;
    indent(num); std::cout << "Condition:" << std::endl;
    if(body==NULL) return;
    indent(num); std::cout << "Body:" << std::endl;
    body->dump(num+1);
}

WhileLoop::~WhileLoop() {
    if (condition) delete condition; condition = 0;
    if (body) delete body; body = 0;
}

Return::Return(Expression* expr) : expr(expr) {
}

void Return::dump(int num) {
    indent(num); std::cout << "Return" << std::endl;
    if(expr==NULL) return;
    indent(num); std::cout << "expr:" << std::endl;
    expr->dump(num+1);
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





StructVariable::StructVariable(std::string* identifier, Type* type) : Variable(identifier, type), explicitOffset(-1) {
    this->offset = offset;
}

void StructVariable::dump(int num) {
    indent(num); std::cout << "StructVariable '" << identifier  << "' @ " << offset << " : " << std::endl;
    // type->dump(num+1);
}


GlobalVariable::GlobalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
    try {
        symbolTable->insertGlobalDefinition(this);
    } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
        std::cerr << "Error: Variable already declared in current scope: " << *identifier << " " << e.what() << std::endl;
    }
}

LocalVariable::LocalVariable(std::string* identifier, Type* type) : Variable(identifier, type) {
    try {
        symbolTable->insertDefinition(this);
    } catch (SymbolTable::DefinitionAlreadyExistsException &e) {
        std::cerr << "Error: Variable already declared in current scope: " << *identifier << " " << e.what() << std::endl;
    }
}



Variable::Variable(std::string* identifier, Type* type) : Declaration(*identifier), type(type), offset(-1) {
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
