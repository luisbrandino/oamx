#ifndef DEBUGGER_WINDOW_H
#define DEBUGGER_WINDOW_H

#include "debugger_core.h"

#define DEBUGGER_WINDOW_WIDTH 800
#define DEBUGGER_WINDOW_HEIGHT 600

void debugger_window_init(Debugger* debugger);
void debugger_window_loop(Debugger* debugger);
void debugger_window_cleanup(Debugger* debugger);

#endif