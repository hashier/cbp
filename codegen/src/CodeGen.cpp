#include"CodeGen.h"

#include<sstream>

Label CodeGen::newMark(std::string name) {
    std::stringstream ss;
    ss << ".L" << name << (mark_counter++);
    return Label(ss.str());
}

CodeGen& operator<<(CodeGen& cg, Mnemonic const& mnemonic){
    mnemonic.write(cg.outputStream());
    return cg;
}

