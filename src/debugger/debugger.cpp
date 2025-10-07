#include "debugger/debugger_core.h"

static Debugger debugger;

extern "C" void debugger_main()
{
    debugger_core_init(&debugger);
}

extern "C" void debugger_stop()
{
    debugger_core_cleanup(&debugger);
}