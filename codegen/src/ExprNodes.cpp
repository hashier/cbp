#include"ExprNodes.h"

void Expr_Mul::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "push  %ebx" << std::endl;
    *out << "mov %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "imull %ebx, %eax" << std::endl;
    *out << "pop  %ebx" << std::endl;

}
void Expr_Div::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "push  %ebx" << std::endl;
    *out << "mov %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "divl %ebx, %eax" << std::endl;
    *out << "pop  %ebx" << std::endl;
}
void Expr_Mod::gen(CodeGen* out) {
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "push  %ebx" << std::endl;
    *out << "mov %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // multiply
    *out << "modl %ebx, %eax" << std::endl;
    *out << "pop  %ebx" << std::endl;
}

void ConstInt::gen(CodeGen* out) {
    *out << "mov $" << value << ", %eax" << std::endl;
}

void Expr_EQ::gen(CodeGen* out) {
    Mark labelEQ = out->newMark("EQ");
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    *out << "je " << labelEQ << std::endl;
    *out << "movl $0, %eax" << std::endl;
    *out << labelEQ << ":" << std::endl;
    *out << "movl $1, %eax" << std::endl;
    *out << "pop %ebx" << std::endl;
}

void Expr_NEQ::gen(CodeGen* out) {
    Mark labelEQ = out->newMark("EQ");
    // left hand operand into %eax
    left->gen(out);
    // save to local var
    *out << "pushl  %ebx" << std::endl;
    *out << "movl %eax, %ebx" << std::endl;
    // right operand into %eax
    right->gen(out);
    // compare
    *out << "cmpl %eax, %ebx" << std::endl;
    *out << "jne " << labelEQ << std::endl;
    *out << "movl $0, %eax" << std::endl;
    *out << labelEQ << ":" << std::endl;
    *out << "movl $1, %eax" << std::endl;
    *out << "pop %ebx" << std::endl;
}

void Expr_LT::gen(CodeGen* out) {
}

void Expr_GT::gen(CodeGen* out) {
}

void Expr_LE::gen(CodeGen* out) {
}

void Expr_GE::gen(CodeGen* out) {
}

void Expr_BoolOR::gen(CodeGen* out) {
}

void Expr_BoolAND::gen(CodeGen* out) {
}

void Expr_BoolXOR::gen(CodeGen* out) {
}

