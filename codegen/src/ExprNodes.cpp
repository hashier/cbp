#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"
#include"DirectedAcyclicGraph.h"
#include"Message.h"

#include <cstdlib>
#include <vector>

void Expr_Cast::genLeft(CodeGen* out) {
    expr->genLeft(out);
}
void Expr_Cast::gen(CodeGen* out) {
    expr->gen(out);
}

void Expr_Mul::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Mul::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << Command("imull")("%ebx");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_Mul::clone()
{
    return new Expr_Mul((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_Div::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Div::gen()", this);
    // right hand operand into %eax
    right->gen(out);
    // save to local var
    *out << Command("pushq")("%rdx");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // left operand into %eax
    left->gen(out);
    //set rdx to zero
    *out << Command("xorq")("%rdx")("%rdx");
    //div (rdx:eax / op) = eax + k*edx
    *out << Command("divl")("%ebx");
    *out << Command("popq")("%rbx");
    *out << Command("popq")("%rdx");
}

Node* Expr_Div::clone()
{
    return new Expr_Div((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_Mod::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Mod::gen()", this);
    // right hand operand into %eax
    right->gen(out);
    // save to local var
    *out << Command("pushq")("%rdx");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // left operand into %eax
    left->gen(out);
    //set rdx to zero
    *out << Command("xorq")("%rdx")("%rdx");
    // div (rdx:eax / op) = eax + k*edx
    *out << Command("divl")("%ebx");
    *out << Command("movl")("%edx")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("popq")("%rdx");
}

Node* Expr_Mod::clone()
{
    return new Expr_Mod((Expression*)left->clone(), (Expression*)right->clone());
}

void Constant::genLeft(CodeGen* out) {
    *out << Message(ERROR, "Left values of Constant expressions not supported!", this);
    exit(42);
}

void ConstInt::gen(CodeGen* out) {
    *out << Command("movl")(value)("%eax");
}

Node* ConstInt::clone()
{
    return new ConstInt(value);
}

void Expr_EQ::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_EQ::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if equal
    *out << Command("sete")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_EQ::clone()
{
    return new Expr_EQ((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_NEQ::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_NEQ::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if not equal
    *out << Command("setne")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_NEQ::clone()
{
    return new Expr_NEQ((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_LT::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_LT::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if less than
    *out << Command("setl")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_LT::clone()
{
    return new Expr_LT((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_GT::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_GT::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if greater than
    *out << Command("setg")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_GT::clone()
{
    return new Expr_GT((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_LE::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_LE::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if less than or equal
    *out << Command("setle")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("addq")("%rbx");
}

Node* Expr_LE::clone()
{
    return new Expr_LE((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_GE::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_GE::gen()", this);
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // compare
    *out << Command("cmpl")("%eax")("%ebx");
    // set LSB of %ax if greater than or equal
    *out << Command("setge")("%al");
    // move from byte %al to long %eax and pad with zeros
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_GE::clone()
{
    return new Expr_GE((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_BoolOR::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BoolOR::gen()", this);
    Label labelTrue = out->newMark("true");
    Label labelFalse = out->newMark("false");
    Label labelEnd = out->newMark("end");
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0; if not equal, jump to labelTrue
    *out << Command("cmpl")(0)("%eax");
    *out << Command("jne")(labelTrue);
    // right operand into %eax
    right->gen(out);
    *out << Command("cmpl")(0)("%eax");
    *out << Command("je")(labelFalse);
    // jump mark "true"
    *out << labelTrue;
    *out << Command("movl")(1)("%eax");
    *out << Command("jmp")(labelEnd);
    // jump mark "false"
    *out << labelFalse;
    *out << Command("movl")(0)("%eax");
    // jump mark "end"
    *out << labelEnd;
}

Node* Expr_BoolOR::clone()
{
    return new Expr_BoolOR((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_BoolAND::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BoolAND::gen()", this);
    Label labelFalse = out->newMark("false");
    Label labelEnd = out->newMark("end");
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0; if equal, jump to labelFalse
    *out << Command("cmpl")(0)("%eax");
    *out << Command("je")(labelFalse);
    // right hand operand into %eax
    right->gen(out);
    // compare %eax and 0; if equal, jump to labelFalse
    *out << Command("cmpl")(0)("%eax");
    *out << Command("je")(labelFalse);
    // if we reached this point, both operands are different from 0
    // thus, we moffel 1 into %eax
    *out << Command("movl")(1)("%eax");
    *out << Command("jmp")(labelEnd);
    // jump mark "false"
    *out << labelFalse;
    *out << Command("movl")(0)("%eax");
    // jump mark "end"
    *out << labelEnd;
}

Node* Expr_BoolAND::clone()
{
    return new Expr_BoolAND((Expression*)left->clone(), (Expression*)right->clone());
}

void Expr_BoolXOR::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BoolXOR::gen()", this);
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0
    *out << Command("cmpl")(0)("%eax");
    // set LSB of %bl if equal
    *out << Command("sete")("%bl");
    // right hand operand into %eax
    right->gen(out);
    // compare %eax and 0
    *out << Command("cmpl")(0)("%eax");
    // set LSB of %bh if equal
    *out << Command("sete")("%bh");
    // compare %bl and %bh; if equal, xor returns false
    *out << Command("cmpl")("%bh")("%bl");
    *out << Command("sete")("%al");
    *out << Command("movzbl")("%al")("%eax");
    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_BoolXOR::clone()
{
    return new Expr_BoolXOR((Expression*)left->clone(), (Expression*)right->clone());
}

Node* Expr_BitLeft::clone() {
    return new Expr_BitLeft(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

// Shift operators use the 8-bit %cl register, which is equal to the lowest 8 bits of %ecx.
// arithmetic left shift
void Expr_BitLeft::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BitLeft::gen()", this);
    right->gen(out);                         // right hand operand into %rax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rcx");        // store %rcx including %cl to the stack
    *out << Command("movq")("%rax")("%rcx"); // %rcx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("shl")("%cl")("%rax");   // %rax = %rax << %cl
    *out << Command("popq")("%rcx");         // restore %rcx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * << *", this);
}

Node* Expr_BitRight::clone() {
    return new Expr_BitRight(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

// arithmetic right shift
void Expr_BitRight::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BitRight::gen()", this);
    right->gen(out);                         // right hand operand into %eax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rcx");        // store %rcx including %cl to the stack
    *out << Command("movq")("%rax")("%rcx"); // %rcx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("shr")("%cl")("%rax");   // %rax = %rax >> %cl
    *out << Command("popq")("%rcx");            // restore %rcx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * >> *", this);
}

Node* Expr_BitOR::clone() {
    return new Expr_BitOR(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

void Expr_BitOR::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BitOR::gen()", this);
    right->gen(out);                         // right hand operand into %rax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("or")("%rbx")("%rax");   // %rax = %rax or %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * | *", this);
}

Node* Expr_BitAND::clone() {
    return new Expr_BitAND(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

void Expr_BitAND::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BitAND::gen()", this);
    right->gen(out);                         // right hand operand into %rax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("and")("%rbx")("%rax");  // %rax = %rax and %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * & *", this);
}

Node* Expr_BitXOR::clone() {
    return new Expr_BitXOR(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

void Expr_BitXOR::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_BitXOR::gen()", this);
    right->gen(out);                         // right hand operand into %rax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("xor")("%rbx")("%rax");  // %rax = %rax xor %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * ^ *", this);
}

Node* Expr_Add::clone() {
    return new Expr_Add(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

void Expr_Add::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Add:gen()", this);
    // error: if floats are given (floats are optional)
    if (!left->getType()->isInteger() || !right->getType()->isInteger()) {
        *out << Message(ERROR,
                "ignore not implemented expression: adding float numbers (floats are optional)",
                this);
        return;
    }
    // warning: if signed + unsigned integer
    if (left->getType()->hasSignedBit() != right->getType()->hasSignedBit()) {
        *out << Message(WARNING,
                "possible loss of precision: adding signed and unsigned numbers",
                this);
    }

    Expression* bigExp;   // the argument with the bigger size of type
    Expression* smallExp; // the argument with the lower size of type
    if (left->getType()->getSize() <= right->getType()->getSize()) {
        smallExp = left;
        bigExp   = right;
    } else {
        smallExp = right;
        bigExp   = left;
    }
    int smallSizeInBytes = smallExp->getType()->getSize();
    int bigSizeInBytes   = bigExp->getType()->getSize();

    smallExp->gen(out);                      // argument with small size into %rax
    if (smallSizeInBytes!=bigSizeInBytes) {  // integer with different precision
        std::string movSmall2Big("movsx");
        if (!smallExp->getType()->hasSignedBit()) {
            movSmall2Big[3] = 'z'; // "movzx"
        }
        *out << Command(movSmall2Big)        // expanding size:
                ("%ax", smallSizeInBytes)    //  argument with smaller size expanding
                ("%ax", bigSizeInBytes);     //  to size of argument with bigger size
    }
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    bigExp->gen(out);                        // argument with big size into %rax
    *out << Command("add")                   // %ax = %ax + %bx:
            ("%bx", bigSizeInBytes)          //  argument with smaller size adding to
            ("%ax", bigSizeInBytes);         //  argument with bigger size
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * + *", this);
}

Node* Expr_Sub::clone() {
    return new Expr_Sub(
            static_cast<Expression*>(left->clone()),
            static_cast<Expression*>(right->clone())
    );
}

void Expr_Sub::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Sub:gen()", this);
    // error: if floats are given (floats are optional)
    if (!left->getType()->isInteger() || !right->getType()->isInteger()) {
        *out << Message(ERROR,
                "ignore not implemented expression: subtracting float numbers (floats are optional)",
                this);
        return;
    }
    right->gen(out);                         // right hand operand into %eax
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("sub")("%rbx")("%rax");  // %rax = %rax - %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
    *out << Command("addq")("$8")("%rsp");
    *out << Message(DEBUG, "result of * - *", this);
}

void Expr_Identifier::genLeft(CodeGen* out) {
    *out << Command("leaq")(ref->getAddress())("%rax");
}
void Expr_Identifier::gen(CodeGen* out) {
    int size = ref->getSize();
    *out << Command("mov", size)(ref->getAddress())("%ax", size);
}
Node* Expr_Identifier::clone()
{
    return new Expr_Identifier(ref);
}

void Expr_Assign::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Assign:gen()", this);
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");           // store ebx to the stack
    left->genLeft(out);
    *out << Command("movq")("%rax")("%rbx");   // %ebx = %eax
    right->gen(out);
    int size = right->getType()->getSize();
    *out << Command("mov", size)("%ax", size)(Reg("rbx"));   // %ebx = %eax
    *out << Command("popq")("%rbx");            // restore %ebx from the stack
    *out << Command("addq")("$8")("%rsp");
}

Node* Expr_Assign::clone()
{
    return new Expr_Assign((Expression*)left->clone(), (Expression*)right->clone());
}

void FuncCall::gen(CodeGen *out) {
    *out << Message(DEBUG, "FuncCall:gen()", this);
    static std::vector<std::string> *classInteger;
    if(!classInteger) {
        classInteger = new std::vector<std::string>();
#ifdef MSVC
        //windows calling convention: AMD x64
        classInteger->push_back("%rcx");
        classInteger->push_back("%rdx");
        classInteger->push_back("%r8");
        classInteger->push_back("%r9");
#else
        classInteger->push_back("%rdi");
        classInteger->push_back("%rsi");
        classInteger->push_back("%rdx");
        classInteger->push_back("%rcx");
        classInteger->push_back("%r8");
        classInteger->push_back("%r9");
#endif
    }

    switch(func->getAbi()) {
        case Abi_c:
            {
            *out << Command("pushq")("%rcx");
            *out << Command("pushq")("%rdx");
            *out << Command("pushq")("%rsi");
            *out << Command("pushq")("%rdi");
            *out << Command("pushq")("%r8");
            *out << Command("pushq")("%r9");
            *out << Command("pushq")("%r10");
            *out << Command("pushq")("%r11");
            *out << Command("cld");
            // TODO: align rsp towards 16 byte
            std::vector<std::string>::const_iterator reg = classInteger->begin();
            std::vector<Expression *> stackArgs;
            int stackArgsSize = 0;
            for (std::list<Expression *>::const_iterator it = arguments->begin();
                    it != arguments->end(); ++it) {
                if((*it)->getType()->isInteger() || dynamic_cast<TypePointer *>((*it)->getType())) {
                    if(reg != classInteger->end()) {
                        (*it)->gen(out);
                        *out << Command("movq")("%rax")(*reg++);
                    } else {
                        stackArgsSize++;
                        stackArgs.push_back(*it);
                    }
                } else {
                    // TODO
                    *out << Command("type not supported as argument type");
                }
            }
            //align towards 16 byte (hopefully)
            if(stackArgsSize % 2 != 0) {
                *out << Command("subq")("$8")("%rsp");
            }
            for (std::vector<Expression *>::const_reverse_iterator it = stackArgs.rbegin();
                    it != stackArgs.rend(); ++it) {
                if((*it)->getType()->isInteger() || dynamic_cast<TypePointer *>((*it)->getType())) {
                    (*it)->gen(out);
                    *out << Command("pushq")("%rax");
                } else {
                    // TODO
                    *out << Command("type not supported as argument type");
                }
            }
            *out << Command("call")(func->getMark(out));
            *out << Command("popq")("%r11");
            *out << Command("popq")("%r10");
            *out << Command("popq")("%r9");
            *out << Command("popq")("%r8");
            *out << Command("popq")("%rdi");
            *out << Command("popq")("%rsi");
            *out << Command("popq")("%rdx");
            *out << Command("popq")("%rcx");
            break;
            }
        case Abi_default:
            std::list<Expression*>::reverse_iterator it;
            for ( it = arguments->rbegin() ; it != arguments->rend(); it++ ) {
                (*it)->gen(out);
                *out << Command("pushq")("%rax");
            }
            *out << Command("call")(func->getMark(out));
            *out << Command("addq")(8 * arguments->size())("%rsp");
            break;
    }
}

Node* FuncCall::clone()
{
    std::list<Expression*>* args = new std::list<Expression*>();
    for(std::list<Expression*>::iterator it=arguments->begin(); it!=arguments->end(); ++it)
        args->push_back((Expression*)(*it)->clone());

    return new FuncCall(new std::string(func->getIdentifier()), args); 
}

void Expr_Ref::gen(CodeGen* out) {
    sub->genLeft(out);
}

Node* Expr_Ref::clone()
{
    return new Expr_Ref((Expression*)sub->clone());
}

void Expr_Deref::genLeft(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Deref::genLeft()", this);
    // Evaluate substatement (= base address)
    if(index) {

        sub->gen(out);
        *out << Command("movq")("%rax")("%rbx");

        // We need the size of the pointed-at value.
        TypePointer* ref = dynamic_cast<TypePointer*>(sub->getType());
        assert(ref && "Cannot dereference a void pointer!");
        int refsize = ref->getType()->getSize();

        // Evaluate offset
        *out << Command("subq")("$8")("%rsp");
        *out << Command("pushq")("%rbx");
        index->gen(out);
        
        *out << Command("subq")("$8")("%rsp");
        *out << Command("pushq")("%rdx");
        *out << Command("imul")(refsize)("%eax");
        *out << Command("popq")("%rdx");
        *out << Command("addq")("$8")("%rsp");
        
        // Return value of variable at address
        *out << Command("leaq")(Reg("rax") + Reg("rbx"))("%rax");
        *out << Command("popq")("%rbx");
        *out << Command("addq")("$8")("%rsp");
    } else {
        sub->gen(out);
        *out << Command("leaq")(Reg("rax"))("%rax");
    }
}

void Expr_Deref::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Deref::gen()", this);
    // Evaluate substatement (= base address)
    sub->gen(out);

    // We need the size of the pointed-at value.
    TypePointer* ref = dynamic_cast<TypePointer*>(sub->getType());
    assert(ref && "Cannot dereference a void pointer!");
    int refsize = ref->getType()->getSize();

    if(!index) {
        // There is no index. No math needed :)
        *out << Command("mov", refsize)(Reg("rax"))("%ax", refsize);
        return;
    }

    // Save base address in rbx
    *out << Command("subq")("$8")("%rsp");
    *out << Command("pushq")("%rbx");
    *out << Command("movq")("%rax")("%rbx");

    // Evaluate offset
    index->gen(out);
    // int indsize = index->getType()->getSize();

    // If the pointed-at type is 1, 2, 4 or 8 bytes in size,
    // we can use the offset multiplier. Otherwise, we have
    // to use a multiplication operation.
    if(refsize == 1 || refsize == 2 || refsize == 4 || refsize == 8)
        *out << Command("mov", refsize)(Reg("rbx") + Reg("rax") * refsize)("%ax", refsize);
    else {
        // Multiply by refsize (TODO is this legal? I don't really think so..)
        *out << Command("subq")("$8")("%rsp");
        *out << Command("pushq")("%rdx");
        *out << Command("imul")(refsize)("%eax");
        *out << Command("popq")("%rdx");
        *out << Command("addq")("$8")("%rsp");
        *out << Command("mov", refsize)(Reg("rbx") + Reg("rax"))("%ax", refsize);
    }

    *out << Command("popq")("%rbx");
    *out << Command("addq")("$8")("%rsp");
}

std::vector<Node**> Expr_Deref::getChildren()
{
    std::vector<Node**> result;
    result.push_back((Node**)&sub);
    result.push_back((Node**)&index);
    return result;
}

Node* Expr_Deref::clone()
{
    return new Expr_Deref((Expression*)sub->clone(), (Expression*)index->clone());
}

void Expr_Struc::gen(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Struc::gen()", this);
    // Get a pointer to top of the struct
    sub->genLeft(out);
    // Get offset and size of the variable within the struct
    Address reg = var->getAddress();
    int size = var->getSize();
    // Return the var (with appropiate mov command) in %rax
    *out << Command("mov", size)(reg)("%ax", size);
}

Node* Expr_Struc::clone()
{
    return new Expr_Struc((Expression*)sub->clone(), new std::string(var->getIdentifier()));
}

void Expr_Struc::genLeft(CodeGen* out) {
    *out << Message(DEBUG, "Expr_Struc::genLeft()", this);
    // Get a pointer to top of the struct
    sub->genLeft(out);
    // And an offset from the base of the struct
    Address reg = var->getAddress();
    // If there is an offset, add it to the pointer value
    if(reg.isDisplaced())
        *out << Command("leaq")(reg)("%rax");
}
