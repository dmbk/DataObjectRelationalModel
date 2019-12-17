#ifndef UTIL_H
#define UTIL_H

template <typename T>
std::enable_if_t<std::is_pointer<T>::value, std::remove_pointer_t<T>&> dereference(T& t) {
    return *t;
}

template <typename T>
std::enable_if_t<!std::is_pointer<T>::value, T&> dereference(T& t) {
    return t;
}


template <class T>
struct PlainType {
    typedef T type;
};

template <class T>
struct PlainType<T*> {
    typedef T type;
};


#endif
