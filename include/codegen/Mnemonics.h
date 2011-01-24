#pragma once

/// This file provides an abstraction (not very high, just strings) for mnemonics.
/// The purpose for now is to have the same intention everywhere.

#include <ostream>
#include <string>
#include <sstream>
#include <cassert>

#include "CodeGen.h"
#include "MsgHandler.h"

class CodeGen;

class Mnemonic {
public:
    virtual std::ostream& write(std::ostream& os) const = 0;
    virtual std::ostream& writeUnformatted(std::ostream& os) const = 0;
    virtual bool needsDebugInfo() const{
    	return false;
    }
};

CodeGen& operator<<(CodeGen& cg, Mnemonic const& mnemonic);

//class CodeGen;

class DebuggableMnemonic : public Mnemonic {
public:
    virtual std::ostream& write(std::ostream& os) const {
    	assert(false);
    	return os;
    }
    virtual std::ostream& writeUnformatted(std::ostream& os) const {
    	assert(false);
    	return os;
    }

    virtual std::ostream& write(std::ostream& os, CodeGen const& cg) const = 0;
    virtual std::ostream& writeUnformatted(std::ostream& os, CodeGen const& cg) const = 0;

	bool needsDebugInfo() const{
		return true;
	}
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

    /**
     * @param regName_ Element of {ax,bx,cx,dx}
     * @param size     size of the register to use in bytes
     */
    explicit Reg(std::string const& regName_, int size) {
        if(regName_[0] == '%'){
            regName = regName_;
        }
        else{
            regName = '%' + regName_;
        }

        switch(size) {
            case 1: regName[2] ='l'; break; // rename last letter from x to l
            case 2: /* default size, leave as-is. */ break;
            case 4: regName[0] = 'e'; regName = '%' + regName; break;
            case 8: regName[0] = 'r'; regName = '%' + regName; break;
            default: assert("Invalid dynamic register size!");
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
        Address a;
        
        assert(lhs.displacement == 0 || rhs.displacement == 0);
        a.displacement = (lhs.displacement == 0) ? rhs.displacement : lhs.displacement;

        int slots = 2;
        if(!rhs.offset.empty() && rhs.multiplier) {
            a.offset = rhs.offset;
            a.multiplier = rhs.multiplier;
            --slots;
        }
        if(!lhs.offset.empty() && lhs.multiplier) {
            a.offset = lhs.offset;
            a.multiplier = lhs.multiplier;
            --slots;
        }

        if(!rhs.offset.empty() && !rhs.multiplier) {
            (slots-- == 2? a.offset: a.base) = rhs.offset;
        }
        if(!lhs.offset.empty() && !lhs.multiplier) {
            (slots-- == 2? a.offset: a.base) = lhs.offset;
        }

        if(!rhs.base.empty()) {
            (slots-- == 2? a.offset: a.base) = rhs.base;
        }
        if(!lhs.base.empty()) {
            (slots-- == 2? a.offset: a.base) = lhs.base;
        }

        assert(slots >= 0);

        return a;
    }

    bool isDisplaced() const{
        return displacement == 0;
    }

    std::ostream& write(std::ostream& os) const {
        // do some checks, should mostly be covered at compile time
        assert((!base.empty() || !offset.empty()) && "One register of address has to be specified");
        assert((!offset.empty() || multiplier == 0) && "To use the multiplier the offset register has to be specified");

        if(displacement != 0){
            os << displacement;
        }

        // We always want the address! Otherwise we wouldn't use the Address class, would we?
        // bool brackets = (displacement != 0 || !offset.empty());
        // if(brackets){
            os << '(';
        // }

        if(!base.empty()){
            os << base;
        }

        if(!offset.empty()){
            os << ',' << offset;
        }

        if(multiplier != 0){
            os << ',' << multiplier;
        }

        // if(brackets){
            os << ')';
        // }

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
    Command(Command const& other)
        :gotFirstArg(other.gotFirstArg){
        command << other.command.str();
    }

    explicit Command(std::string const& command_)
        : gotFirstArg(false) {
        assert(command_[0] != '.');

        command << command_;
    }

    explicit Command(std::string const& command_, int size)
        : gotFirstArg(false) {
        assert(command_[0] != '.');

        command << command_;

        switch(size) {
            case 1: command << "b"; break;
            case 4: command << "l"; break;
            case 8: command << "q"; break;
            default: assert("Invalid size specified!");
        }
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
        //assert(reg[0] == '%'); // can't do this, reg can be name of a global variable
        assert(reg[reg.size() - 1] != ',');

        preformat();
        command << reg;
        return *this;
    }

    /**
     * @param regName_ Element of {ax,bx,cx,dx}
     * @param size     size of the register to use in bytes
     */
    Command& operator()(std::string const& regName_, int size){
        assert(regName_[0] == '%');
        assert(regName_[regName_.size() - 1] != ',');

        preformat();

        std::string tmp(regName_);

        switch(size) {
            case 1: tmp[2] = 'l'; break; // rename last letter from x to l
            case 2: /* default size, leave as-is. */ break;
            case 4: tmp[0] = 'e'; tmp = '%' + tmp; break;
            case 8: tmp[0] = 'r'; tmp = '%' + tmp; break;
            default: assert("Invalid size specified!");
        }

        command << tmp;
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
    Directive(Directive const& other){
        directive << other.directive.str();
    }

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
