#pragma once

#include "AbstractNodes.h"

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
        
        Interval getInterval() const {
            return interval;
        }
        
        void setInterval(Interval const& i){
            interval = i;
        }
        
        virtual void solveConstraints(/*SymbolTable*/);
    
    protected:
        virtual int getMemoryOffset();
        Type* type;
        int offset;
        Interval interval;
};

class GlobalVariable : public Variable {
    public:
        GlobalVariable(std::string* identifier, Type* type);
        GlobalVariable(GlobalVariable* node);
        virtual Address getAddress();
        std::vector<Node**> getChildren();
        virtual Node* clone();
};

class LocalVariable : public Variable {
    public:
        LocalVariable(std::string* identifier, Type* type);
        LocalVariable(LocalVariable* node);
        std::vector<Node**> getChildren();
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
        std::vector<Node**> getChildren();
        virtual Node* clone();
    protected:
        int explicitOffset;
        virtual int getMemoryOffset();
};

