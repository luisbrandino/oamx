#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include "memory.h"

#define SCREEN_WIDTH  160
#define SCREEN_HEIGHT 144

#define OAM_TICKS    80
#define VRAM_TICKS   172
#define HBLANK_TICKS 204
#define VBLANK_TICKS 456

#define LCDC_SPRITE_ENABLED (1 << 1)
#define LCDC_WINDOW_ENABLED (1 << 5)
#define LCDC_LCD_ENABLED    (1 << 7)

#define SIGNED_TILE_INDEX   0
#define UNSIGNED_TILE_INDEX 1

#define STAT_LYC_FLAG            (1 << 2)

#define STAT_INT_LYC_ENABLE      (1 << 6)
#define STAT_INT_HBLANK_ENABLE   (1 << 3)
#define STAT_INT_VBLANK_ENABLE   (1 << 4)
#define STAT_INT_OAM_ENABLE      (1 << 5)

typedef enum {
    HBLANK,
    VBLANK,
    OAM,
    VRAM
} PpuMode;

typedef enum {
    OVERLAP_BG = 0,
    BEHIND_BG = 1
} PriorityType;

typedef struct {
    uint8_t y;
    uint8_t x;
    uint8_t tile;
    uint8_t flags;
    uint8_t oam_index;
    PriorityType priority;
} Sprite;

typedef struct {
    PpuMode mode;
    uint16_t ticks;
    Sprite visible_sprites[10];
    uint8_t visible_sprite_count;
    uint8_t sprite_height;
    uint8_t window_line_counter;
    uint8_t lcd_on;
    uint8_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
} Ppu;

Ppu* ppu_init();
void ppu_step(Ppu* ppu, Memory* mem, uint8_t ticks);

#endif