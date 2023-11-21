#include "core/input.hpp"

void input_set_keystate(KeyState& keystate, bool isDown, bool isRepeat) {
    if (isDown != keystate.IsDown) {
        keystate.TransitionCount ++;
    }
    keystate.IsDown = isDown;
    keystate.IsRepeat = isRepeat;
}

bool input_key_was_pressed_this_frame(KeyState& KeyState) {
    return (KeyState.IsDown && (KeyState.TransitionCount > 0));
}

bool input_key_was_held_this_frame(KeyState& KeyState) {
    return (KeyState.IsDown && (KeyState.TransitionCount == 0));
}