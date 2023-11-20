#include "core/platform.hpp"
#include "core/game.hpp"

#include "maths/random.hpp"

static SDL_Color color_from_vec4(Vec4 color) {
    return {
        (u8)(color.x * 255.0),
        (u8)(color.y * 255.0),
        (u8)(color.z * 255.0),
        (u8)(color.w * 255.0)
    };
}

Context* platform_init() {
    Context* context = new Context();

    /*
        Initialising SDL. Note we only intiialised Video, since we are targeting emscripten.
    */

    i32 ok = SDL_Init(SDL_INIT_VIDEO);
    CX_ASSERT(ok == 0, "SDL failed to initialise.");

    /*
        Create a window.
        TODO: Pull hard-coded windowdims out.
    */

    context->WindowHandle = SDL_CreateWindow(
        "Tetris!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        704, 576,
        SDL_WINDOW_BORDERLESS
    );

    CX_ASSERT(context->WindowHandle != NULL, "SDL failed to create a valid window.");
    SDL_GetWindowSize(context->WindowHandle, &context->WindowWidth, &context->WindowHeight);

    /*
        Create SDL renderer. Must set blend mode to allow for alpha blending and transparency.
    */

    context->Renderer = SDL_CreateRenderer(context->WindowHandle, -1, SDL_RENDERER_PRESENTVSYNC);
    CX_ASSERT(context->Renderer != NULL, "SDL failed to create a valid rendering context.");

    SDL_SetRenderDrawBlendMode(context->Renderer, SDL_BLENDMODE_BLEND);

    /*
        Initialise SoLoud engine.
    */

    context->AudioEngine.init();


    /*
        Initialise SDL_TTF.
    */
    
    ok = TTF_Init();
    CX_ASSERT(ok == 0, "TTF failed to initialise.");

    /*
        Set game to run, create Input and Game data structs, initialise the Main Clock.
    */

    context->IsRunning = true;
    context->Inputs = new PlayerInputs();
    context->Game = new Game();
    context->MainClock = new Utils::Clock();

    /*
        Seed the PRNG.
    */

    SetGlobalSeed(SDL_GetPerformanceCounter());

    game_init(context);

    return context;
}

void platform_shutdown(Context* context) {

    game_shutdown(context);

    TTF_Quit();

    SDL_DestroyRenderer(context->Renderer);
    SDL_DestroyWindow(context->WindowHandle);
    SDL_Quit();

    delete context->Game;
    delete context->Inputs;
    delete context;
    context = nullptr;
}

void platform_quit(Context* context) {
    #if CORTEX_PLATFORM_WEB
        emscripten_cancel_main_loop();
    #else
        context->IsRunning = false;
    #endif
}

void platform_main_loop(void* memory) {
    Context* context = (Context*)memory;
    f64 dt = context->MainClock->Tick();
    
    platform_process_events(context);
    game_update_and_render(context, dt);

    platform_swap_buffers(context->Renderer);
}

