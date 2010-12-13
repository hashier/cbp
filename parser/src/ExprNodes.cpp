#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "ExprNodes.h"
#include "Exceptions.h"



void Unary::dump(int num) {
	indent(num); std::cout << "Unary: " << typeid(*this).name() << std::endl;
	sub->dump(num);
}


void Binary::dump(int num) {
	indent(num); std::cout << "Binary: " << typeid(*this).name() << std::endl;

	indent(num); std::cout << "Left:" << std::endl;
	left->dump(num+1);

	indent(num); std::cout << "Right:" << std::endl;
	right->dump(num+1);
}


Expr_Ref::Expr_Ref(Expression* sub) : Unary(sub) {
    type = new TypePointer(sub->getType());
}


void Expr_Struc::dump(int num)
{
    indent(num); std::cout << "Expr_Struc:";
    // sub->dump(num);
}

Expr_Deref::Expr_Deref(Expression *sub) : Unary(sub), index(0) {
    if(typeid(*sub->getType()) != typeid(TypePointer))
        std::cerr << "Warning: Dereferencing non-pointer variable!" << std::endl;
}

Expr_Deref::Expr_Deref(Expression *sub, Expression *index) : Unary(sub), index(index) {
    if(typeid(*sub->getType()) != typeid(TypePointer))
        std::cerr << "Warning: Dereferencing non-pointer variable!" << std::endl;
}

Type* Expr_Deref::getType() {
    TypePointer* pointedType = dynamic_cast<TypePointer*>(sub->getType());
    if(pointedType)
        return pointedType->getType();
    else
        return TypeVoid::getSingleton();
}

ConstInt::ConstInt(int value) : value(value) {
    // TODO always 32 bit?
    type = new TypeSimple(Type_int32);
}

void ConstInt::dump(int num) {
    indent(num);
    std::cout << "Const Int: " << value << std::endl;
}

int ConstInt::val() const {
    return value;
}

Type* ConstInt::getType() {
    return type;
}

ConstFloat::ConstFloat(float value) : value(value) {
    // TODO always 32 bit?
    type = new TypeSimple(Type_float32);
}

void ConstFloat::dump(int num) {
    indent(num);
    std::cout << "Const Float: " << value << std::endl;
}

Type* ConstFloat::getType() {
    return type;
}









Expr_Struc::Expr_Struc(Expression* exp, std::string* identifier) {
}

Expr_Identifier::Expr_Identifier(std::string *identifier)
{
    try
    {
        ref = dynamic_cast<Variable *>(symbolTable->getDefinition(*identifier));
        if (ref == NULL)
        {
            std::cerr << "Error: Identifier '" << *identifier << "' in Expression is not declared as a variable." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Undefined identifier in Expression: '" << *identifier << "'. " << e.what() << std::endl;
    }
}

void Expr_Identifier::dump(int num) {
    std::cout << "Identifier in Expression:" << std::endl;
    ref->dump(num+1);
}

void Expr_Cast::dump(int num)
{
      indent(num); std::cout << "Cast as:" << std::endl;
      // if(castType != NULL) {
        // castType->dump(num + 1);
      // }
}

/** Return the type of a binary expression.
 * We basically just check if either of the two operands is of
 * a floating point type. If so, the result is floating point
 * as well, otherwise it's an int.
 * @throw TypeConversionException if either type is something we can't calculate with.
 * @return the type of the expression's result
 */
Type* Binary::getType() {
    // Don't short-circuit this, we do want a full traversal and type-check.
    TypeSimple* leftType = dynamic_cast<TypeSimple*>(left->getType());
    TypeSimple* rightType = dynamic_cast<TypeSimple*>(right->getType());
    if(!leftType)
        throw TypeConversionException(/*"Invalid type of left-hand operand."*/);
    if(!rightType)
        throw TypeConversionException(/*"Invalid type of right-hand operand."*/);

    // If both types are either floating or non-floating, just return the bigger one.
    if(leftType->isFloating() == rightType->isFloating()) {
        return (*leftType) > (*rightType) ? leftType : rightType;
    }
    // Otherwise, it's more complicated. For now, just return the one that's floating.
    // TODO Handle special case: non-floating type > floating type!
    return leftType->isFloating() ? leftType : rightType;
}


FuncCall::FuncCall(std::string* identifier, std::list<Expression*>* arguments) : arguments(arguments)
{
    try
    {
        func = dynamic_cast<Function *>(Node::symbolTable->getDefinition(*identifier));
        if (func == NULL)
        {
            std::cerr << "Error: Identifier '" << *identifier << "' is not a function, but is used as a function." << std::endl;
        }
    }
    catch (SymbolTable::DefinitionNotFoundException &e)
    {
        std::cerr << "Error: Call to undeclared Function: " << *identifier << " " << e.what() << std::endl;
    }
}

void FuncCall::dump(int num)
{
    indent(num); std::cout << "Function Call: " << func->getIdentifier() << std::endl;
}
