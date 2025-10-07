#ifndef DEBUGGER_UI_H
#define DEBUGGER_UI_H

#include "debugger_core.h"
#include "imgui.h"

void debugger_ui_init(Debugger* debugger);
void debugger_ui_render(Debugger* debugger);
void debugger_ui_cleanup(Debugger* debugger);

#endif