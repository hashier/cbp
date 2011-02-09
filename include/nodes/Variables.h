#pragma once

#include "AbstractNodes.h"
#include "Intervals.h"

class Type;

class Variable : public Declaration {
    public:
        Variable(std::string* identifier, Type* type);
        Variable(Variable *node);
        virtual void dump(int num = 0);

        virtual void constProp() { };

        Type* getType() { return type; }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual int getSize();
        virtual void gen(CodeGen* out);
        virtual Address getAddress();
        
        virtual void solveConstraints(ConstrainedEnvironment& env);
        virtual void replaceChild(Node* oldChild, Node* newChild){ assert(false && "replaceChild not allowed"); }
        virtual std::vector<Node**> getChildren() { return std::vector<Node**>(); }
    
    protected:
        virtual int getMemoryOffset();
        Type* type;
        int offset;
};

class GlobalVariable : public Variable {
    public:
        GlobalVariable(std::string* identifier, Type* type);
        GlobalVariable(GlobalVariable* node);
        virtual Address getAddress();
        virtual Node* clone();
};

class LocalVariable : public Variable {
    public:
        LocalVariable(std::string* identifier, Type* type);
        LocalVariable(LocalVariable* node);
        virtual Node* clone();
};

class StructVariable : public Variable {
    public:
        StructVariable(std::string* identifier, Type* type);
        StructVariable(StructVariable *node);
        inline void setExplicitOffset(int explicitOffset) {
            this->explicitOffset = explicitOffset;
        }
        virtual int setStackOffset(int offset, bool offBySize = true);
        virtual Address getAddress();
        void dump(int num = 0);
        int getExplicitOffset() { return explicitOffset; };
        virtual Node* clone();
    protected:
        int explicitOffset;
        virtual int getMemoryOffset();
};

