
#ifndef NAME_OF_H
#define	NAME_OF_H


#include <cxxabi.h>
#include <cstdlib>
#include <string>
#include <typeinfo>
#include <memory>

template <class T> 
struct name_of
{
    static std::string value()
    {
        int status;
        std::unique_ptr<char, void (*)(void*)>  real_name { abi::__cxa_demangle( typeid(T).name(), 0, 0, &status ), &std::free };
        return real_name.get();
    }
};

template <class T>
struct static_name_of;

#endif

