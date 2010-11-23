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
