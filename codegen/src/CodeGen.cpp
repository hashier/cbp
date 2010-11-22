#include"CodeGen.h"

#include<sstream>

Mark CodeGen::newMark(std::string name) {
    std::stringstream ss(name);
    ss << (mark_counter++);
    return ss.str();
}

