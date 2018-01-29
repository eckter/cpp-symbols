#pragma once

#include <cxxabi.h>
#include <functional>
#include <iostream>
#include <link.h>
#include <string>
#include <typeinfo>

namespace cpp_symbols
{
    /**
     * @brief Get a function in a .so
     *
     * @param name The name of the function, with namespace:: if relevant
     * @param filename The path to the .so
     *
     * @return An std::function, empty if error
     */
    template <typename Signature>
    std::function<Signature>
    get_function(std::string name, std::string filename);

    /**
     * @brief Get a method in a .so
     *
     * @param name The name of the method, with namespace::class::name
     * @param filename The path to the .so
     * @param is_const Constness of the method
     *
     * @return An std::function, emtpy if error
     *
     * The returned function has the implicit object parameter as void* as
     * first parameter
     */
    template <typename Signature>
    auto
    get_method_constness(std::string name,
                         std::string filename,
                         bool is_const = false);

    /**
     * @brief Same as get_method_constness, but ignores constness
     *
     * @param name The name of the method, with namespace::class::name
     * @param filename The path to the .so
     * @param is_const Constness of the method
     *
     * @return An std::function, emtpy if error
     *
     * The returned function has the implicit object parameter as void* as
     * first parameter
     *
     * If both const and non const symbols are present, the non const one will
     * be returned
     */
    template <typename Signature>
    auto
    get_method(std::string name,
               std::string filename);

    /**
     * @brief Find a mangled function symbol in a .so
     *
     * @param handle The .so mapped in memory
     * @param wanted_prototype The prototype of the function
     *
     * @return The mangled symbol of function or empty string if not found
     */
    std::string find_symbol(void* handle, std::string wanted_prototype);

    /**
     * @brief Load a .so in memory
     *
     * @param filename The path to the .so
     *
     * @return The pointer to the .so in memory
     */
    void* load_handle(const char* filename);

    /**
     * @brief Get the function of type T
     *        (T is either function_ptr or method_ptr)
     *
     * @param prototype The prototype of the function
     * @param filename The path to the .so
     *
     * @return The function as a pointer to function, nullptr if empty
     */
    template<typename T>
    T load_symbol(std::string prototype, std::string filename);

    /**
     * @brief Get the prototype of a function
     *
     * @param name The name of the function, with namespace:: or class::
     * @param is_const Constness of the function
     *
     * @return The prototype of the function
     */
    template <typename ... Args>
    std::string get_prototype(std::string name, bool is_const = false);
}
#include "cpp-symbols.hxx"
