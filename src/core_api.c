#include "../inc/core_api.h"
#include <stdio.h>

GB_cpu_snapshot_t cpu_snapshot;
atomic_bool syncing;

uint8_t GB_get_cpu_snapshot(GB_cpu_snapshot_t* cpu_snapshot_out)
{
    if (atomic_load(&syncing))
    {
        return 0;
    }

    *cpu_snapshot_out = cpu_snapshot;

    return 1;
}

void GB_sync_snapshot(Cpu* cpu, Memory* mem)
{
    atomic_store(&syncing, 1);

    cpu_snapshot.a = cpu->a;
    cpu_snapshot.f = cpu->f;
    cpu_snapshot.b = cpu->b;
    cpu_snapshot.c = cpu->c;
    cpu_snapshot.d = cpu->d;
    cpu_snapshot.e = cpu->e;
    cpu_snapshot.h = cpu->h;
    cpu_snapshot.l = cpu->l;
    cpu_snapshot.pc = cpu->pc;
    cpu_snapshot.sp = cpu->sp;
    cpu_snapshot.ime = cpu->ime;

    atomic_store(&syncing, 0);
}