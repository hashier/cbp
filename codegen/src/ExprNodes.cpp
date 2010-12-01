#include"ExprNodes.h"

void Expr_Mul::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "imull %ebx, %eax" << std::endl;
    *out << "popl  %ebx" << std::endl;

}
void Expr_Div::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "divl %ebx, %eax" << std::endl;
    *out << "popl  %ebx" << std::endl;
}
void Expr_Mod::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "modl %ebx, %eax" << std::endl;
    *out << "popl  %ebx" << std::endl;
}

void ConstInt::gen(CodeGen* out) {
    *out << "movl $" << value << ", %eax" << std::endl;
}

void Expr_EQ::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if equal
    *out << "sete %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_NEQ::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if not equal
    *out << "setne %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_LT::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if less than
    *out << "setl %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_GT::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if greater than
    *out << "setg %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_LE::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if less than or equal
    *out << "setle %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_GE::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    // set LSB of %ax if greater than or equal
    *out << "setge %al" << std::endl;
    // move from byte %al to long %eax and pad with zeros
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

void Expr_BoolOR::gen(CodeGen* out) {
    Mark labelTrue = out->newMark("true");
    Mark labelFalse = out->newMark("false");
    Mark labelEnd = out->newMark("end");
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0; if not equal, jump to labelTrue
    *out << "cmpl $0, %eax" << std::endl;
    *out << "jne " << labelTrue << std::endl;
    // right operand into %eax
    right->gen(out);
    *out << "cmpl $0, %eax" << std::endl;
    *out << "je " << labelFalse << std::endl;
    // jump mark "true"
    *out << labelTrue << ":" << std::endl;
    *out << "movl $1, %eax" << std::endl;
    *out << "jmp " << labelEnd << std::endl;
    // jump mark "false"
    *out << labelFalse << ":" << std::endl;
    *out << "movl $0, %eax" << std::endl;
    // jump mark "end"
    *out << labelEnd << ":" << std::endl;
}

void Expr_BoolAND::gen(CodeGen* out) {
    Mark labelFalse = out->newMark("false");
    Mark labelEnd = out->newMark("end");
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0; if equal, jump to labelFalse
    *out << "cmpl $0, %eax" << std::endl;
    *out << "je " << labelFalse << std::endl;
    // right hand operand into %eax
    right->gen(out);
    // compare %eax and 0; if equal, jump to labelFalse
    *out << "cmpl $0, %eax" << std::endl;
    *out << "je " << labelFalse << std::endl;
    // if we reached this point, both operands are different from 0
    // thus, we moffel 1 into %eax
    *out << "movl $1, %eax" << std::endl;
    *out << "jmp " << labelEnd << std::endl;
    // jump mark "false"
    *out << labelFalse << ":" << std::endl;
    *out << "movl $0, %eax" << std::endl;
    // jump mark "end"
    *out << labelEnd << ":" << std::endl;
}

void Expr_BoolXOR::gen(CodeGen* out) {
    *out << "pushl %ebx" << std::endl;
    // left hand operand into %eax
    left->gen(out);
    // compare %eax and 0
    *out << "cmpl $0, %eax" << std::endl;
    // set LSB of %bl if equal
    *out << "sete %bl" << std::endl;
    // right hand operand into %eax
    right->gen(out);
    // compare %eax and 0
    *out << "cmpl $0, %eax" << std::endl;
    // set LSB of %bh if equal
    *out << "sete %bh" << std::endl;
    // compare %bl and %bh; if equal, xor returns false
    *out << "cmpl %bh, %bl" << std::endl;
    *out << "sete %al" << std::endl;
    *out << "movzbl %al, %eax" << std::endl;
    *out << "popl %ebx" << std::endl;
}

// Shift operators use the 8-bit %cl register, which is equal to the lowest 8 bits of %ecx.
// arithmetic left shift
void Expr_BitLeft::gen(CodeGen* out) {
    right->gen(out);                          // right hand operand into %eax
    *out << "\tpushl\t%ecx"     << std::endl; // store %ecx including %cl to the stack
    *out << "\tmov\t%eax, %ecx" << std::endl; // %ecx = %eax including %cl = %eax & 0x1F
    left->gen(out);                           // left hand operand into %eax
    *out << "\tsal\t%cl, %eax"  << std::endl; // %eax = %eax << %cl
    *out << "\tpopl\t%ecx"      << std::endl; // restore %ecx from the stack
}

// arithmetic right shift
void Expr_BitRight::gen(CodeGen* out) {
    right->gen(out);                          // right hand operand into %eax
    *out << "\tpushl\t%ecx"     << std::endl; // store %ecx including %cl to the stack
    *out << "\tmov\t%eax, %ecx" << std::endl; // %ecx = %eax including %cl = %eax & 0x1F
    left->gen(out);                           // left hand operand into %eax
    *out << "\tsar\t%cl, %eax"  << std::endl; // %eax = %eax >> %cl
    *out << "\tpopl\t%ecx"      << std::endl; // restore %ecx from the stack
}

void Expr_BitOR::gen(CodeGen* out) {
    right->gen(out);                           // right hand operand into %eax
    *out << "\tpushl\t%ebx"      << std::endl; // store %ebx to the stack
    *out << "\tmovl\t%eax, %ebx" << std::endl; // %ebx = %eax
    left->gen(out);                            // left hand operand into %eax
    *out << "\torl\t%ebx, %eax"  << std::endl; // %eax = %eax or %ebx
    *out << "\tpopl\t%ebx"       << std::endl; // restore %ebx from the stack
}

void Expr_BitAND::gen(CodeGen* out) {
    right->gen(out);                           // right hand operand into %eax
    *out << "\tpushl\t%ebx"      << std::endl; // store ebx to the stack
    *out << "\tmovl\t%eax, %ebx" << std::endl; // %ebx = %eax
    left->gen(out);                            // left hand operand into %eax
    *out << "\tandl\t%ebx, %eax" << std::endl; // %eax = %eax and %ebx
    *out << "\tpopl\t%ebx"       << std::endl; // restore %ebx from the stack
}

void Expr_BitXOR::gen(CodeGen* out) {
    right->gen(out);                           // right hand operand into %eax
    *out << "\tpushl\t%ebx"      << std::endl; // store ebx to the stack
    *out << "\tmovl\t%eax, %ebx" << std::endl; // %ebx = %eax
    left->gen(out);                            // left hand operand into %eax
    *out << "\txorl\t%ebx, %eax" << std::endl; // %eax = %eax xor %ebx
    *out << "\tpopl\t%ebx"       << std::endl; // restore %ebx from the stack
}

void Expr_Add::gen(CodeGen* out) {
    right->gen(out);                           // right hand operand into %eax
    *out << "\tpushl\t%ebx"      << std::endl; // store ebx to the stack
    *out << "\tmovl\t%eax, %ebx" << std::endl; // %ebx = %eax
    left->gen(out);                            // left hand operand into %eax
    *out << "\taddl\t%ebx, %eax" << std::endl; // %eax = %eax + %ebx
    *out << "\tpopl\t%ebx"       << std::endl; // restore %ebx from the stack
}

void Expr_Sub::gen(CodeGen* out) {
    right->gen(out);                           // right hand operand into %eax
    *out << "\tpushl\t%ebx"      << std::endl; // store ebx to the stack
    *out << "\tmovl\t%eax, %ebx" << std::endl; // %ebx = %eax
    left->gen(out);                            // left hand operand into %eax
    *out << "\tsubl\t%ebx, %eax" << std::endl; // %eax = %eax - %ebx
    *out << "\tpopl\t%ebx"       << std::endl; // restore %ebx from the stack
}
