#ifndef INPUT_H
#define INPUT_H

#include "memory.h"

#define KEY_RIGHT  (1 << 0)
#define KEY_LEFT   (1 << 1)
#define KEY_UP     (1 << 2)
#define KEY_DOWN   (1 << 3)

#define KEY_A      (1 << 0)
#define KEY_B      (1 << 1)
#define KEY_SELECT (1 << 2)
#define KEY_START  (1 << 3)

void update_key_states(Memory* mem);

#endif