#include <stdlib.h>
#include <string.h>
#include "../inc/interrupts.h"
#include "../inc/ppu.h"

static Sprite read_sprite(Memory* mem, uint8_t index)
{
    uint8_t sprite_y = mem->oam[index * 4];
    uint8_t sprite_x = mem->oam[index * 4 + 1];
    uint8_t tile = mem->oam[index * 4 + 2];
    uint8_t flags = mem->oam[index * 4 + 3];
    PriorityType priority = (flags & (1 << 7)) ? BEHIND_BG : OVERLAP_BG;

    return (Sprite) { sprite_y, sprite_x, tile, flags, priority };
}

static uint16_t get_tile_base_address(uint8_t lcdc_bit4, int16_t tile_index)
{
    if (lcdc_bit4)
        return 0x8000 + (uint16_t)tile_index * 16;
    
    return 0x9000 + tile_index * 16;
}

static uint8_t get_tile_pixel_from_vram(Memory* mem, uint16_t tile_base_address, uint8_t line, uint8_t x)
{
    // addr points to the first line of the tile, so to get the correct 
    // line, we should do addr + line * 2 (each line is 2 bytes)
    uint8_t b1 = memory_read(mem, tile_base_address + line * 2);
    uint8_t b2 = memory_read(mem, tile_base_address + line * 2 + 1);

    uint8_t mask = 1 << (7 - x);
    // let x be 2:
    // b1 = low
    // b2 = high
    // b1 = 0b00000000
    // b2 = 0b00100000
    // result for x = 2 should be:
    // pixel = 0b00000010
    uint8_t pixel = (((b2 & mask) >> (7 - x)) << 1) | ((b1 & mask) >> (7 - x));
    return pixel & 0x03;
}

static uint8_t get_sprite_pixel(Ppu* ppu, Memory* mem, uint16_t tile_index, uint8_t y, uint8_t x, uint8_t flags)
{
    y = (flags & (1 << 6)) ? (ppu->sprite_height - 1) - y : y;
    x = (flags & (1 << 5)) ? 7 - x : x;

    uint16_t tile_to_use = tile_index;
    if (ppu->sprite_height == 16 && y >= 8)
    {
        tile_to_use++;
        y -= 8;
    }

    uint16_t tile_base_address = get_tile_base_address(1, tile_to_use); // always unsigned
    return get_tile_pixel_from_vram(mem, tile_base_address, y, x);
}

static void ppu_draw_background(Ppu* ppu, Memory* mem)
{
    uint8_t scx = memory_read(mem, 0xFF43);
    uint8_t scy = memory_read(mem, 0xFF42);
    uint8_t wy = memory_read(mem, 0xFF4A);
    uint8_t wx = memory_read(mem, 0xFF4B);
    uint8_t lcdc = memory_read(mem, 0xFF40);
    uint8_t window_enabled = lcdc & (1 << 5);

    for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
    {
        uint16_t element_x;
        uint16_t element_y;
        uint16_t tilemap_base;
        uint8_t tile_col;
        uint8_t tile_row;
        uint8_t line_x;
        uint8_t line_y;

        if (window_enabled && ppu->ly >= wy && x >= (wx - 7))
        {
            // window
            tilemap_base = (lcdc & (1 << 6)) ? 0x9C00 : 0x9800;
            element_x = (uint8_t)(x - (wx - 7));
            element_y = (uint8_t)(ppu->ly - wy);
        }
        else
        {
            // background
            tilemap_base = (lcdc & (1 << 3)) ? 0x9C00 : 0x9800;
            element_x = (uint8_t)(x + scx);
            element_y = (uint8_t)(ppu->ly + scy);
        }

        tile_col = element_x / 8;
        tile_row = element_y / 8;
        line_x = element_x % 8;
        line_y = element_y % 8;

        uint16_t tilemap_index = tile_row * 32 + tile_col;
        uint8_t tile_index = memory_read(mem, tilemap_base + tilemap_index);

        uint8_t lcdc_bit4 = lcdc & (1 << 4);
        uint16_t tile_base_address = get_tile_base_address(lcdc_bit4, lcdc_bit4 ? tile_index : (int8_t)tile_index);
        uint8_t color = get_tile_pixel_from_vram(mem, tile_base_address, line_y, line_x);

        uint8_t bgp = memory_read(mem, 0xFF47);
        uint8_t shade = (bgp >> (color * 2)) & 0x03;

        ppu->framebuffer[ppu->ly][x] = shade;
    }
}