void platform_process_events(Context* context) {
    context->Inputs->Left.TransitionCount = 0;
    context->Inputs->Right.TransitionCount = 0;
    context->Inputs->Up.TransitionCount = 0;
    context->Inputs->Down.TransitionCount = 0;
    context->Inputs->Space.TransitionCount = 0;
    context->Inputs->Back.TransitionCount = 0;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                platform_quit(context);
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        input_set_keystate(context->Inputs->Left, true, (e.key.repeat != 0));
                        break;
                    case SDLK_d:
                        input_set_keystate(context->Inputs->Right, true, (e.key.repeat != 0));
                        break;
                    case SDLK_s:
                        input_set_keystate(context->Inputs->Down, true, (e.key.repeat != 0));
                        break;
                    case SDLK_w:
                        input_set_keystate(context->Inputs->Up, true, (e.key.repeat != 0));
                        break;
                    case SDLK_LEFT:
                        input_set_keystate(context->Inputs->Left, true, (e.key.repeat != 0));
                        break;
                    case SDLK_RIGHT:
                        input_set_keystate(context->Inputs->Right, true, (e.key.repeat != 0));
                        break;
                    case SDLK_DOWN:
                        input_set_keystate(context->Inputs->Down, true, (e.key.repeat != 0));
                        break;
                    case SDLK_UP:
                        input_set_keystate(context->Inputs->Up, true, (e.key.repeat != 0));
                        break;
                    case SDLK_SPACE:
                        input_set_keystate(context->Inputs->Space, true, (e.key.repeat != 0));
                        break;
                    case SDLK_ESCAPE:
                        input_set_keystate(context->Inputs->Back, true, (e.key.repeat != 0));
                        break;
                    case SDLK_e:
                        input_set_keystate(context->Inputs->Swap, true, (e.key.repeat != 0));
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        input_set_keystate(context->Inputs->Left, false, (e.key.repeat != 0));
                        break;
                    case SDLK_d:
                        input_set_keystate(context->Inputs->Right, false, (e.key.repeat != 0));
                        break;
                    case SDLK_s:
                        input_set_keystate(context->Inputs->Down, false, (e.key.repeat != 0));
                        break;
                    case SDLK_w:
                        input_set_keystate(context->Inputs->Up, false, (e.key.repeat != 0));
                        break;
                    case SDLK_LEFT:
                        input_set_keystate(context->Inputs->Left, false, (e.key.repeat != 0));
                        break;
                    case SDLK_RIGHT:
                        input_set_keystate(context->Inputs->Right, false, (e.key.repeat != 0));
                        break;
                    case SDLK_DOWN:
                        input_set_keystate(context->Inputs->Down, false, (e.key.repeat != 0));
                        break;
                    case SDLK_UP:
                        input_set_keystate(context->Inputs->Up, false, (e.key.repeat != 0));
                        break;
                    case SDLK_SPACE:
                        input_set_keystate(context->Inputs->Space, false, (e.key.repeat != 0));
                        break;
                    case SDLK_ESCAPE:
                        input_set_keystate(context->Inputs->Back, false, (e.key.repeat != 0));
                        break;
                    case SDLK_e:
                        input_set_keystate(context->Inputs->Swap, false, (e.key.repeat != 0));
                        break;
                }
                break;
        }
    }
}

/*
    Swaps buffers and clears the new backbuffer to magenta.
*/

void platform_swap_buffers(SDL_Renderer* renderer) {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderClear(renderer);
}

void draw_quad_filled(SDL_Renderer* renderer, Vec4 color, Rect2D rect) {
    SDL_Color col = color_from_vec4(color);
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    SDL_Rect drawRect = { (i32)rect.x, (i32)rect.y, (i32)rect.w, (i32)rect.h };
    SDL_RenderFillRect(renderer, &drawRect);
}

void draw_quad_outline(SDL_Renderer* renderer, Vec4 color, Rect2D rect) {
    SDL_Color col = color_from_vec4(color);
    SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a);
    SDL_Rect drawRect = { (i32)rect.x, (i32)rect.y, (i32)rect.w, (i32)rect.h };
    SDL_RenderDrawRect(renderer, &drawRect);
}

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, Vec4 color, i32 left, i32 top) {
    SDL_Color textColor = color_from_vec4(color);

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text,
        textColor
    );

    SDL_Texture* tex = SDL_CreateTextureFromSurface(
        renderer,
        surface
    );

    SDL_FreeSurface(surface);

    i32 textWidth;
    i32 textHeight;

    SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect dst = {
        left,
        top,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}

void draw_text_centered(SDL_Renderer* renderer, TTF_Font* font, const char* text, Vec4 color, i32 centerX, i32 centerY) {
    SDL_Color textColor = color_from_vec4(color);

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text,
        textColor
    );

    SDL_Texture* tex = SDL_CreateTextureFromSurface(
        renderer,
        surface
    );

    SDL_FreeSurface(surface);

    i32 textWidth;
    i32 textHeight;

    SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect dst = {
        centerX - (textWidth / 2),
        centerY + (textHeight / 2),
        textWidth,
        textHeight
    };

    SDL_RenderCopy(renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}