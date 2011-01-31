#ifndef INTERVALS_H
#define INTERVALS_H

#include <ostream>

class Interval {

public:
    // construct ∅
    Interval()
        : low(1), up(0) {}
        
   // construct [a,a]
   Interval(int val)
        : low(val), up(val) {}

    // construct [a,b]
    Interval(int low_, int up_)
        : low(low_), up(up_) {}
    
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
    
    // return a from [a,b]
    int lower() const {
        return low;
    }
    
    // return b from [a,b]
    int upper() const {
        return up;
    }
    
    // [a,b] == [c,d] ?
    bool operator==(Interval const& rhs) const {
        return lower() == rhs.lower() && upper() == rhs.upper();
    }

private:
    int low;
    int up;
};

// interval arithmetic
Interval operator+ (Interval const& lhs, Interval const& rhs);
Interval operator- (Interval const& lhs, Interval const& rhs);
Interval operator* (Interval const& lhs, Interval const& rhs);
Interval operator/ (Interval const& lhs, Interval const& rhs);

// I/O
std::ostream& operator<<(std::ostream& os, Interval const& i);

#endif // INTERVALS_H

