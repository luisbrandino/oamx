#include <stdlib.h>
#include <string.h>

#include "../inc/interrupts.h"
#include "../inc/platform.h"
#include "../inc/display.h"
#include "../inc/ppu.h"

static inline void request_stat_interrupt_if_enabled(Memory* mem, uint8_t mask)
{
    if (mem->stat & mask)
        request_interrupt(mem, LCD_STAT_INTERRUPT);
}

static Sprite read_sprite(Memory* mem, uint8_t index)
{
    uint8_t sprite_y = mem->oam[index * 4];
    uint8_t sprite_x = mem->oam[index * 4 + 1];
    uint8_t tile = mem->oam[index * 4 + 2];
    uint8_t flags = mem->oam[index * 4 + 3];
    PriorityType priority = (flags & (1 << 7)) ? BEHIND_BG : OVERLAP_BG;

    return (Sprite) { sprite_y, sprite_x, tile, flags, index, priority };
}

static uint16_t get_tile_base_address(uint8_t is_unsigned, int16_t tile_index)
{
    if (is_unsigned)
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
    uint8_t pixel = (((b2 & mask) >> (7 - x)) << 1) | ((b1 & mask) >> (7 - x));
    return pixel & 0x03;
}

static uint8_t ppu_get_sprite_pixel(Ppu* ppu, Memory* mem, uint16_t tile_index, uint8_t y, uint8_t x, uint8_t flags)
{
    y = (flags & (1 << 6)) ? (ppu->sprite_height - 1) - y : y;
    x = (flags & (1 << 5)) ? 7 - x : x;

    uint16_t tile_to_use = tile_index;
    if (ppu->sprite_height == 16)
    {
        tile_to_use &= 0xFE; 
        if (y >= 8)
        {
            tile_to_use++;
            y -= 8;
        }
    }

    uint16_t tile_base_address = get_tile_base_address(UNSIGNED_TILE_INDEX, tile_to_use);
    return get_tile_pixel_from_vram(mem, tile_base_address, y, x);
}

static void ppu_render_element_pixel(Ppu* ppu, Memory* mem, uint8_t screen_x, uint16_t element_x, uint16_t element_y, uint16_t tilemap_base)
{
    uint8_t tile_col = element_x / 8;
    uint8_t tile_row = element_y / 8;
    uint8_t line_x = element_x % 8;
    uint8_t line_y = element_y % 8;

    uint16_t tilemap_index = tile_row * 32 + tile_col;
    uint8_t tile_index = memory_read(mem, tilemap_base + tilemap_index);

    uint8_t is_unsigned = mem->lcdc & (1 << 4) ? UNSIGNED_TILE_INDEX : SIGNED_TILE_INDEX;
    uint16_t tile_base_address = get_tile_base_address(is_unsigned, is_unsigned ? tile_index : (int8_t)tile_index);
    uint8_t color = get_tile_pixel_from_vram(mem, tile_base_address, line_y, line_x);

    uint8_t bgp = mem->bgp;
    uint8_t shade = (bgp >> (color * 2)) & 0x03;

    ppu->framebuffer[mem->ly][screen_x] = shade;
}

static void ppu_draw_window(Ppu* ppu, Memory* mem)
{
    if (!(mem->lcdc & LCDC_WINDOW_ENABLED))
        return;

    uint8_t window_x = mem->wx - 7;
    uint8_t window_y = mem->wy;

    if (mem->ly < window_y)
        return;

    if (mem->ly >= window_y)
        ppu->window_line_counter = 0;

    uint8_t screen_rendered = 0;
    for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
    {
        if (x < window_x)
            continue;

        screen_rendered = 1;
        uint16_t element_x = x - window_x;
        uint16_t element_y = ppu->window_line_counter;
        uint16_t tilemap_base = (mem->lcdc & (1 << 6)) ? 0x9C00 : 0x9800;

        ppu_render_element_pixel(ppu, mem, x, element_x, element_y, tilemap_base);
    }

    if (screen_rendered)
        ppu->window_line_counter++;
}

static void ppu_draw_background(Ppu* ppu, Memory* mem)
{
    uint8_t scx = mem->scx;
    uint8_t scy = mem->scy;

    for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
    {
        uint16_t element_x = (uint8_t)(x + scx);
        uint16_t element_y = (uint8_t)(mem->ly + scy);
        uint16_t tilemap_base = (mem->lcdc & (1 << 3)) ? 0x9C00 : 0x9800;
        
        ppu_render_element_pixel(ppu, mem, x, element_x, element_y, tilemap_base);
    }
}

static void ppu_oam_search(Ppu* ppu, Memory* mem)
{
    uint8_t lcdc = mem->lcdc;
    ppu->sprite_height = (lcdc & (1 << 2)) ? 16 : 8;
    ppu->visible_sprite_count = 0;

    for (uint8_t i = 0; i < 40; i++)
    {
        Sprite sprite = read_sprite(mem, i);
        
        if ((mem->ly + 16) >= sprite.y && (mem->ly + 16) < (sprite.y + ppu->sprite_height))
        {
            ppu->visible_sprites[ppu->visible_sprite_count++] = sprite;
            if (ppu->visible_sprite_count >= 10)
                break;
        }
    }
}

