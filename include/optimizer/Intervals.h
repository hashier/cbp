#ifndef INTERVALS_H
#define INTERVALS_H

#include <limits>
#include <algorithm>
#include <cmath>
#include <ostream>

namespace Intervals {

namespace detail {
    template<typename T>
    T const& min(T const& t1, T const& t2, T const& t3, T const& t4){
        return (std::min)(t1, (std::min)(t2, (std::min)(t3, t4)));
    }
    
    template<typename T>
    T const& max(T const& t1, T const& t2, T const& t3, T const& t4){
        return (std::max)(t1, (std::max)(t2, (std::max)(t3, t4)));
    }
}

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

template<typename T>
Interval<T> operator- (Interval<T> const& lhs, Interval<T> const& rhs){
    return Interval<T>(lhs.lower() - rhs.lower(), lhs.upper() - rhs.upper());
}

template<typename T>
Interval<T> operator* (Interval<T> const& lhs, Interval<T> const& rhs){
    // [a,b] * [c,d] = [min (ac, ad, bc, bd), max (ac, ad, bc, bd)]
    return Interval<T>(
        (detail::min)(
            lhs.lower() * rhs.lower(),
            lhs.lower() * rhs.upper(),
            lhs.upper() * rhs.lower(),
            lhs.upper() * rhs.upper()
        ),
        (detail::max)(
            lhs.lower() * rhs.lower(),
            lhs.lower() * rhs.upper(),
            lhs.upper() * rhs.lower(),
            lhs.upper() * rhs.upper()
        )
    );
}

template<typename T>
Interval<T> operator/ (Interval<T> const& lhs, Interval<T> const& rhs){
    // [a,b] / [c,d] = [min (a/c, a/d, b/c, b/d), max (a/c, a/d, b/c, b/d)]
    double a = static_cast<double>(lhs.lower());
    double b = static_cast<double>(lhs.upper());
    double c = static_cast<double>(rhs.lower());
    double d = static_cast<double>(rhs.upper());
    
    return Interval<T>(
        (detail::min)(
            std::floor(a / c),
            std::floor(a / d),
            std::floor(b / c),
            std::floor(b / d)
        ),
        (detail::max)(
            std::ceil(a / c),
            std::ceil(a / d),
            std::ceil(b / c),
            std::ceil(b / d)
        )
    );
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
