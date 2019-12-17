#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

/*****************************************************************
 * Class Name      : Singleton
 *
 * Base Class      : None
 *
 * Purpose         : Inherit this class in order to turn any other class a singleton. (CRTP usage in other classes)
 *
 * Special Members :
 * (static member) p - the instance of the object
 * (static member) mutex -  this mutex to be used while instantiating p (thread safe)
 *
 * templates       : T - The class type to be turned to singleton
 *
 ****************************************************************/
template <class T>
class Singleton
{

public:
    static T& Instance() {
        if(p == 0) { // check first to avoid mutex lock when p is not 0
            _mutex.lock();
            if (p == 0)
                p = new T;
            _mutex.unlock();
        }
        return *p;
    }

    static void Destroy() {
        if(p != 0) delete p;
        p = 0;
    }

    static bool Exists() {
        return (p != 0);
    }

protected:
    static T *p;
    static std::mutex _mutex;
    Singleton() {}
    explicit Singleton(Singleton const &);
    Singleton& operator = (Singleton const &);
};

template <class T>
T* Singleton<T>::p = 0;
template <class T>
std::mutex Singleton<T>::_mutex;


#endif
