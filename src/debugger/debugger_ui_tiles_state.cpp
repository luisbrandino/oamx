#include "debugger/debugger_ui_tiles_state.h"

#include <vector>

inline uint16_t debugger_ui_tiles_read_unsigned_tiles(GB_memory_snapshot_t* memory_snapshot, uint8_t tile_index)
{
    return 0x8000 + (tile_index * 16);
}

void debugger_ui_tiles_read_tile(GB_memory_snapshot_t* memory_snapshot, uint8_t tile_index, uint32_t* pixels)
{
    uint16_t tile_base_address = debugger_ui_tiles_read_unsigned_tiles(memory_snapshot, tile_index);

    for (uint8_t y = 0; y < 8; y++)
    {
        uint8_t byte1 = memory_snapshot->vram[tile_base_address + (y * 2)];
        uint8_t byte2 = memory_snapshot->vram[tile_base_address + (y * 2) + 1];

        for (uint8_t x = 0; x < 8; x++)
        {
            uint8_t mask = (1 << (7 - x));
            uint8_t color = (((byte2 & mask) >> (7 - x)) << 1) | ((byte1 & mask) >> (7 - x));

            uint8_t index = (tile_index * 64) + (y * 8) + x;
            uint8_t shade = 255 - color * 85;
            uint32_t rgba = (0xFF << 24) | (shade << 16) | (shade << 8) | shade;

            pixels[index] = color;
        }
    }
}

void debugger_ui_tiles_draw_tiles_grid(ImVec2 topLeft, int cols, int rows, float cellSize, const ImU32* pixels)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            ImVec2 p0 = ImVec2(topLeft.x + x * cellSize, topLeft.y + y * cellSize);
            ImVec2 p1 = ImVec2(p0.x + cellSize, p0.y + cellSize);

            ImU32 color = pixels[y * cols + x];
            draw_list->AddRectFilled(p0, p1, color);
        }
    }
}

static GLuint tiles_texture = 0;

void debugger_ui_render_tiles_state(Debugger* debugger)
{
    GB_memory_snapshot_t* memory_snapshot = &debugger->gb_state->memory_snapshot;

    const int TILE_SIZE = 8;
    const int TILE_COUNT = 256;
    const int TILE_COLS = 16;
    const int TILE_ROWS = TILE_COUNT / TILE_COLS;
    const int ATLAS_WIDTH = TILE_COLS * TILE_SIZE;
    const int ATLAS_HEIGHT = TILE_ROWS * TILE_SIZE;
    std::vector<uint32_t> atlas_pixels(ATLAS_WIDTH * ATLAS_HEIGHT);

    for (uint8_t i = 0; i < 255; i++)
    {
        int tile_x = (i % TILE_COLS) * TILE_SIZE;
        int tile_y = (i / TILE_COLS) * TILE_SIZE;

        uint16_t tile_base_address = debugger_ui_tiles_read_unsigned_tiles(memory_snapshot, i);
        
        for (uint8_t y = 0; y < TILE_SIZE; y++)
        {
            uint8_t byte1 = memory_snapshot->vram[tile_base_address + (y * 2)];
            uint8_t byte2 = memory_snapshot->vram[tile_base_address + (y * 2) + 1];

            for (uint8_t x = 0; x < TILE_SIZE; x++)
            {
                uint8_t mask = (1 << (7 - x));
                uint8_t color = (((byte2 & mask) >> (7 - x)) << 1) | ((byte1 & mask) >> (7 - x));

                uint8_t index = (i * 64) + (y * 8) + x;
                uint8_t shade = 255 - color * 85;
                uint32_t rgba = (0xFF << 24) | (shade << 16) | (shade << 8) | shade;

                int dst_x = tile_x + x;
                int dst_y = tile_y + y;

                atlas_pixels[dst_y * ATLAS_WIDTH + dst_x] = color;
            }
        }
    }

    if (tiles_texture == 0)
    {
        glGenTextures(1, &tiles_texture);
        glBindTexture(GL_TEXTURE_2D, tiles_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    
    glBindTexture(GL_TEXTURE_2D, tiles_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ATLAS_WIDTH, ATLAS_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlas_pixels.data());

    ImGui::Begin("Tiles");

    float cell_size = 15.0f;
    ImVec2 display_size(ATLAS_WIDTH * (cell_size / TILE_SIZE), ATLAS_HEIGHT * (cell_size / TILE_SIZE));

    ImGui::Image((ImTextureID)(intptr_t)tiles_texture, display_size);

    ImGui::End();
}