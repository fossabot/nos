#pragma once

#include <ncxx/basic-types.hpp>
#include <ncxx/string/string-view.hpp>

// must be in std
namespace std {

// infos taken from LLVM
struct source_location
{
    // The names source_location::__impl, _M_file_name, _M_function_name, _M_line, and _M_column
    // are hard-coded in the compiler and must not be changed here.
    struct __impl
    {
        const char* _M_file_name;
        const char* _M_function_name;
        unsigned _M_line;
        unsigned _M_column;
    };

    // GCC returns the type 'const void*' from the builtin, while clang returns
    // `const __impl*`. Per C++ [expr.const], casts from void* are not permitted
    // in constant evaluation, so we don't want to use `void*` as the argument
    // type unless the builtin returned that, anyhow, and the invalid cast is
    // unavoidable.
    using __bsl_ty = decltype(__builtin_source_location());

    constexpr source_location() = default;

    const __impl* __ptr_ = nullptr;
};

} // namespace std

namespace NOS {

class SourceLocation : private std::source_location
{
    using Base = std::source_location;

public:
    // The defaulted __ptr argument is necessary so that the builtin is evaluated
    // in the context of the caller. An explicit value should never be provided.
    static consteval SourceLocation current(__bsl_ty __ptr = __builtin_source_location());

    constexpr size_t line() const;
    constexpr size_t column() const;
    constexpr StringView fileName() const;
    constexpr StringView functionName() const;
};

consteval SourceLocation SourceLocation::current(__bsl_ty __ptr)
{
    SourceLocation sl;
    sl.__ptr_ = static_cast<const __impl*>(__ptr);
    return sl;
}

constexpr size_t SourceLocation::line() const
{
    return __ptr_ != nullptr ? static_cast<size_t>(__ptr_->_M_line) : 0;
}

constexpr size_t SourceLocation::column() const
{
    return __ptr_ != nullptr ? static_cast<size_t>(__ptr_->_M_column) : 0;
}

constexpr StringView SourceLocation::fileName() const
{
    return __ptr_ != nullptr ? StringView{__ptr_->_M_file_name} : StringView{};
}

constexpr StringView SourceLocation::functionName() const
{
    return __ptr_ != nullptr ? StringView{__ptr_->_M_function_name} : StringView{};
}

} // namespace NOS
