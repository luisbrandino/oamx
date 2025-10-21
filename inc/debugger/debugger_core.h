#ifndef DEBUGGER_CORE_H
#define DEBUGGER_CORE_H

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <thread>
#include <atomic>
#include "imgui.h"
#include "core_api.h"

typedef struct {
    GB_cpu_snapshot_t cpu_snapshot;
    GB_memory_snapshot_t memory_snapshot;
} GB_state_t;

typedef struct {
    GLFWwindow* window;
    ImGuiContext* imgui_context;
    std::thread debugger_thread;
    std::atomic<bool> running;
    GB_state_t* gb_state;
} Debugger;

void debugger_core_init(Debugger* debugger);
void debugger_core_loop(Debugger* debugger);
void debugger_core_cleanup(Debugger* debugger);

#endif