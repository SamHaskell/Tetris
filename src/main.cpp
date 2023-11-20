#include "core/platform.hpp"
#include "core/game.hpp"
#include "maths/random.hpp"

int main(int argc, char* argv[]) {

    Context* context = platform_init();

    game_init(context);

#if CORTEX_PLATFORM_WEB
    emscripten_set_main_loop_arg(platform_main_loop, (void*)context, 60, true);
#else
    while(context->IsRunning) {
        platform_main_loop((void*)context);
    }
#endif

    platform_shutdown(context);
    return 0;
}