#include <kernel/kernel.hpp>

#include <kernel/drivers/serial.hpp>
#include <kernel/lang/cxxabi.hpp>
#include <kernel/utility/log.hpp>

namespace NOS::Kernel {

void initialize()
{
    Serial::initializePorts({Serial::Ports[0]});

    Log::info("Kernel initialization");

    Log::info("\t - cxxabi");
    Lang::CxxAbi::init();
}

void run()
{
}

} // namespace NOS::Kernel
