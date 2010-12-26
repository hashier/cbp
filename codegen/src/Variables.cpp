#include "Variables.h"
#include "ProgramNodes.h"

int Variable::setStackOffset(int offset, bool offBySize) {
    this->offset = offset +(offBySize ? -getSize() : 0);
    return -getSize();
}

int Variable::getMemoryOffset() {
    return offset;
}

int Variable::getSize() {
    return type->getSize();
}

int StructVariable::setStackOffset(int offset, bool dummy) {
    if(explicitOffset < 0)
        this->offset = offset;
    return getSize();
}

int StructVariable::getMemoryOffset() {
    return explicitOffset < 0 ? offset : explicitOffset;
}

Address Variable::getAddress() {
    return Reg("rbp") + getMemoryOffset();
}
