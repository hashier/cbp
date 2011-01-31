#ifndef INTERVALS_H
#define INTERVALS_H

#include <limits>
#include <ostream>

namespace Intervals {

template <typename T>
class Interval {

public:
    Interval()
        : low(static_cast<T>(1)), up(static_cast<T>(0)) {}
        
   Interval(T val)
        : low(val), up(val) {}

    Interval(T low_, T up_)
        : low(low_), up(up_) {}
        
    bool empty() const {
        return up < low;
    }
    
    T lower() const {
        return low;
    }
    
    T upper() const {
        return up;
    }
    
    static Interval world() {
        return Interval(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
    }
    
    bool operator==(Interval const& rhs) const {
        return lower() == rhs.lower() && upper() == rhs.upper();
    }

private:
    T low;
    T up;
};

template<typename T>
Interval<T> operator+ (Interval<T> const& lhs, Interval<T> const& rhs){
    return Interval<T>(lhs.lower() + rhs.lower(), lhs.upper() + rhs.upper());
}

}

template<typename T>
std::ostream& operator<<(std::ostream& os, Intervals::Interval<T> const& i){
    if(i == Intervals::Interval<T>::world()){
        return os << "[-∞,+∞]";
    }
    else{
        return os << '[' << i.lower() << ',' << i.upper() << ']';
    }
}

#endif // INTERVALS_H
