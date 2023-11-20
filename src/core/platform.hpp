#pragma once

#include "core/base.h"
#include "core/utils.hpp"
#include "maths/linalg.hpp"
#include "maths/geometry.hpp"

#include "soloud.h"

#if CORTEX_PLATFORM_WEB
    #include "SDL.h"
    #include "SDL_ttf.h"
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_ttf.h>
#endif

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

struct Game;

struct Context {
    bool IsRunning;
    i32 WindowWidth;
    i32 WindowHeight;
    SDL_Window* WindowHandle;
    SDL_Renderer* Renderer;
    SoLoud::Soloud  AudioEngine;
    PlayerInputs* Inputs;
    Utils::Clock* MainClock;
    Game* Game;
};

void draw_quad_filled(Context* context, Vec4 color, Rect2D rect);
void draw_quad_outline(Context* context, Vec4 color, Rect2D rect);

void draw_text(Context* context, TTF_Font* font, const char* text, Vec4 color, i32 left, i32 bottom);
void draw_text_centered(Context* context, TTF_Font* font, const char* text, Vec4 color, i32 centerX, i32 centerY);