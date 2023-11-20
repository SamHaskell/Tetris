#include "core/platform.hpp"
#include "core/game.hpp"

#include "maths/random.hpp"

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

    return context;
}

void platform_shutdown(Context* context) {
    TTF_Quit();

    SDL_DestroyRenderer(context->Renderer);
    SDL_DestroyWindow(context->WindowHandle);
    SDL_Quit();

    delete context->Game;
    delete context->Inputs;
    delete context;
    context = nullptr;
}

void platform_main_loop(void* memory) {
    Context* context = (Context*)memory;
    f64 dt = context->MainClock->Tick();
    game_update_and_render(context, dt);
}

void draw_quad_filled(Context* context, Vec4 color, Rect2D rect) {
    SDL_SetRenderDrawColor(
        context->Renderer,
        (i32)(color.x * 255.0f),
        (i32)(color.y * 255.0f),
        (i32)(color.z * 255.0f),
        (i32)(color.w * 255.0f)
    );
    SDL_Rect drawRect = { (i32)rect.x, context->WindowHeight - (i32)(rect.y + rect.h), (i32)rect.w, (i32)rect.h };
    SDL_RenderFillRect(context->Renderer, &drawRect);
}

void draw_quad_outline(Context* context, Vec4 color, Rect2D rect) {
    SDL_SetRenderDrawColor(
        context->Renderer,
        (i32)(color.x * 255.0f),
        (i32)(color.y * 255.0f),
        (i32)(color.z * 255.0f),
        (i32)(color.w * 255.0f)
    );
    SDL_Rect drawRect = { (i32)rect.x, context->WindowHeight - (i32)(rect.y + rect.h), (i32)rect.w, (i32)rect.h };
    SDL_RenderDrawRect(context->Renderer, &drawRect);
}

void draw_text(Context* context, TTF_Font* font, const char* text, Vec4 color, i32 left, i32 bottom) {
    SDL_Color textColor = {
        (u8)(color.x * 255.0),
        (u8)(color.y * 255.0),
        (u8)(color.z * 255.0),
        (u8)(color.w * 255.0), 
    };

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text,
        textColor
    );

    SDL_Texture* tex = SDL_CreateTextureFromSurface(
        context->Renderer,
        surface
    );

    SDL_FreeSurface(surface);

    i32 textWidth;
    i32 textHeight;

    SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect dst = {
        left,
        context->WindowHeight - bottom + textHeight,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(context->Renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}

void draw_text_centered(Context* context, TTF_Font* font, const char* text, Vec4 color, i32 centerX, i32 centerY) {
    SDL_Color textColor = {
        (u8)(color.x * 255.0),
        (u8)(color.y * 255.0),
        (u8)(color.z * 255.0),
        (u8)(color.w * 255.0), 
    };

    SDL_Surface* surface = TTF_RenderText_Blended(
        font,
        text,
        textColor
    );

    SDL_Texture* tex = SDL_CreateTextureFromSurface(
        context->Renderer,
        surface
    );

    SDL_FreeSurface(surface);

    i32 textWidth;
    i32 textHeight;

    SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect dst = {
        centerX - (textWidth / 2),
        context->WindowHeight - (centerY + (textHeight / 2)),
        textWidth,
        textHeight
    };

    SDL_RenderCopy(context->Renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}