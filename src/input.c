#include <SDL2/SDL.h>
#include "../inc/input.h"

void update_key_states(Memory* mem)
{
    const Uint8* key_states = SDL_GetKeyboardState(NULL);

    if (key_states[SDL_SCANCODE_Z]) mem->joypad_state &= ~(KEY_A << 4); else mem->joypad_state |= (KEY_A << 4);
    if (key_states[SDL_SCANCODE_X]) mem->joypad_state &= ~(KEY_B << 4); else mem->joypad_state |= (KEY_B << 4);
    if (key_states[SDL_SCANCODE_RSHIFT]) mem->joypad_state &= ~(KEY_SELECT << 4); else mem->joypad_state |= (KEY_SELECT << 4);
    if (key_states[SDL_SCANCODE_RETURN]) mem->joypad_state &= ~(KEY_START << 4); else mem->joypad_state |= (KEY_START << 4);

    if (key_states[SDL_SCANCODE_RIGHT]) mem->joypad_state &= ~KEY_RIGHT; else mem->joypad_state |= KEY_RIGHT;
    if (key_states[SDL_SCANCODE_LEFT]) mem->joypad_state &= ~KEY_LEFT; else mem->joypad_state |= KEY_LEFT;
    if (key_states[SDL_SCANCODE_UP]) mem->joypad_state &= ~KEY_UP; else mem->joypad_state |= KEY_UP;
    if (key_states[SDL_SCANCODE_DOWN]) mem->joypad_state &= ~KEY_DOWN; else mem->joypad_state |= KEY_DOWN;
}