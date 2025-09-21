#ifndef PPU_H
#define PPU_H

#include <stdint.h>
#include "memory.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

#define OAM_TICKS 80
#define RAM_TICKS 172
#define HBLANK_TICKS 204
#define VBLANK_TICKS 456

typedef enum {
    HBLANK,
    VBLANK,
    OAM,
    RAM
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
    PriorityType priority;
} Sprite;

typedef struct {
    PpuMode mode;
    uint8_t ticks;
    uint8_t ly;
    Sprite visible_sprites[10];
    uint8_t visible_sprite_count;
    uint8_t sprite_height;
    uint8_t framebuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
} Ppu;

Ppu* ppu_init();
void ppu_step(Ppu* ppu, Memory* mem, uint8_t ticks);
void ppu_get_framebuffer(Ppu* ppu, uint8_t out[SCREEN_HEIGHT][SCREEN_WIDTH]);

#endif