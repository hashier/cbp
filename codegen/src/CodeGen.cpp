#include"CodeGen.h"

#include<sstream>

Label CodeGen::newMark(std::string name, bool func) {
    std::stringstream ss;
    ss << (func ? "" : ".L") << name << (mark_counter++);
    return Label(ss.str());
}

CodeGen& operator<<(CodeGen& cg, Mnemonic const& mnemonic){
    mnemonic.write(cg.outputStream());
    return cg;
}

