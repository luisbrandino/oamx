#include "debugger/debugger_window.h"
#include "debugger/debugger_ui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>

void debugger_window_close_callback(GLFWwindow* window)
{
    Debugger* debugger = (Debugger*)glfwGetWindowUserPointer(window);
    if (debugger)
    {
        debugger_core_cleanup(debugger);
    }
}

void debugger_window_init(Debugger* debugger)
{
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to init GLFW\n");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    debugger->window = glfwCreateWindow(DEBUGGER_WINDOW_WIDTH, DEBUGGER_WINDOW_HEIGHT, "the functional debugger", NULL, NULL);
    if (!debugger->window)
    {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return;
    }
    
    glfwSetWindowUserPointer(debugger->window, debugger);
    glfwSetWindowCloseCallback(debugger->window, debugger_window_close_callback);
    glfwMakeContextCurrent(debugger->window);
    glfwSwapInterval(1);
}

void debugger_window_loop(Debugger* debugger)
{
    if (glfwWindowShouldClose(debugger->window))
        return;

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    debugger_ui_render(debugger);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(debugger->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(debugger->window);
}

void debugger_window_cleanup(Debugger* debugger)
{
    if (debugger->window)
    {
        glfwDestroyWindow(debugger->window);
        debugger->window = NULL;
    }

    glfwTerminate();
}