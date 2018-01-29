#include "../src/cpp-symbols.hh"
#include <string>
#include <cassert>

using namespace cpp_symbols;

char* lib;

template <typename T>
std::function<T> check(const std::string& name, bool exist = true)
{
    auto func = get_function<T>(name, lib);
    assert(exist == (func != nullptr));
    return func;
}

template <typename T>
auto check_method(const std::string& name, bool constness = false, bool exist = true)
{
    auto method = get_method_constness<T>(name, lib, constness);
    assert(exist == (method != nullptr));
    return method;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;

    lib = argv[1];

    check<int(int, int)>("foo", false);

    auto foo_int = check<int()>("foo");
    assert(foo_int() == 42);

    auto foo_arg = check<int(int)>("foo");
    assert(foo_arg(0) == 3);

    auto multiple_args = check<std::string(int, const std::string&, double)>("bar");
    assert(multiple_args(0, "mult", 1.5) == "mult");

    auto foo_namespace = check<int()>("toto::foo");
    assert(foo_namespace() == 42);

    auto constr = check<void*()>("constructor_wrapper");
    void* inst = constr();

    auto foo_basic = check_method<int()>("Class::foo");
    assert(foo_basic(inst) == 1);

    auto foo_const = check_method<int()>("Class::foo", true);
    assert(foo_const(inst) == 0);

    auto foo_string = check_method<std::string(const std::string&)>("Class::foo");
    assert(foo_string(inst, "regal") == "regal");

    auto destr = check<void(void*)>("destructor_wrapper");
    destr(inst);

    return 0;
}
