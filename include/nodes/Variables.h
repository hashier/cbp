#pragma once

#include "AbstractNodes.h"

class Type;

class Variable : public Declaration {
    public:
        Variable(std::string* identifier, Type* type);
        virtual void dump(int num = 0);
        Type* getType() { return type; }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getMemoryOffset();
        virtual int getSize();
        virtual void gen(CodeGen* out);
    protected:
        Type* type;
        int offset;
};

class GlobalVariable : public Variable {
    public:
        GlobalVariable(std::string* identifier, Type* type);
};

class LocalVariable : public Variable {
    public:
        LocalVariable(std::string* identifier, Type* type);
};

class StructVariable : public Variable {
    public:
        StructVariable(std::string* identifier, Type* type);
        inline void setExplicitOffset(int explicitOffset) {
            this->explicitOffset = explicitOffset;
        }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getMemoryOffset();
        void dump(int num = 0);
    private:
        int explicitOffset;
};