static void ppu_draw_scanline(Ppu* ppu, Memory* mem)
{   
    ppu_draw_background(ppu, mem);

    for (uint8_t i = 0; i < ppu->visible_sprite_count; i++)
    {
        Sprite sprite = ppu->visible_sprites[i];

        for (int px = 0; px < 8; px++)
        {
            uint8_t screen_x = sprite.x - 8 + px;
            if (screen_x >= SCREEN_WIDTH)
                continue;

            uint8_t line_in_sprite = ppu->ly - (sprite.y - 16);
            uint8_t color = get_sprite_pixel(ppu, mem, sprite.tile, line_in_sprite, px, sprite.flags);

            if (color == 0)
                continue;

            uint8_t palette = (sprite.flags & (1 << 4)) ? memory_read(mem, 0xFF49) : memory_read(mem, 0xFF48);
            uint8_t shade = (palette >> (color * 2)) & 0x03;

            uint8_t bg_pixel = ppu->framebuffer[ppu->ly][screen_x];

            // sprite's pixel should not overlap background
            if (sprite.priority == BEHIND_BG && bg_pixel != 0)
                continue;

            ppu->framebuffer[ppu->ly][screen_x] = shade;
        }
    }
}

static void ppu_oam_search(Ppu* ppu, Memory* mem)
{
    uint8_t lcdc = memory_read(mem, 0xFF40);
    ppu->sprite_height = (lcdc & (1 << 2)) ? 16 : 8;
    ppu->visible_sprite_count = 0;

    for (uint8_t i = 0; i < 40; i++)
    {
        Sprite sprite = read_sprite(mem, i);
        if ((ppu->ly + 16) >= sprite.y && (ppu->ly + 16) < (sprite.y + ppu->sprite_height))
        {
            ppu->visible_sprites[ppu->visible_sprite_count++] = sprite;
            if (ppu->visible_sprite_count >= 10)
                break;
        }
    }
}

void ppu_step(Ppu* ppu, Memory* mem, uint8_t ticks)
{
    ppu->ticks += ticks;

    switch (ppu->mode)
    {
        case OAM:
            if (ppu->ticks >= OAM_TICKS)
            {
                ppu_oam_search(ppu, mem);
                ppu->ticks -= OAM_TICKS;
                ppu->mode = RAM;
            }
            break;
        
        case RAM:
            if (ppu->ticks >= RAM_TICKS)
            {
                ppu_draw_scanline(ppu, mem);
                ppu->ticks -= RAM_TICKS;
                ppu->mode = HBLANK;
            }
            break;

        case HBLANK:
            if (ppu->ticks >= HBLANK_TICKS)
            {
                ppu->ticks -= HBLANK_TICKS;
                ppu->ly++;

                if (ppu->ly == SCREEN_HEIGHT)
                {
                    request_interrupt(mem, VBLANK_INTERRUPT);
                    ppu->mode = VBLANK;
                }
                else
                {
                    ppu->mode = OAM;
                }

                memory_write(mem, 0xFF44, ppu->ly);
            }
            break;

        case VBLANK:
            if (ppu->ticks >= VBLANK_TICKS)
            {
                ppu->ticks -= VBLANK_TICKS;
                ppu->ly++;

                if (ppu->ly > 153)
                {
                    ppu->ly = 0;
                    ppu->mode = OAM;
                }

                memory_write(mem, 0xFF44, ppu->ly);
            }
    }
}

Ppu* ppu_init()
{
    Ppu* ppu = (Ppu*) malloc(sizeof(Ppu));
    memset(ppu, 0, sizeof(Ppu));

    ppu->mode = OAM;
    ppu->ly = 0;
    ppu->ticks = 0;
    ppu->sprite_height = 8;
    ppu->visible_sprite_count = 0;
}

void ppu_get_framebuffer(Ppu* ppu, uint8_t out[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    memcpy(out, ppu->framebuffer, SCREEN_HEIGHT * SCREEN_WIDTH);
}