#pragma once

#include "core/base.h"
#include "core/utils.hpp"
#include "maths/linalg.hpp"
#include "maths/geometry.hpp"

#if CORTEX_PLATFORM_WEB
    #include "SDL.h"
#else
    #include <SDL2/SDL.h>
#endif

struct KeyState {
    bool IsDown = false;
    u32 TransitionCount = 0;
};

struct PlayerInputs {
    KeyState Up;
    KeyState Right;
    KeyState Down;
    KeyState Left;
    KeyState Space;
    KeyState Back;
};

struct GameState;

struct Context {
    bool IsRunning;
    i32 WindowWidth;
    i32 WindowHeight;
    SDL_Window* WindowHandle;
    SDL_Renderer* Renderer;
    PlayerInputs* Inputs;
    Utils::Clock* MainClock;
    GameState* GameState;
};

void draw_quad_filled(Context* context, Vec4 color, Rect2D rect);
void draw_quad_outline(Context* context, Vec4 color, Rect2D rect);