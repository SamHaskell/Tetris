#include "core/game.hpp"
#include "core/platform.hpp"
#include "maths/random.hpp"

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

static void input_set_keystate(KeyState& keystate, bool isDown) {
    if (isDown != keystate.IsDown) {
        keystate.TransitionCount ++;
    }
    keystate.IsDown = isDown;
}

static bool input_key_was_pressed_this_frame(KeyState& KeyState) {
    return (KeyState.IsDown && (KeyState.TransitionCount > 0));
}

static void game_handle_events(Context* context) {
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
                game_quit(context);
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        input_set_keystate(context->Inputs->Left, true);
                        break;
                    case SDLK_d:
                        input_set_keystate(context->Inputs->Right, true);
                        break;
                    case SDLK_s:
                        input_set_keystate(context->Inputs->Down, true);
                        break;
                    case SDLK_w:
                        input_set_keystate(context->Inputs->Up, true);
                        break;
                    case SDLK_LEFT:
                        input_set_keystate(context->Inputs->Left, true);
                        break;
                    case SDLK_RIGHT:
                        input_set_keystate(context->Inputs->Right, true);
                        break;
                    case SDLK_DOWN:
                        input_set_keystate(context->Inputs->Down, true);
                        break;
                    case SDLK_UP:
                        input_set_keystate(context->Inputs->Up, true);
                        break;
                    case SDLK_SPACE:
                        input_set_keystate(context->Inputs->Space, true);
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        input_set_keystate(context->Inputs->Left, false);
                        break;
                    case SDLK_d:
                        input_set_keystate(context->Inputs->Right, false);
                        break;
                    case SDLK_s:
                        input_set_keystate(context->Inputs->Down, false);
                        break;
                    case SDLK_w:
                        input_set_keystate(context->Inputs->Up, false);
                        break;
                    case SDLK_LEFT:
                        input_set_keystate(context->Inputs->Left, false);
                        break;
                    case SDLK_RIGHT:
                        input_set_keystate(context->Inputs->Right, false);
                        break;
                    case SDLK_DOWN:
                        input_set_keystate(context->Inputs->Down, false);
                        break;
                    case SDLK_UP:
                        input_set_keystate(context->Inputs->Up, false);
                        break;
                    case SDLK_SPACE:
                        input_set_keystate(context->Inputs->Space, false);
                        break;
                }
                break;
        }
    }
}

static void game_render_field(Context* context, i32 left, i32 bottom) {
    // Draw the walls.
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        Rect2D rectLeft = Rect2D(left, bottom + (j * 32), 32, 32);
        Rect2D rectRight = Rect2D(left + (FIELD_WIDTH + 1) * 32, bottom + (j * 32), 32, 32);
        Vec4 color = {0.3, 0.3, 0.3, 1.0};
        draw_quad_filled(context, color, rectLeft);
        draw_quad_filled(context, color, rectRight);
        draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, rectLeft);
        draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, rectRight);
    }

    // Draw the cells of the field.
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        for (i32 i = 1; i <= FIELD_WIDTH; i++) {
            u32 cell = context->GameState->Field[j][i - 1];
            Rect2D rect = Rect2D(left + (i * 32), bottom + (j * 32), 32, 32);
            Vec4 color = s_Shapes[cell].Color;
            draw_quad_filled(context, color, rect);
            draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, rect);
        }
    }

    // Draw the players active shape.
    f32 offsetX = (f32)((context->GameState->PlayerX + 1) * 32);
    f32 offsetY = (f32)(context->GameState->PlayerY * 32);
    shape_render(context, context->GameState->ActiveShape, offsetX, offsetY);
}

static void game_zero_field(Context* context) {
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        for (i32 i = 0; i < FIELD_WIDTH; i++) {
            context->GameState->Field[j][i] = 0;
        }
    }
}

static void field_set_cell(u32* field, u32 row, u32 col, u32 value) {
    field[(row * FIELD_WIDTH) + col] = value;
}

static void field_place_shape(u32* field, Shape shape, i32 shapeX, i32 shapeY) {
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) { 
            i32 row = (3 - j) + shapeY;
            i32 col = i + shapeX; 
            if (shape.Data[(j * 4) + i]) {
                field_set_cell(field, row, col, shape.ID);
            }
        }
    }
}

static void game_spawn_shape(Context* context, u32 ID) {
    context->GameState->PlayerX = 3;
    context->GameState->PlayerY = 14;
    context->GameState->ActiveShape = s_Shapes[ID];
}

