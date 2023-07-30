#include <kernel/arch/x86_64/idt.hpp>

#include <kernel/arch/x86_64/cpu.hpp>
#include <kernel/arch/x86_64/gdt.hpp>
#include <kernel/utility/log.hpp>
#include <kernel/utility/panic.hpp>
#include <ncxx/utility/to-underlying-type.hpp>

extern "C" void* NOS_interruptTable[];
extern "C" void NOS_interruptHandler(NOS::X86_64::CPU::Registers* registers);

namespace NOS::X86_64 {

namespace {

IDT* idt{nullptr};

void logRegisters(const X86_64::CPU::Registers& registers)
{
    Log::error("idt: registers");
    Log::ScopeIndent scopeIndent;

    // clang-format off
    Log::error("r8=0x{:X} r9=0x{:X} r10=0x{:X} r11=0x{:X} r12=0x{:X} r13=0x{:X} r14=0x{:X} r15=0x{:X}",
               registers.r8, registers.r9, registers.r10, registers.r11, registers.r12, registers.r13, registers.r14, registers.r15);
    Log::error("rax=0x{:X} rbx=0x{:X} rcx=0x{:X} rdx=0x{:X} rsi=0x{:X} rdi=0x{:X} rbp=0x{:X}",
               registers.rax, registers.rbx, registers.rcx, registers.rdx, registers.rsi, registers.rdi, registers.rbp);
    Log::error("interrupt=0x{:X} error code=0x{:X}",
               registers.interrupt, registers.errorCode);
    // clang-format on
}

} // namespace

void IDT::Entry::set(void* isr_, uint8_t typeAttr_, uint8_t ist_)
{
    u64_t isr = reinterpret_cast<u64_t>(isr_);
    offset1 = static_cast<u16_t>(isr);
    selector = GDT::Selector::Code;
    ist = ist_;
    typeAttr = typeAttr_;
    offset2 = static_cast<u16_t>(isr >> 16);
    offset3 = static_cast<u32_t>(isr >> 32);
    zero = 0;
}

IDT::IDT()
{
    NOS_ASSERT(idt == nullptr);

    idt = this;
}

void IDT::load()
{
    Log::info("idt: loading");

    for (size_t i = 0; i < _entries.size(); ++i)
    {
        _entries[i].set(NOS_interruptTable[i]);
    }

    Register r = makeRegister();

    asm volatile("cli");
    asm volatile("lidt %0" ::"memory"(r));
    asm volatile("sti");
}
void IDT::dispatch(const CPU::Registers& registers)
{
    Log::info("idt: dispatch");

    const size_t exceptionCount = toUnderlyingType(Exception::Count);

    if (registers.interrupt < exceptionCount)
    {
        handleException(registers);
    }
    else if (registers.interrupt >= exceptionCount && registers.interrupt < _entries.size())
    {
        Log::info("idt: Unhandled interrupt {}", registers.interrupt);
        logRegisters(registers);
    }
    else
    {
        Log::error("idt: Unknown interrupt {} on CPU {}", ExceptionString[registers.interrupt], 0);
        logRegisters(registers);
        panic();
    }
}

IDT::Register IDT::makeRegister() const
{
    return {.limit = static_cast<u16_t>(sizeof(Entry) * _entries.size() - 1),
            .base = reinterpret_cast<uintptr_t>(_entries.data())};
}

void IDT::handleException(const CPU::Registers& registers)
{
    Log::error("idt: Exception {} on CPU {}", ExceptionString[registers.interrupt], 0);
    logRegisters(registers);
    panic();
}

} // namespace NOS::X86_64

extern "C" void NOS_interruptHandler(NOS::X86_64::CPU::Registers* registers)
{
    NOS::X86_64::idt->dispatch(*registers);
}
