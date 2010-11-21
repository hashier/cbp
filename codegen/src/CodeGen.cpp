#include"CodeGen.h"

int Mark::counter = 0;
std::map<std::string, Mark*> CodeGen::marks;

Mark CodeGen::mark(std::string name) {
    if(marks.count(name) == 0) {
        marks[name] = new Mark(name);
    }
    return *(marks[name]);
}

