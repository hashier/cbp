#ifndef MAYBE_H
#define MAYBE_H

// Not everything has a result ⇒ Maybe type as non-instrusive solution
#include <utility>
#include <cassert>

template<typename T>
class Maybe {
private:
    typedef std::pair<bool, T> MaybeContainer;  
    struct NothingTag {};
    
public:
    static NothingTag nothing;
    Maybe(NothingTag) : data(false, T()) {}    
    Maybe(T const& t) : data(true, t) {}
    
    bool isJust() const {
        return data.first;
    }
    
    T const& get() const {
        assert(isJust());
        return data.second;
    }
    
private:
    MaybeContainer data;
};

template<typename T>
typename Maybe<T>::NothingTag Maybe<T>::nothing;

template<typename T>
Maybe<T> just(T const& t){
    return Maybe<T>(t);
}

// generic return type
struct nothing {
    template<typename T>
    operator Maybe<T>(){
        return Maybe<T>(Maybe<T>::nothing);
    }
};

template<typename T>
bool isJust(Maybe<T> const& m){
    return m.isJust();
}

template<typename T>
bool isNothing(Maybe<T> const& m){
    return !m.isJust();
}

template<typename T>
T const& fromJust(Maybe<T> const& m){
    return m.get();
}

#endif // MAYBE_H
