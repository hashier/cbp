/*
 * Message.cpp
 *
 *  Created on: 17.01.2011
 *      Author: Remo
 */

#include "Message.h"

Message::Message() {}
Message::Message(const Verbosity& verbosity, const std::string& message, const Node* node)
    : verbosity(verbosity)
    , message(message)
    , lineNumber(1)   // TODO: no line number yet
{
    lineNumber = node->getLineNumber();
}

std::ostream& Message::write(std::ostream& os, const CodeGen& cg) const {
    MsgHandler::getInstance().addMessage(
        message,
        verbosity,
        lineNumber,
        cg.getInputFileName()
    );
    // write to console
    if (verbosity!=DEBUG && verbosity!=DEBUG_EAX) { // write DEBUG information not to console
        MsgHandler::getInstance().writeLastMessage(); // to std::cerr
    }
    // write to assembler file
    os << "// ";
    MsgHandler::getInstance().writeLastMessage(os);
    if (verbosity==DEBUG_EAX) {
        static int labelTextCount = 0;
        os << "// begin DEBUG_EAX" << std::endl;
        os << "	.section .rdata,\"dr\"" << std::endl;
        os << ".LDEBUGEAX_" << labelTextCount << ":" << std::endl;
        os << "	.ascii \"" << message.c_str() << " in line " << lineNumber << "\\0\"" << std::endl;
        os << "	.text" << std::endl;
        os << "	pushq	%rax" << std::endl;
        os << "	pushq	%rbx" << std::endl;
        os << "	pushq	%rcx" << std::endl;
        os << "	pushq	%rdx" << std::endl;
        os << "	pushq	%rsi" << std::endl;
        os << "	pushq	%rdi" << std::endl;
        os << "	pushq	%r8" << std::endl;
        os << "	pushq	%r9" << std::endl;
        os << "	pushq	%r10" << std::endl;
        os << "	pushq	%r11" << std::endl;
        os << "	leaq	.LDEBUGEAX_" << labelTextCount << "(%rip), %rdx" << std::endl;
        os << "	movl	%eax, %ecx" << std::endl;
        if (cg.isWithUnderscore()) {
            os << "	call	_print_eax" << std::endl;
        } else {
            os << "	call	print_eax" << std::endl;
        }
        os << "	popq	%r11" << std::endl;
        os << "	popq	%r10" << std::endl;
        os << "	popq	%r9" << std::endl;
        os << "	popq	%r8" << std::endl;
        os << "	popq	%rdi" << std::endl;
        os << "	popq	%rsi" << std::endl;
        os << "	popq	%rdx" << std::endl;
        os << "	popq	%rcx" << std::endl;
        os << "	popq	%rbx" << std::endl;
        os << "	popq	%rax" << std::endl;
        os << "// end DEBUG_EAX" << std::endl;
        labelTextCount++;
    }
    return os;
}

std::ostream& Message::writeUnformatted(std::ostream& os, const CodeGen& cg) const {
    return write(os, cg);
}
