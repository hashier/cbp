#include "Intervals.h"

#include <limits>
#include <algorithm>
#include <cmath>

namespace detail {
template<bool>
struct StaticCheck;

template<>
struct StaticCheck<true>{};
}

#define STATIC_CHECK(expr, msg)                     \
    detail::StaticCheck<(expr)> ASSERTION_##msg;    \
    (void)ASSERTION_##msg;

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

Interval::Interval()
    : low(1), up(0) {
    STATIC_CHECK(sizeof(Integer) >= 8, INTEGER_TO_SMALL_MUST_BE_8);
}
    
Interval::Interval(Integer val)
    : low(val), up(val) {}

Interval::Interval(Integer low_, Integer up_)
    : low(low_ <= up_ ? low_ : 1),
    up(up_ >= low_ ? up_ : 0) {}

Interval::Interval(double val)
    : low((Integer)std::floor(val)), up((Integer)std::ceil(val)) {}

Interval::Interval(double low_, double up_)
    : low(low_ <= up_ ? (Integer)std::floor(low_) : 1),
    up(up_ >= low_ ? (Integer)std::ceil(up_) : 0) {}

Interval Interval::world() {
    return Interval(std::numeric_limits<Integer>::min(), std::numeric_limits<Integer>::max());
}

bool Interval::operator==(Interval const& rhs) const {
    return lower() == rhs.lower() && upper() == rhs.upper();
}
bool Interval::operator!=(Interval const& rhs) const {
    return !(*this == rhs);
}

Interval operator+ (Interval const& lhs, Interval const& rhs){
    return Interval(lhs.lower() + rhs.lower(), lhs.upper() + rhs.upper());
}

Interval operator- (Interval const& lhs, Interval const& rhs){
    return Interval(lhs.lower() - rhs.lower(), lhs.upper() - rhs.upper());
}

Interval operator* (Interval const& lhs, Interval const& rhs){
    // [a,b] * [c,d] = [min (ac, ad, bc, bd), max (ac, ad, bc, bd)]
    return Interval(
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

Interval operator/ (Interval const& lhs, Interval const& rhs){
    // this may be improved if multiple intervals are used
    if(in(0, lhs) || in(0, rhs)){
        return Interval::world();
    }
    else{
        // [a,b] / [c,d] = [min (a/c, a/d, b/c, b/d), max (a/c, a/d, b/c, b/d)]
        double a = static_cast<double>(lhs.lower());
        double b = static_cast<double>(lhs.upper());
        double c = static_cast<double>(rhs.lower());
        double d = static_cast<double>(rhs.upper());
        
        return Interval(
            (detail::min)(
                (a / c),
                (a / d),
                (b / c),
                (b / d)
            ),
            (detail::max)(
                (a / c),
                (a / d),
                (b / c),
                (b / d)
            )
        );
    }
}

//// set operations ////
bool in(Interval::Integer x, Interval const& i){
    return x >= i.lower() && x <= i.upper();
}

// A ∩ B = [max(a,c),min(b,d)]
Interval operator& (Interval const& lhs, Interval const& rhs){
    return Interval(
        (std::max)(lhs.lower(), rhs.lower()),
        (std::min)(lhs.upper(), rhs.upper())
    );
}

// A ∩ (B ∪ [-∞,c]) = [a,min(b,d)]
Interval restrictLeft(Interval const& lhs, Interval const& rhs){
    return Interval(lhs.lower(), (std::min)(lhs.upper(), rhs.upper()));
}

// A ∩ (B ∪ [d,+∞]) = [max(a,c),b]
Interval restrictRight(Interval const& lhs, Interval const& rhs){
    return Interval((std::max)(lhs.lower(), rhs.lower()), lhs.upper());
}

std::ostream& operator<<(std::ostream& os, Interval const& i){
    if(i == Interval::world()){
        return os << "[-∞,+∞]";
    }
    else if(i.empty()){
        return os << "∅";
    }
    else{
        return os << '[' << i.lower() << ',' << i.upper() << ']';
    }
}
