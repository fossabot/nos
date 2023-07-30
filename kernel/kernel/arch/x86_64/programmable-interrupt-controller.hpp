#pragma once

#include <ncxx/basic-types.hpp>

namespace NOS::X86_64 {

namespace IO::Port {

static constexpr u16_t PIC1 = 0x20;
static constexpr u16_t PIC1Data = PIC1 + 1;

static constexpr u16_t PIC2 = 0xA0;
static constexpr u16_t PIC2Data = PIC1 + 1;

} // namespace IO::Port

struct ICW1
{
    static constexpr u8_t ICW4 = 0x01;      /* Indicates that ICW4 will be present */
    static constexpr u8_t Single = 0x02;    /* Single (cascade) mode */
    static constexpr u8_t Interval4 = 0x04; /* Call address interval 4 (8) */
    static constexpr u8_t Level = 0x08;     /* Level triggered (edge) mode */
    static constexpr u8_t Init = 0x10;      /* Initialization  */
};

struct ICW4
{
    static constexpr u8_t Mode8086 = 0x01;       /* 8086/88 (MCS-80/85) mode */
    static constexpr u8_t Auto = 0x02;           /* Auto (normal) EOI */
    static constexpr u8_t BufferedSlave = 0x08;  /* Buffered mode/slave */
    static constexpr u8_t BufferedMaster = 0x0C; /* Buffered mode/master */
    static constexpr u8_t SFNM = 0x10;           /* Special fully nested (not) */
};

static constexpr u8_t EOI = 0x20;

class ProgrammableInterruptController
{
public:
    void initialize();
    void disable();

    void mask(u8_t irq);
    void unmask(u8_t irq);

    void eoi(u64_t interrupt);
};

} // namespace NOS::X86_64