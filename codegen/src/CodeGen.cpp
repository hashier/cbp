#include"CodeGen.h"

#include<sstream>

Mark CodeGen::newMark(std::string name) {
    std::stringstream ss;
    ss << name << (mark_counter++);
    return ss.str();
}

