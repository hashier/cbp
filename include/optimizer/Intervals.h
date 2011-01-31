#ifndef INTERVALS_H
#define INTERVALS_H


#include <ostream>

class Interval {

public:
    Interval()
        : low(1), up(0) {}
        
   Interval(int val)
        : low(val), up(val) {}

    Interval(int low_, int up_)
        : low(low_), up(up_) {}
    
    // rounds:
    // [a] = [⌊a⌋, ⌈a⌉]
    // [a,b] = [⌊a⌋, ⌈b⌉]
    Interval(double val);
    Interval(double low_, double up_);
        
    bool empty() const {
        return up < low;
    }
    
    int lower() const {
        return low;
    }
    
    int upper() const {
        return up;
    }
    
    static Interval world();
    
    bool operator==(Interval const& rhs) const {
        return lower() == rhs.lower() && upper() == rhs.upper();
    }

private:
    int low;
    int up;
};

Interval operator+ (Interval const& lhs, Interval const& rhs);
Interval operator- (Interval const& lhs, Interval const& rhs);
Interval operator* (Interval const& lhs, Interval const& rhs);
Interval operator/ (Interval const& lhs, Interval const& rhs);

std::ostream& operator<<(std::ostream& os, Interval const& i);

#endif // INTERVALS_H

