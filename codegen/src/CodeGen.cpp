#include"CodeGen.h"

int Mark::counter = 0;

Mark CodeGen::mark(std::string name) {
    if(marks.count(name) == 0) {
        marks[name] = new Mark(name);
    }
    return *(marks[name]);
}
