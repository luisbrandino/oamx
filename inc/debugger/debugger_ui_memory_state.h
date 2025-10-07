#ifndef DEBUGGER_UI_MEMORY_STATE_H
#define DEBUGGER_UI_MEMORY_STATE_H

#include "debugger_core.h"

void debugger_ui_render_memory_state(Debugger* debugger);

void debugger_ui_render_vram_state(Debugger* debugger);
void debugger_ui_render_wram_state(Debugger* debugger);
void debugger_ui_render_oam_state(Debugger* debugger);
void debugger_ui_render_io_state(Debugger* debugger);
void debugger_ui_render_hram_state(Debugger* debugger);
void debugger_ui_render_io_registers_state(Debugger* debugger);

#endif