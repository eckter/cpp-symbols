#include <stdio.h>
#include <string>
#include <iostream>

class Class
{
public:
    std::string foo(const std::string& test);
    int foo() const;
    int foo();
};

std::string Class::foo(const std::string& test)
{
    return test;
}

int Class::foo()
{
    return 1;
}

int Class::foo() const
{
    return 0;
}

void* constructor_wrapper()
{
    return reinterpret_cast<void*>(new Class());
}

void destructor_wrapper(void* ptr)
{
    auto tmp = reinterpret_cast<Class*>(ptr);
    delete tmp;
}

namespace toto
{
    int foo()
    {
        return 42;
    }
}

int foo()
{
    return 42;
}

int foo(int)
{
    return 3;
}

std::string bar(int i, const std::string& s, double d)
{
    (void) i;
    (void) d;
    return s;
}
