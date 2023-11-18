#include "core/game.hpp"
#include "core/platform.hpp"

static Shape s_Shapes[8] = {
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        .Color = {
            1.0, 1.0, 1.0, 1.0
        },
        .ID = 0,        
    },
    {
        .Data = {
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        .Color = {
            0.996, 0.004, 0.0, 1.0
        },
        .ID = 1,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        .Color = {
            0.992, 0.6, 0.796, 1.0
        },
        .ID = 2,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 1,
            0, 0, 0, 0
        },
        .Color = {
            1.0, 0.396, 0.012, 1.0
        },
        .ID = 3,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 1,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        .Color = {
            1.0, 1.0, 0.0, 1.0
        },
        .ID = 4,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 1, 0,
            0, 1, 1, 1,
            0, 0, 0, 0
        },
        .Color = {
            0.0, 0.0, 0.996, 1.0
        },
        .ID = 5,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 1, 1,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        .Color = {
            0.0, 0.502, 0.004, 1.0
        },
        .ID = 6,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 1,
            0, 0, 0, 0
        },
        .Color = {
            0.506, 0.004, 0.498, 1.0
        },
        .ID = 7,
    },
};

static void game_swap_buffers(Context* context) {
    SDL_RenderPresent(context->Renderer);
    SDL_SetRenderDrawColor(context->Renderer, 20, 20, 20, 20);
    SDL_RenderClear(context->Renderer);
}

static void game_quit(Context* context) {
    #if CORTEX_PLATFORM_WEB
        emscripten_cancel_main_loop();
    #else
        context->IsRunning = false;
    #endif
}

static void game_set_keystate(KeyState& keystate, bool isDown) {
    if (isDown != keystate.IsDown) {
        keystate.TransitionCount ++;
    }
    keystate.IsDown = isDown;
}

static void game_handle_events(Context* context) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                game_quit(context);
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        game_set_keystate(context->Inputs->Left, true);
                        break;
                    case SDLK_d:
                        game_set_keystate(context->Inputs->Right, true);
                        break;
                    case SDLK_s:
                        game_set_keystate(context->Inputs->Down, true);
                        break;
                    case SDLK_w:
                        game_set_keystate(context->Inputs->Up, true);
                        break;
                    case SDLK_LEFT:
                        game_set_keystate(context->Inputs->Left, true);
                        break;
                    case SDLK_RIGHT:
                        game_set_keystate(context->Inputs->Right, true);
                        break;
                    case SDLK_DOWN:
                        game_set_keystate(context->Inputs->Down, true);
                        break;
                    case SDLK_UP:
                        game_set_keystate(context->Inputs->Up, true);
                        break;
                    case SDLK_SPACE:
                        game_set_keystate(context->Inputs->Space, true);
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        game_set_keystate(context->Inputs->Left, false);
                        break;
                    case SDLK_d:
                        game_set_keystate(context->Inputs->Right, false);
                        break;
                    case SDLK_s:
                        game_set_keystate(context->Inputs->Down, false);
                        break;
                    case SDLK_w:
                        game_set_keystate(context->Inputs->Up, false);
                        break;
                    case SDLK_LEFT:
                        game_set_keystate(context->Inputs->Left, false);
                        break;
                    case SDLK_RIGHT:
                        game_set_keystate(context->Inputs->Right, false);
                        break;
                    case SDLK_DOWN:
                        game_set_keystate(context->Inputs->Down, false);
                        break;
                    case SDLK_UP:
                        game_set_keystate(context->Inputs->Up, false);
                        break;
                    case SDLK_SPACE:
                        game_set_keystate(context->Inputs->Space, false);
                        break;
                }
                break;
        }
    }
}

void game_update_and_render(Context* context) {
    game_handle_events(context);

    
    

    game_swap_buffers(context);
}