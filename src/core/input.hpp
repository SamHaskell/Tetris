#pragma once

#include "core/base.h"

struct KeyState {
    bool IsDown = false;
    bool IsRepeat = false;
    u32 TransitionCount = 0;
};

struct PlayerInputs {
    KeyState Up;
    KeyState Right;
    KeyState Down;
    KeyState Left;
    KeyState Space;
    KeyState Back;
    KeyState Swap;
};

void input_set_keystate(KeyState& keystate, bool isDown, bool isRepeat);

bool input_key_was_pressed_this_frame(KeyState& KeyState);
bool input_key_was_held_this_frame(KeyState& KeyState);