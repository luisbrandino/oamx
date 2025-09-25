#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include "../inc/ppu.h"

#define WINDOW_NAME "oamx"

typedef struct {
    uint8_t is_running;
} DisplayContext;

void display_init(DisplayContext* ctx);
void display_poll(DisplayContext* ctx);
void display_quit(DisplayContext* ctx);
void display_render(Ppu* ppu);

#endif