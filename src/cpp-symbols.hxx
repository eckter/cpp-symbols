#include "cpp-symbols.hh"

#include <tuple>
#include <vector>

namespace cpp_symbols
{
    namespace _helpers
    {
        template<typename Signature>
        struct helper;

        template<typename R, typename ... Args>
        struct helper<R(Args...)>
        {
            static
            std::function<R(Args...)>
            get_function(std::string name, std::string filename)
            {
                std::string prototype = get_prototype<Args...>(name, false);
                if (prototype == "")
                    return nullptr;
                return load_symbol<R(*)(Args...)>(prototype, filename);
            }

            static
            std::function<R(void*, Args...)>
            get_method(std::string name,
                std::string filename,
                bool is_const)
            {
                std::string prototype = get_prototype<Args...>(name, is_const);
                if (prototype == "")
                    return nullptr;
                return load_symbol<R(*)(void*, Args...)>(prototype, filename);
            }
        };

        template <typename T>
        std::string get_type()
        {
            int status;
            char* demangled
                = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
            if (!demangled)
                return "";
            std::string type = demangled;
            free(demangled);
            if (std::is_const<T>())
                type += " const";
            if (std::is_reference<T>())
            {
                type +=
                    std::is_const<typename std::remove_reference<T>::type>()
                    ? " const&"
                    : "&";
            }
            return type;
        }

        template <bool first>
        std::string concatenate_args()
        {
            return "";
        }

        template <bool first, typename Arg, typename ... Args>
        std::string concatenate_args()
        {
            std::string res = first ? "" : ", ";
            res += get_type<Arg>() + concatenate_args<false, Args...>();
            return res;
        }

        template <typename ... Args>
        std::string get_parameter_list()
        {
            return concatenate_args<true, Args...>();
        }

        inline
        std::tuple<char*, std::size_t> read_symbols(void* handle)
        {
            struct link_map* map;
            dlinfo(handle, RTLD_DI_LINKMAP, &map);

            ElfW(Dyn)* dyn = (ElfW(Dyn)*)(map->l_ld);
            char* strtab = 0;
            ElfW(Word) string_size = 0;
            while (dyn->d_tag != DT_NULL)
            {
                if (dyn->d_tag == DT_STRSZ)
                    string_size = dyn->d_un.d_val;
                else if (dyn->d_tag == DT_STRTAB)
                    strtab = (char*)dyn->d_un.d_ptr;
                dyn++;
            }
            return std::make_tuple(strtab, string_size);
        }

        inline
        std::vector<std::string> split_array(char* array, std::size_t size)
        {
            std::vector<std::string> res;
            std::string current;
            for (unsigned int i = 0; i < size; i++)
            {
                if (array[i])
                    current += array[i];
                else
                {
                    res.push_back(current);
                    current = "";
                }
            }
            return res;
        }

        inline
        std::string find_matching_symbol(
            const std::vector<std::string>& symbols,
            const std::string& wanted_prototype)
        {
            for (auto s : symbols)
            {
                int status = -1;
                if (s.size() > 0 && s[0] == '_')
                {
                    char* demangled
                        = abi::__cxa_demangle(s.c_str(), NULL, 0, &status);
                    if (demangled)
                    {
                        std::string prototype(demangled);
                        free(demangled);
                        if (prototype == wanted_prototype)
                            return s;
                    }
                }
            }
            return "";
        }
    }

    template<typename T>
    T load_symbol(std::string prototype, std::string filename)
    {
        void* handle = load_handle(filename.c_str());
        if (!handle)
        {
            std::cerr << filename << ": file not found\n";
            return nullptr;
        }
        std::string symbol = find_symbol(handle, prototype);
        if (symbol == "")
        {
            std::cerr << prototype << ": not found\n";
            return nullptr;
        }
        void* function_ptr = dlsym(handle, symbol.c_str());
        return reinterpret_cast<T>(function_ptr);
    }

    template <typename ... Args>
    std::string get_prototype(std::string name, bool is_const)
    {
        std::string prototype = name + "(";
        prototype += _helpers::get_parameter_list<Args...>();
        prototype += ")";
        if (is_const)
            prototype += " const";
        return prototype;
    }

    template <typename T>
    std::function<T>
    get_function(std::string name, std::string filename)
    {
        return _helpers::helper<T>::get_function(name, filename);
    }

    template <typename T>
    auto
    get_method_constness(std::string name,
        std::string filename,
        bool is_const)
    {
        return _helpers::helper<T>::get_method(name, filename, is_const);
    }

    template <typename T>
    auto
    get_method(std::string name,
        std::string filename)
    {
        auto non_const = get_method_constness<T>(name, filename, false);
        if (non_const)
            return non_const;
        return get_method_constness<T>(name, filename, true);
    }

    inline
    std::string find_symbol(void* handle, std::string wanted_prototype)
    {
        auto raw_symbols = _helpers::read_symbols(handle);
        std::vector<std::string> symbols = _helpers::split_array(
            std::get<0>(raw_symbols),
            std::get<1>(raw_symbols));
        return _helpers::find_matching_symbol(symbols, wanted_prototype);
    }

    inline
    void* load_handle(const char* filename)
    {
        void *handle = dlopen(filename, RTLD_LAZY);
        if (!handle)
            abort();
        return handle;
    }
}
