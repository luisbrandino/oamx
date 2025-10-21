#include <stdio.h>
#include "debugger/debugger_window.h"
#include "debugger/debugger_ui.h"
#include "debugger/debugger_core.h"
#include "core_api.h"

void debugger_core_init(Debugger* debugger)
{
    GB_state_t* gb_state = (GB_state_t*)malloc(sizeof(GB_state_t));
    if (!gb_state)
    {
        fprintf(stderr, "Failed to init GB state\n");
        return;
    }

    debugger->gb_state = gb_state;

    debugger->debugger_thread = std::thread([debugger]() {
        debugger_window_init(debugger);
        debugger_ui_init(debugger);

        debugger->running = true;
        while (debugger->running)
            debugger_core_loop(debugger);
    });

    debugger->debugger_thread.detach();
}

/**
 * should sync the current gameboy state data before rendering
 */
void debugger_core_loop(Debugger* debugger)
{
    /**
     * gets the latest gameboy state snapshot from the core api. it's thread-safe 
     */
    if (debugger->gb_state)
    {
        GB_get_cpu_snapshot(&debugger->gb_state->cpu_snapshot);
        GB_get_memory_snapshot(&debugger->gb_state->memory_snapshot);
    }

    debugger_window_loop(debugger);
}

void debugger_core_cleanup(Debugger* debugger)
{
    debugger_window_cleanup(debugger);
    debugger_ui_cleanup(debugger);
    free(debugger->gb_state);

    debugger->running = false;
    if (debugger->debugger_thread.joinable())
        debugger->debugger_thread.join();
}