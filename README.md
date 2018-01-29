This is a header-only library that can be used to load C++ functions and
methods in a shared library.

It is the result of failed attempts at loading any C++ code at runtime.
We had to find another approach because it didn't answer all of our needs,
because of templates and other problematic cases. It can still be useful,
as it can be used to load any function regardless of name mangling, and with
a nice interface.

Public functions are documented in `src/cpp-symbols.hh`. The main ones are
`std::function<P> get_function<P>(std::string func_name, std::string filename)`
and its variations, with `P` being a function prototype type like `void(int)`.

To run the tests, run `tests/run_tests.sh`.

Possible improvements:

* Improving error managements through exceptions
* Finding a better way to handle classes, for example by using a class

Authors:

* Eloi Charpentier, eloi.charpentier@epita.fr
* Yannick Utard, yannick.utard@epita.fr
