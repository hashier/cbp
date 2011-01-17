#include "Mnemonics.h"

CodeGen& operator<<(CodeGen& cg, Mnemonic const& mnemonic){
    // not very pretty, but functional to separate normal mnemonics
    // and those which need debugging infos
    if(mnemonic.needsDebugInfo()){
        dynamic_cast<DebuggableMnemonic const&>(mnemonic).write(cg.outputStream(), cg);
    }
    else{
        mnemonic.write(cg.outputStream());
    }
    return cg;
}

std::ostream& operator<<(std::ostream& os, Reg const& r){
    return r.write(os);
}

//  - base + displacement + offset
//  - base + displacement + offset * multiplier
//  - base + offset * multiplier
//  - base + offset
Address operator+(Address const& base, Address const& offset){
    return Address::combine(base, offset);
}

//  - base + displacement
//  - offset * multiplier + displacement
Address operator+(Address const& base, int displacement){
    return Address::displaced(base, displacement);
}

//  - displacement + base
//  - displacement + offset * multiplier
Address operator+(int displacement, Address const& base){
    return Address::displaced(base, displacement);
}

//  - offset * multiplier
Address operator*(Reg const& lhs, int rhs){
    return Address::offsetAddress(lhs, rhs);
}
