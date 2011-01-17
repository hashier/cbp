#pragma once

#include "AbstractNodes.h"

class Type;

class Variable : public Declaration {
    public:
        Variable(std::string* identifier, Type* type);
        virtual void dump(int num = 0);

		virtual void constProp() { };

        Type* getType() { return type; }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getSize();
        virtual void gen(CodeGen* out);
        virtual Address getAddress();
    protected:
        virtual int getMemoryOffset();
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
        virtual Address getAddress();
        void dump(int num = 0);
        int getExplicitOffset() { return explicitOffset; }
    protected:
        int explicitOffset;
        virtual int getMemoryOffset();
};

