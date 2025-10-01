#include <time.h>
#include <assert.h>

#include "../inc/interrupts.h"
#include "../inc/display.h"
#include "../inc/memory.h"
#include "../inc/timer.h"
#include "../inc/cpu.h"
#include "../inc/ppu.h"
#include "../inc/rom.h"

#define GB_CLOCK_SPEED    4194304
#define GB_FPS            59.73
#define TICKS_PER_FRAME   (GB_CLOCK_SPEED / GB_FPS)
#define FRAME_DURATION    (1000000ULL / GB_FPS)

static inline uint64_t get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000ULL + (ts.tv_nsec / 1000ULL);
}

int main(int argc, char **argv)
{
    Memory* mem = memory_init();
    Cpu* cpu = cpu_init();
    Ppu* ppu = ppu_init();
    Timer timer = { 0, 0 };

    DisplayContext ctx;
    display_init(&ctx);

    assert(argc > 1);
    load_rom(mem, argv[1]);

    uint64_t next_frame_time = get_time_us() + FRAME_DURATION;
    while (ctx.is_running)
    {
        uint32_t frame_ticks = 0;
        while (frame_ticks < TICKS_PER_FRAME)
        {
            uint16_t ticks = cpu_step(cpu, mem);
            ppu_step(ppu, mem, ticks);
            handle_interrupts(cpu, ppu, mem);
            timer_update(&timer, mem, ticks);
            frame_ticks += ticks;
        }
    
        uint64_t now = get_time_us();
        if (now < next_frame_time)
        {
            uint64_t sleep_us = next_frame_time - now;
            struct timespec ts;
            ts.tv_sec = sleep_us / 1000000ULL;
            ts.tv_nsec = (sleep_us % 1000000ULL) * 1000ULL;
            nanosleep(&ts, NULL);
        }
        next_frame_time += FRAME_DURATION;

        display_poll(&ctx);
    }

    return 0;
}