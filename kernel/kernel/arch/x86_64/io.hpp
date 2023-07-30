#pragma once

#include <kernel/arch/x86_64/details/io-impl.hpp>
#include <ncxx/basic-types.hpp>
#include <ncxx/concept/same-as.hpp>

namespace NOS::X86_64::IO {

namespace Port {

static constexpr u16_t Debug = 0x00E9;

} // namespace Port

template<typename T>
concept SupportedType = SameAs<T, u8_t> || SameAs<T, u16_t> || SameAs<T, u32_t>;

template<typename T>
requires(SupportedType<T>)
static inline T in(u16_t port)
{
    return Details::inImpl<T>(port);
}

template<typename T>
requires(SupportedType<T>)
static inline void out(u16_t port, T value)
{
    Details::outImpl(port, value);
}

} // namespace NOS::X86_64::IO
