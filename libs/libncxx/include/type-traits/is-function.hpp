#pragma once

#include <type-traits/integral-constant.hpp>

namespace NOS {

template<class T>
struct IsFunction : IntegralConstant<bool, __is_function(T)>
{};

template<class T>
inline constexpr bool IsFunctionV = IsFunction<T>::Value;

} // namespace NOS
