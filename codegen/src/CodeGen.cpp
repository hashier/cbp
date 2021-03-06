#include"CodeGen.h"

#include<sstream>

Label CodeGen::newMark(std::string name, bool func) {
    std::stringstream ss;
    ss << (func ? "" : ".L") << name << (mark_counter++);
    return Label(ss.str());
}
