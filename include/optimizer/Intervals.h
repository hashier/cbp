#ifndef INTERVALS_H
#define INTERVALS_H

#include <ostream>

class Interval {
public:
    typedef unsigned long long Integer;

    // construct ∅
    Interval();        
    // construct [a,a]
    Interval(Integer val);
    // construct [a,b]
    Interval(Integer low_, Integer up_);
    
    // rounds:
    // [a] = [⌊a⌋, ⌈a⌉]
    // [a,b] = [⌊a⌋, ⌈b⌉]
    Interval(double val);
    Interval(double low_, double up_);
    
    // construct [-∞,+∞]
    static Interval world();
    
    // [a,b] == ∅ ?
    bool empty() const {
        return up < low;
    }
    
    // [a,a]?
    bool singleton() const {
        return up == low;
    }
    
    // return a from [a,b]
    Integer lower() const {
        return low;
    }
    
    // return b from [a,b]
    Integer upper() const {
        return up;
    }
    
    // [a,b] == [c,d] ?
    bool operator==(Interval const& rhs) const;
    bool operator!=(Interval const& rhs) const;

private:
    // property: if *this == ∅ then low = 1 and up = 0
    Integer low;
    Integer up;
};

// interval arithmetic
Interval operator+ (Interval const& lhs, Interval const& rhs);
Interval operator- (Interval const& lhs, Interval const& rhs);
Interval operator* (Interval const& lhs, Interval const& rhs);
Interval operator/ (Interval const& lhs, Interval const& rhs);

// set operations
// x ∈ [a,b] ?
bool in(Interval::Integer x, Interval const& i);
// A ∩ B
Interval operator& (Interval const& lhs, Interval const& rhs);

// A = [a,b], B = [c,d]
// A ∩ (B ∪ [-∞,c])
Interval restrictLeft(Interval const& lhs, Interval const& rhs);
// A ∩ (B ∪ [d,+∞])
Interval restrictRight(Interval const& lhs, Interval const& rhs);

// I/O
std::ostream& operator<<(std::ostream& os, Interval const& i);

#endif // INTERVALS_H