static bool game_check_collision(Context* context, Shape shape, i32 shapeX, i32 shapeY) {
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            i32 row = (3 - j) + shapeY;
            i32 col = i + shapeX;
            bool isBoundary = (col >= FIELD_WIDTH || col < 0) || (row < 0);
            bool fieldVal = isBoundary;
            if (row < FIELD_HEIGHT) {
                fieldVal |= (bool)(context->GameState->Field[row][col]);
            }
            bool collision = (bool)shape.Data[(j * 4) + i] && fieldVal;
            if (collision) { 
                return true;
            }
        }
    }
    return false;
}

static void game_process_inputs(Context* context) {
    if (input_key_was_pressed_this_frame(context->Inputs->Up)) {
        Shape shape = context->GameState->ActiveShape;
        shape_rotate(shape);
        if (!game_check_collision(context, shape, context->GameState->PlayerX, context->GameState->PlayerY)) {
            shape_rotate(context->GameState->ActiveShape);
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Right)) {
        if (!game_check_collision(
            context, 
            context->GameState->ActiveShape, 
            context->GameState->PlayerX + 1, 
            context->GameState->PlayerY
            )
        ) {
            context->GameState->PlayerX ++;
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Left)) {
        if (!game_check_collision(
            context, 
            context->GameState->ActiveShape, 
            context->GameState->PlayerX - 1, 
            context->GameState->PlayerY
            )
        ) {
            context->GameState->PlayerX --;
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Down)) {
        if (!game_check_collision(
            context, 
            context->GameState->ActiveShape, 
            context->GameState->PlayerX, 
            context->GameState->PlayerY - 1
            )
        ) {
            context->GameState->PlayerY --;
        } else {
            field_place_shape(
                context->GameState->Field[0], 
                context->GameState->ActiveShape,
                context->GameState->PlayerX,
                context->GameState->PlayerY
            );
            game_spawn_shape(context, RandU32(1, 7));
        }

        context->GameState->ElapsedSinceLastMoveDown = 0.0;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Space)) {
        i32 dy = 1;
        while(!game_check_collision(
            context, 
            context->GameState->ActiveShape, 
            context->GameState->PlayerX, 
            context->GameState->PlayerY - dy
        )) {
            dy ++;
        }
        context->GameState->PlayerY -= dy - 1;
        field_place_shape(
            context->GameState->Field[0], 
            context->GameState->ActiveShape,
            context->GameState->PlayerX,
            context->GameState->PlayerY
        );
        game_spawn_shape(context, RandU32(1, 7));

        context->GameState->ElapsedSinceLastMoveDown = 0.0;
    }
}

static bool field_check_line(u32* field, u32 row) {
    bool isFull = true;
    for (i32 i = 0; i < FIELD_WIDTH; i++) {
        if (!field[row * FIELD_WIDTH + i]) {
            isFull = false;
            break;
        }
    }
    return isFull;
}

static void game_clear_lines(Context* context) {
    u32 lineCount = 0;
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        if (field_check_line(context->GameState->Field[0], j)) {
            lineCount ++;
        }
    }
    CX_DEBUG("Lines Full: %u", lineCount);
}

void game_init(Context* context) {
    context->GameState->MainFont = TTF_OpenFont("assets/pico/pico-8.ttf", 36);
    CX_ASSERT(context->GameState->MainFont != NULL, "Failed to load font!");

    game_zero_field(context);
    game_spawn_shape(context, RandU32(1, 7));

    context->GameState->DeltaTime = 1.0/60.0;
    context->GameState->ElapsedGameTime = 0.0;
    context->GameState->ElapsedSinceLastMoveDown = 0.0;
    context->GameState->TimeToMoveDown = 0.5;

    context->MainClock->Tick();
}

void game_update_and_render(Context* context) {

    CX_INFO("%lf", context->GameState->DeltaTime);

    game_handle_events(context);

    game_process_inputs(context);

    if (context->GameState->ElapsedSinceLastMoveDown > context->GameState->TimeToMoveDown) {
        if (!game_check_collision(
            context, 
            context->GameState->ActiveShape, 
            context->GameState->PlayerX, 
            context->GameState->PlayerY - 1
            )
        ) {
            context->GameState->PlayerY --;
        } else {
            field_place_shape(
                context->GameState->Field[0], 
                context->GameState->ActiveShape,
                context->GameState->PlayerX,
                context->GameState->PlayerY
            );
            game_spawn_shape(context, RandU32(1, 7));
        }

        context->GameState->ElapsedSinceLastMoveDown = 0.0;
    }

    game_clear_lines(context);

    game_render_field(context, 0, 0);

    draw_text(
        context,
        context->GameState->MainFont,
        "tetris!",
        {1.0, 1.0, 1.0, 1.0},
        464, 50
    );

    game_swap_buffers(context);

    context->GameState->DeltaTime = context->MainClock->Tick();
    context->GameState->ElapsedGameTime += context->GameState->DeltaTime;
    context->GameState->ElapsedSinceLastMoveDown += context->GameState->DeltaTime;
}