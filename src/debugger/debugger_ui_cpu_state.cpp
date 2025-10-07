#include "debugger/debugger_ui_cpu_state.h"
#include "core_api.h"

void debugger_ui_render_cpu_state(Debugger* debugger)
{
    ImGui::Begin("CPU State");

    ImGui::Text("Registers:");
    ImGui::Separator();

    GB_cpu_snapshot_t* cpu = &debugger->gb_state->cpu_snapshot;

    ImGui::Text("A:  0x%02X", cpu->a);
    ImGui::SameLine(120);
    ImGui::Text("F:  0x%02X", cpu->f);

    ImGui::Text("B:  0x%02X", cpu->b);
    ImGui::SameLine(120);
    ImGui::Text("C:  0x%02X", cpu->c);

    ImGui::Text("D:  0x%02X", cpu->d);
    ImGui::SameLine(120);
    ImGui::Text("E:  0x%02X", cpu->e);

    ImGui::Text("H:  0x%02X", cpu->h);
    ImGui::SameLine(120);
    ImGui::Text("L:  0x%02X", cpu->l);

    ImGui::Separator();
    ImGui::Text("PC: 0x%04X", cpu->pc);
    ImGui::SameLine(160);
    ImGui::Text("SP: 0x%04X", cpu->sp);

    ImGui::Separator();
    ImGui::Text("IME: %s", cpu->ime ? "Enabled" : "Disabled");

    ImGui::Text("Flags:");
    bool z = cpu->f & 0x80;
    bool n = cpu->f & 0x40;
    bool h = cpu->f & 0x20;
    bool c = cpu->f & 0x10;

    ImGui::Checkbox("Z", &z);
    ImGui::SameLine();
    ImGui::Checkbox("N", &n);
    ImGui::SameLine();
    ImGui::Checkbox("H", &h);
    ImGui::SameLine();
    ImGui::Checkbox("C", &c);

    if (z || n || h || c)
    {
        ImGui::TextDisabled("(Flags are read-only)");
    }

    ImGui::End();
}