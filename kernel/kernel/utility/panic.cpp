#include <kernel/utility/panic.hpp>

#include <kernel/arch/interrupt.hpp>
#include <kernel/utility/log.hpp>
#include <kernel/arch/x86_64/cpu.hpp>

namespace NOS {

void panic(StringView message, SourceLocation location)
{
    Log::newLine();

    if (!message.isEmpty())
    {
        Log::error(message);
    }

    Log::error("File: {}", location.fileName());
    Log::error("Line: {}", location.line());
    Log::error("Column: {}", location.column());
    Log::error("Function: {}", location.functionName());
    Log::error("System halted");

    Interrupt::hcf();
}

} // namespace NOS
