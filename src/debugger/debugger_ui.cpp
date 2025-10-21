#include "debugger/debugger_ui.h"
#include "debugger/debugger_ui_cpu_state.h"
#include "debugger/debugger_ui_memory_state.h"
#include "debugger/debugger_ui_tiles_state.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

void debugger_ui_init(Debugger* debugger)
{
    debugger->imgui_context = ImGui::CreateContext();
    if (!debugger->imgui_context)
    {
        fprintf(stderr, "Failed to create ImGui context\n");
        return;
    }

    ImGui_ImplGlfw_InitForOpenGL(debugger->window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    ImGui::StyleColorsDark();
}

void debugger_ui_render(Debugger* debugger)
{
    debugger_ui_render_cpu_state(debugger);
    debugger_ui_render_memory_state(debugger);
    debugger_ui_render_tiles_state(debugger);
}

void debugger_ui_cleanup(Debugger* debugger)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    if (debugger->imgui_context)
    {
        ImGui::DestroyContext(debugger->imgui_context);
        debugger->imgui_context = NULL;
    }
}