static void ppu_draw_sprites(Ppu* ppu, Memory* mem)
{
    ppu_oam_search(ppu, mem);
    Sprite* pixel_sprite_map[SCREEN_WIDTH] = { NULL };
    for (uint8_t i = 0; i < ppu->visible_sprite_count; i++)
    {
        Sprite* sprite = &ppu->visible_sprites[i];
        int sprite_x_in_screen = sprite->x - 8;
        uint8_t line_in_sprite = mem->ly - (sprite->y - 16);

        for (int px = 0; px < 8; px++)
        {
            uint8_t screen_x = sprite_x_in_screen + px;
            if (screen_x < 0 || screen_x >= SCREEN_WIDTH)
                continue;

            uint8_t color = ppu_get_sprite_pixel(ppu, mem, sprite->tile, line_in_sprite, px, sprite->flags);
            if (color == 0)
                continue;

            uint8_t bg_pixel = ppu->framebuffer[mem->ly][screen_x];
            if (sprite->priority == BEHIND_BG && bg_pixel != 0)
                continue;

            Sprite *other_sprite = pixel_sprite_map[screen_x];
            if (other_sprite != NULL)
            {
                if (other_sprite->x < sprite->x)
                    continue;

                if (other_sprite->x == sprite->x && other_sprite->oam_index < sprite->oam_index)
                    continue;
            }

            uint8_t palette = (sprite->flags & (1 << 4)) ? mem->obp1 : mem->obp0;
            uint8_t shade = (palette >> (color * 2)) & 0x03;

            pixel_sprite_map[screen_x] = sprite;
            ppu->framebuffer[mem->ly][screen_x] = shade;
        }
    }
}

static void ppu_draw_scanline(Ppu* ppu, Memory* mem)
{   
    ppu_draw_background(ppu, mem);
    ppu_draw_window(ppu, mem);

    if (!(mem->lcdc & LCDC_SPRITE_ENABLED))
        return;

    ppu_draw_sprites(ppu, mem);
}

static inline void ppu_enter_mode(Ppu* ppu, Memory* mem, PpuMode mode)
{
    ppu->mode = mode;
    mem->stat = ((mem->stat & 0xFC) | ppu->mode);
}

static void ppu_update_lyc_flag(Ppu* ppu, Memory* mem)
{
    if (mem->ly != mem->lyc)
    {
        mem->stat &= (uint8_t)~(STAT_LYC_FLAG);
        return;
    }

    mem->stat |= STAT_LYC_FLAG;
    if (mem->stat & STAT_INT_LYC_ENABLE)
        request_interrupt(mem, LCD_STAT_INTERRUPT);
}

uint8_t ppu_is_lcd_off(Ppu* ppu, Memory* mem)
{
    uint8_t lcd_off = mem->lcdc & 0x80 == 0;

    if (lcd_off)
    {
        ppu->lcd_on = 0;
        ppu->mode = HBLANK;
        return lcd_off;
    }

    if (!lcd_off && !ppu->lcd_on)
    {
        ppu->lcd_on = 1;
        ppu->ticks = 0;
    }

    return lcd_off;
}

void ppu_handle_oam(Ppu* ppu, Memory* mem)
{
    if (ppu->ticks >= OAM_TICKS)
    {
        ppu->ticks -= OAM_TICKS;
        ppu_enter_mode(ppu, mem, VRAM);
    }
}

void ppu_handle_vram(Ppu* ppu, Memory* mem)
{
    if (ppu->ticks >= VRAM_TICKS)
    {
        ppu->ticks -= VRAM_TICKS;
        ppu_enter_mode(ppu, mem, HBLANK);

        if (mem->ly < SCREEN_HEIGHT)
            ppu_draw_scanline(ppu, mem);

        request_stat_interrupt_if_enabled(mem, STAT_INT_HBLANK_ENABLE);
    }
}

void ppu_handle_hblank(Ppu* ppu, Memory* mem)
{
    if (ppu->ticks >= HBLANK_TICKS)
    {
        ppu->ticks -= HBLANK_TICKS;
        mem->ly++;

        ppu_update_lyc_flag(ppu, mem);

        if (mem->ly == SCREEN_HEIGHT)
        {
            request_interrupt(mem, VBLANK_INTERRUPT);
            ppu_enter_mode(ppu, mem, VBLANK);
            display_render(ppu);
            request_stat_interrupt_if_enabled(mem, STAT_INT_VBLANK_ENABLE);
        }
        else
        {
            ppu_enter_mode(ppu, mem, OAM);
            request_stat_interrupt_if_enabled(mem, STAT_INT_OAM_ENABLE);
        }
    }
}

void ppu_handle_vblank(Ppu* ppu, Memory* mem)
{
    if (ppu->ticks >= VBLANK_TICKS)
    {
        ppu->ticks -= VBLANK_TICKS;
        mem->ly++;

        ppu_update_lyc_flag(ppu, mem);

        if (mem->ly >= (SCREEN_HEIGHT + 9))
        {   
            mem->ly = 0;
            ppu_enter_mode(ppu, mem, OAM);
            request_stat_interrupt_if_enabled(mem, STAT_INT_OAM_ENABLE);
        }
    }
}

void ppu_step(Ppu* ppu, Memory* mem, uint8_t ticks)
{
    ppu->ticks += ticks;

    if (unlikely(ppu_is_lcd_off(ppu, mem)))
        return;

    switch (ppu->mode)
    {
        case OAM:       return ppu_handle_oam(ppu, mem);
        case VRAM:      return ppu_handle_vram(ppu, mem);
        case HBLANK:    return ppu_handle_hblank(ppu, mem);
        case VBLANK:    return ppu_handle_vblank(ppu, mem);
        nodefault
    }
}

Ppu* ppu_init()
{
    Ppu* ppu = (Ppu*) malloc(sizeof(Ppu));
    memset(ppu, 0, sizeof(Ppu));

    ppu->mode = OAM;
    ppu->ticks = 0;
    ppu->sprite_height = 8;
    ppu->visible_sprite_count = 0;
}