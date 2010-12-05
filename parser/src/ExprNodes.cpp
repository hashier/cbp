#include "ProgramNodes.h"
#include "ExprNodes.h"
#include "ExprNodes.h"
#include "Exceptions.h"

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


FuncCall::FuncCall(std::string* identifier, std::list<Expression*>* exprs) 
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
