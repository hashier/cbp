#include "Mnemonics.h"

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

std::ostream& operator<<(std::ostream& os, Message const& message){
    return message.write(os);
}
