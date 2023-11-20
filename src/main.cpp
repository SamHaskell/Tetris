#include "core/platform.hpp"
#include "core/game.hpp"
#include "maths/random.hpp"

int main(int argc, char* argv[]) {

    Context* context = new Context();

    i32 ok = SDL_Init(SDL_INIT_VIDEO);
    CX_ASSERT(ok == 0, "SDL failed to initialise.");

    context->WindowHandle = SDL_CreateWindow(
        "Tetris!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        704, 576,
        SDL_WINDOW_BORDERLESS
    );
    CX_ASSERT(context->WindowHandle != NULL, "SDL failed to create a valid window.");
    SDL_GetWindowSize(context->WindowHandle, &context->WindowWidth, &context->WindowHeight);

    context->Renderer = SDL_CreateRenderer(context->WindowHandle, -1, SDL_RENDERER_PRESENTVSYNC);
    CX_ASSERT(context->Renderer != NULL, "SDL failed to create a valid rendering context.");

    SDL_SetRenderDrawBlendMode(context->Renderer, SDL_BLENDMODE_BLEND);

    context->AudioEngine.init();

    ok = TTF_Init();
    CX_ASSERT(ok == 0, "TTF failed to initialise.");

    context->IsRunning = true;
    context->Inputs = new PlayerInputs();
    context->Game = new Game();
    context->MainClock = new Utils::Clock();

    SetGlobalSeed(SDL_GetPerformanceCounter());

    game_init(context);

#if CORTEX_PLATFORM_WEB
    emscripten_set_main_loop_arg(game_update_and_render, (void*)context, 60, true);
#else
    while(context->IsRunning) {
        game_update_and_render(context);
    }
#endif

    TTF_Quit();

    SDL_DestroyRenderer(context->Renderer);
    SDL_DestroyWindow(context->WindowHandle);
    SDL_Quit();

    delete context->Game;
    delete context->Inputs;
    delete context;
    context = nullptr;

    return 0;
}