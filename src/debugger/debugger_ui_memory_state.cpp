#include "debugger/debugger_ui_memory_state.h"

void debugger_ui_render_memory_state(Debugger* debugger)
{
    debugger_ui_render_oam_state(debugger);

    /* maybe raw memory view here? */
}

void debugger_ui_render_oam_state(Debugger* debugger)
{
    ImGui::Begin("OAM");

    ImGui::End();
}