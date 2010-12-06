/// This file provides an abstraction (not very high, just strings) for mnemonics.
/// The purpose for now is to have the same indention everywhere.

#include <ostream>
#include <string>
#include <sstream>
#include <cassert>

class Mnemonic {
public:
    virtual std::ostream& write(std::ostream& os) const = 0;
    virtual std::ostream& writeUnformatted(std::ostream& os) const = 0;
};

class Nothing : public Mnemonic {
public:
    explicit Nothing(std::string const& nodeId_)
        : nodeId(nodeId_) {}
        
    std::ostream& write(std::ostream& os) const {
        return os << nodeId << ": to be implemented!" << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return write(os);
    }

private:
    std::string nodeId;
};

class Label : public Mnemonic {
public:
    Label(){}
    
    explicit Label(std::string const& label_)
        : label(label_) {}
    
    std::ostream& write(std::ostream& os) const {
        return os << label << ':' << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return os << label;
    }

private:
    std::string label;
};

// Automatic generation of addresses
// Addresses are calculated by base + displacement + offset * multiplier
// where base and offset are registers and displacement and multiplier
// are integers.
// Allowed forms:
//  - base
//  - base + displacement
//  - base + displacement + offset
//  - base + displacement + offset * multiplier
//  - base + offset * multiplier
//  - base + offset
//  - displacement + offset * multiplier
//  - offset * multiplier
// These are enforced by the accepted parameters of the corresponding
// operators. All others shouldn't compile.

class Reg {
public:
    Reg() {}
    
    explicit Reg(std::string const& regName_) {
        if(regName_[0] == '%'){
            regName = regName_;
        }
        else{
            regName = '%' + regName_;
        }
    }
    
    bool empty() const{
        return regName.empty();
    }
    
    std::ostream& write(std::ostream& os) const {
        return os << regName;
    }

private:
    std::string regName;
};

std::ostream& operator<<(std::ostream& os, Reg const& r);

class Address {
public:
    Address()
        : displacement(0),
        multiplier(0){}
        
    //  - base
    Address(Reg const& base_)
        : base(base_),
        displacement(0),
        multiplier(0){}
        
    static Address offsetAddress(Reg const& offset, int multiplier){
        Address a;
        a.offset = offset;
        a.multiplier = multiplier;
        return a;
    }
    
    static Address displaced(Address const& base, int displacement){
        assert(base.displacement == 0);
        
        Address a = base;
        a.displacement = displacement; // maybe += to allow calculations?
        return a;
    }
    
    static Address combine(Address const& lhs, Address const& rhs){
        // check, that only one exists
        assert(lhs.base.empty() || rhs.base.empty());
        assert(lhs.displacement == 0 || rhs.displacement == 0);
        assert(lhs.offset.empty() || rhs.offset.empty());
        assert(lhs.multiplier == 0 || rhs.multiplier == 0);
    
        Address a;
        a.base = lhs.base.empty() ? rhs.base : lhs.base;
        a.displacement = (lhs.displacement == 0) ? rhs.displacement : lhs.displacement;
        a.offset = lhs.offset.empty() ? rhs.offset : lhs.offset;
        a.multiplier = (lhs.multiplier == 0) ? rhs.multiplier : lhs.multiplier;
        return a;
    }
    
    std::ostream& write(std::ostream& os) const {
        // do some checks, should mostly be covered at compile time
        assert((!base.empty() || !offset.empty()) && "One register of address has to be specified");
        assert((!offset.empty() || multiplier == 0) && "To use the multiplier the offset register has to be specified");
        
        if(displacement != 0){
            os << displacement;
        }
        
        bool brackets = (displacement != 0 || !offset.empty());
        if(brackets){
            os << '(';
        }
        
        if(!base.empty()){
            os << base;
        }
        
        if(!offset.empty()){
            os << ',' << offset;
        }
        
        if(multiplier != 0){
            os << ',' << multiplier;
        }
        
        if(brackets){
            os << ')';
        }
        
        return os;
    }
    
private:
    Reg base;
    int displacement;
    Reg offset;
    int multiplier;
};

//  - base + displacement + offset
//  - base + displacement + offset * multiplier
//  - base + offset * multiplier
//  - base + offset
Address operator+(Address const& base, Address const& offset);

//  - base + displacement
//  - offset * multiplier + displacement
Address operator+(Address const& base, int displacement);

//  - displacement + base
//  - displacement + offset * multiplier
Address operator+(int displacement, Address const& base);

//  - offset * multiplier
Address operator*(Reg const& lhs, int rhs);

class Command : public Mnemonic {
public:        
    explicit Command(std::string const& command_)
        : gotFirstArg(false) {
        assert(command_[0] != '.');
        
        command << command_;    
    }
    
private:
    void preformat(){
        if(gotFirstArg){
            command << ',';
        }
        else{
            command << ' ';
            gotFirstArg = true;
        }
    }
    
public:
        
    /// Semantic:
    /// appends param as it is to the current command,
    /// inserting a space between them (nothing more).
    Command& operator()(std::string const& reg){
        assert(reg[0] == '%');
        assert(reg[reg.size() - 1] != ',');
        
        preformat();
        command << reg;
        return *this;
    }
    
    /// Semantic:
    /// appends arg declared as numeric constant (with $) to the current command.
    Command& operator()(int arg){
        preformat();
        command << '$' << arg;
        return *this;
    }
    
    /// Semantic:
    /// appends addr to the current command.
    Command& operator()(Address const& addr){
        preformat();
        addr.write(command);
        return *this;
    }
    
    /// Semantic:
    /// appends label as it is to the current command,
    /// inserting a space between them (nothing more).
    Command& operator()(Label const& label){
        preformat();
        label.writeUnformatted(command);
        return *this;
    }
    
    std::ostream& write(std::ostream& os) const {
        return os << "    " << command.str() << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return os << command.str();
    }

private:
    std::stringstream command;
    bool gotFirstArg;
};

// use this to get ostream formatting
class Directive : public Mnemonic {
public:        
    explicit Directive(std::string const& directive_){
        directive << directive_;    
    }
            
    template<typename T>
    Directive& operator<<(T const& v) {
        directive << v;
        return *this;
    }
    
    std::ostream& write(std::ostream& os) const {
        return os << "    " << directive.str() << std::endl;
    }
    
    std::ostream& writeUnformatted(std::ostream& os) const {
        return os << directive.str();
    }

private:
    std::stringstream directive;
};

