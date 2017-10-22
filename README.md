# result

Simple header-only variant-driven C++17 `Result<T,E>` implementation.

## Requirements

* c++17 or c++1z with support of variant

## Features

* Small, header-only.
* Pure C++.
* Human and machine readable (with custom return errors support) error
  representation.
* Compile-time check of dereference.

## Usage example

```cpp
#include <cassert>
#include <iostream>

#include "result.hpp"

using namespace std::string_literals;

enum class ErrorCode { Error };
using IError = Result::Error<ErrorCode>;
template <class T> using IResult = Result::Result<T, ErrorCode>;


class Test
{
public:
    Test() = default;
    ~Test() = default;
    friend std::ostream &operator<<(std::ostream &os, const Test &)
    {
        os << "I'm test class";
        return os;
    };
};


template <class T> Result::Content print_example(IResult<T> r)
{
    switch (r.type()) {
    case Result::Content::Value:
        std::cout << "Result has value " << r.get() << std::endl;
        return Result::Content::Value;
    case Result::Content::Error:
        std::cout << "Result has error " << r.error().message() << std::endl;
        return Result::Content::Error;
    case Result::Content::Empty:
        std::cout << "Result does not contain anything" << std::endl;
        return Result::Content::Empty;
    }
}


int main()
{
    assert(print_example<int>(42) == Result::Content::Value);
    assert(print_example<int>(IError("int error"s)) == Result::Content::Error);

    assert(print_example<std::string>("a string"s) == Result::Content::Value);
    assert(print_example<std::string>(IError("std::string error"s))
           == Result::Content::Error);

    assert(print_example<Test>(Test()) == Result::Content::Value);
    assert(print_example<Test>(IError("Test error"s))
           == Result::Content::Error);

    return 0;
}
```
