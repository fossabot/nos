#include <kernel/arch/x86_64/programmable-interrupt-controller.hpp>

#include <kernel/arch/x86_64/io.hpp>
#include <kernel/utility/log.hpp>
#include <ncxx/utility/scope-exit.hpp>

namespace NOS::X86_64 {

void ProgrammableInterruptController::initialize()
{
    Log::info("pic: Initialization");

    // Cache mask
    const u8_t cached1Data = IO::in<u8_t>(IO::Port::PIC1Data);
    const u8_t cached2Data = IO::in<u8_t>(IO::Port::PIC2Data);

    // Restore mask on exit
    auto restoreCachedDataOnScopeExit = makeScopeExit([&cached1Data, &cached2Data] {
        IO::out<u8_t>(IO::Port::PIC1Data, cached1Data);
        IO::out<u8_t>(IO::Port::PIC2Data, cached2Data);
    });

    IO::out<u8_t>(IO::Port::PIC1, ICW1::Init | ICW1::ICW4); // starts the initialization sequence (in cascade mode)
    IO::out<u8_t>(IO::Port::PIC2, ICW1::Init | ICW1::ICW4); // starts the initialization sequence (in cascade mode)
    IO::out<u8_t>(IO::Port::PIC1Data, 0x20);                // ICW2: Master PIC vector offset
    IO::out<u8_t>(IO::Port::PIC2Data, 0x28);                // ICW2: Slave PIC vector offset
    IO::out<u8_t>(IO::Port::PIC1Data, 0x04);                // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    IO::out<u8_t>(IO::Port::PIC2Data, 0x02);                // ICW3: tell Slave PIC its cascade identity (0000 0010)
    IO::out<u8_t>(IO::Port::PIC1Data, ICW4::Mode8086);      // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    IO::out<u8_t>(IO::Port::PIC2Data, ICW4::Mode8086);      // ICW4: have the PICs use 8086 mode (and not 8080 mode)
}

void ProgrammableInterruptController::disable()
{
    IO::out<u8_t>(IO::Port::PIC2Data, 0xFF);
    IO::out<u8_t>(IO::Port::PIC1Data, 0xFF);
}

void ProgrammableInterruptController::mask(u8_t irq)
{
    const u16_t port = irq < 8 ? IO::Port::PIC1Data : IO::Port::PIC2Data;

    if (irq >= 8)
    {
        irq -= 8;
    }

    const u8_t value = IO::in<u8_t>(port) | static_cast<u8_t>(1 << irq);
    IO::out(port, value);
}

void ProgrammableInterruptController::unmask(u8_t irq)
{
    const u16_t port = irq < 8 ? IO::Port::PIC1Data : IO::Port::PIC2Data;

    if (irq >= 8)
    {
        irq -= 8;
    }

    const u8_t value = IO::in<u8_t>(port) & ~static_cast<u8_t>(1 << irq);
    IO::out<u8_t>(port, value);
}

void ProgrammableInterruptController::eoi(u64_t interrupt)
{
    if (interrupt >= 8)
    {
        IO::out<u8_t>(IO::Port::PIC2Data, EOI);
    }

    IO::out<u8_t>(IO::Port::PIC1Data, EOI);
}

} // namespace NOS::X86_64