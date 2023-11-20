#pragma once

#include "core/base.h"
#include "core/utils.hpp"
#include "core/input.hpp"
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

/*
    Main platform layer functionality
*/

Context* platform_init();
void platform_shutdown(Context* context);
void platform_quit(Context* context);
void platform_main_loop(void* memory);

void platform_process_events(Context* context);
void platform_swap_buffers(SDL_Renderer* renderer);

/*
    Basic platform rendering API
*/

void draw_quad_filled(SDL_Renderer* renderer, Vec4 color, Rect2D rect);
void draw_quad_outline(SDL_Renderer* renderer, Vec4 color, Rect2D rect);
void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, Vec4 color, i32 left, i32 bottom);
void draw_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, Vec4 color, i32 centerX, i32 centerY);

/*
    TODO: Add some sort of platform Audio API to pull direct soloud calls out of game layer.
*/

