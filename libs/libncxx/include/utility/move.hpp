#pragma once

#include <type-traits/remove-reference.hpp>

namespace NOS {

template<class T>
constexpr RemoveReferenceT<T>&& move(T&& v)
{
    return static_cast<RemoveReferenceT<T>&&>(v);
}

} // namespace N
