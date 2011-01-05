#include"ExprNodes.h"
#include"Variables.h"
#include"ProgramNodes.h"

void Expr_Cast::genLeft(CodeGen* out) {
    expr->genLeft(out);
}
void Expr_Cast::gen(CodeGen* out) {
    expr->gen(out);
}

void Expr_Mul::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << Command("imull")("%ebx");
    *out << Command("popq")("%rbx");

}
void Expr_Div::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << Command("divl")("%ebx");
    *out << Command("popq")("%rbx");
}
void Expr_Mod::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << Command("pushq")("%rbx");
    *out << Command("movl")("%eax")("%ebx");
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << Command("divl")("%ebx");
    *out << Command("movl")("%edx")("%eax");
    *out << Command("popq")("%rbx");
}

void ConstInt::gen(CodeGen* out) {
    *out << Command("movl")(value)("%eax");
}

void Expr_EQ::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
}

void Expr_NEQ::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
}

void Expr_LT::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
}

void Expr_GT::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
}

void Expr_LE::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
    *out << Command("popq")("%rbx");
}

void Expr_GE::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
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
}

void Expr_BoolOR::gen(CodeGen* out) {
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

void Expr_BoolAND::gen(CodeGen* out) {
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

void Expr_BoolXOR::gen(CodeGen* out) {
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
}

// Shift operators use the 8-bit %cl register, which is equal to the lowest 8 bits of %ecx.
// arithmetic left shift
void Expr_BitLeft::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %rax
    *out << Command("pushq")("%rcx");        // store %rcx including %cl to the stack
    *out << Command("movq")("%rax")("%rcx"); // %rcx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("shl")("%cl")("%rax");   // %rax = %rax << %cl
    *out << Command("popq")("%rcx");         // restore %rcx from the stack
}

// arithmetic right shift
void Expr_BitRight::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %eax
    *out << Command("pushq")("%rcx");        // store %rcx including %cl to the stack
    *out << Command("movq")("%rax")("%rcx"); // %rcx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("shr")("%cl")("%rax");   // %rax = %rax >> %cl
    *out << Command("popq")("%rcx");            // restore %rcx from the stack
}

void Expr_BitOR::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %rax
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("or")("%rbx")("%rax");   // %rax = %rax or %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
}

void Expr_BitAND::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %rax
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("and")("%rbx")("%rax");  // %rax = %rax and %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
}

void Expr_BitXOR::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %rax
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("xor")("%rbx")("%rax");  // %rax = %rax xor %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
}

void Expr_Add::gen(CodeGen* out) {
    // only integer arguments are allowed yet (floats are optional)
    assert(right->getType()->isInteger());
    assert(left->getType()->isInteger());
    // both arguments must have a signed bit or both arguments must have not a signed bit
    assert(left->getType()->hasSignedBit()==right->getType()->hasSignedBit());

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

    std::string movSmall2Big("movsx");
    if (!smallExp->getType()->hasSignedBit()) {
        movSmall2Big[3] = 'z'; // "movzx"
    }
    smallExp->gen(out);                      // right hand operand into %rax
    *out << Command(movSmall2Big)            // expanding bits:
            ("%ax", smallSizeInBytes)        //  argument with smaller size expanding to
            ("%ax", bigSizeInBytes);         //  to size of argument with bigger size
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    bigExp->gen(out);                        // left hand operand into %rax
    *out << Command("add")                   // %ax = %ax + %bx:
            ("%bx", bigSizeInBytes)          //  argument with smaller size adding to
            ("%ax", bigSizeInBytes);         //  argument with bigger size
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
}

void Expr_Sub::gen(CodeGen* out) {
    right->gen(out);                         // right hand operand into %eax
    *out << Command("pushq")("%rbx");        // store %rbx to the stack
    *out << Command("movq")("%rax")("%rbx"); // %rbx = %rax
    left->gen(out);                          // left hand operand into %rax
    *out << Command("sub")("%rbx")("%rax");  // %rax = %rax - %rbx
    *out << Command("popq")("%rbx");         // restore %rbx from the stack
}

void Expr_Identifier::genLeft(CodeGen* out) {
    *out << Command("leaq")(ref->getAddress())("%rax");
}
void Expr_Identifier::gen(CodeGen* out) {
    int size = ref->getSize();
    *out << Command("mov", size)(ref->getAddress())("%ax", size);
}

void Expr_Assign::gen(CodeGen* out) {
    *out << Command("pushq")("%rbx");           // store ebx to the stack
    left->genLeft(out);
    *out << Command("movq")("%rax")("%rbx");   // %ebx = %eax
    right->gen(out);
    int size = right->getType()->getSize();
    *out << Command("mov", size)("%ax", size)(Reg("rbx"));   // %ebx = %eax
    *out << Command("popq")("%rbx");            // restore %ebx from the stack
}

void FuncCall::gen(CodeGen *out) {
    std::list<Expression*>::iterator it;
    for ( it = arguments->begin() ; it != arguments->end(); it++ ) {
        (*it)->gen(out);
        *out << Command("pushq")("%rax");
    }
    *out << Command("call")(func->getMark(out));
}

void Expr_Ref::gen(CodeGen* out) {
    sub->genLeft(out);
}

void Expr_Deref::genLeft(CodeGen* out) {
    // Evaluate substatement (= base address)
    sub->gen(out);
    if(index) {
        // Evaluate offset
        *out << Command("pushq")("%rbx");
        index->gen(out);
        *out << Command("movl")("%eax")("%ebx");
        // Return value of variable at address
        *out << Command("leaq")(Reg("rax") + Reg("ebx"))("%rax");
    } else
        *out << Command("leaq")(Reg("rax"))("%rax");
}

void Expr_Deref::gen(CodeGen* out) {
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
        *out << Command("imul")(refsize);
        *out << Command("mov", refsize)(Reg("rbx") + Reg("rax"))("%ax", refsize);
    }

    *out << Command("popq")("%rbx");
}

void Expr_Struc::gen(CodeGen* out) {
    // Get a pointer to top of the struct
    sub->genLeft(out);
    // Get offset and size of the variable within the struct
    Address reg = var->getAddress();
    int size = var->getSize();
    // Return the var (with appropiate mov command) in %rax
    *out << Command("mov", size)(reg)("%ax", size);
}

void Expr_Struc::genLeft(CodeGen* out) {
    // Get a pointer to top of the struct
    sub->genLeft(out);
    // And an offset from the base of the struct
    Address reg = var->getAddress();
    // If there is an offset, add it to the pointer value
    if(reg.isDisplaced())
        *out << Command("leaq")(reg)("%rax");
}
