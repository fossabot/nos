#pragma once

#include <ncxx/basic-types.hpp>

void* operator new(NOS::size_t size);
void* operator new[](NOS::size_t size);
void operator delete(void* p);
void operator delete[](void* p);

inline void* operator new(NOS::size_t, void* p)
{
    return p;
}

inline void* operator new[](NOS::size_t, void* p)
{
    return p;
}

inline void operator delete(void*, void*)
{}
inline void operator delete[](void*, void*)
{}
