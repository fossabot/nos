#include <kernel/lang/new.hpp>

#include <kernel/utility/log.hpp>
#include <ncxx/memory/allocator/stack-allocator.hpp>

namespace NOS {

namespace {

using Allocator = Memory::StackAllocator<8196 * 1>;

static Allocator allocator;

}

} // namespace NOS

void* operator new(NOS::size_t size)
{
    NOS::Memory::Block block = NOS::allocator.allocate(size);
    NOS_ASSERT(block.pointer != nullptr);
    return block.pointer;
}

void* operator new[](NOS::size_t size)
{
    NOS::Memory::Block block = NOS::allocator.allocate(size);
    NOS_ASSERT(block.pointer != nullptr);
    return block.pointer;
}

void operator delete(void* p)
{
    // TODO Handle size
    NOS::Memory::Block block{
        .pointer = p,
        .size = 1};
    NOS::allocator.deallocate(block);
}

void operator delete[](void* p)
{
    // TODO Handle size
    NOS::Memory::Block block{
        .pointer = p,
        .size = 1};
    NOS::allocator.deallocate(block);
}
