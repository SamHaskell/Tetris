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
            0.14, 0.14, 0.18, 1.0
        },
        .ID = 0,        
    },
    {
        .Data = {
            0, 0, 0, 0,
            1, 1, 1, 1,
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

/*
    Miscellaneous functions, platform actions etc.
*/

static void game_swap_buffers(Context* context) {
    SDL_RenderPresent(context->Renderer);
    SDL_SetRenderDrawColor(context->Renderer, 20, 20, 20, 20);
    SDL_RenderClear(context->Renderer);
}

static void game_swap_shapes(Context* context) {
    Shape temp = context->Game->ActiveShape;
    context->Game->ActiveShape = context->Game->NextShape;
    context->Game->NextShape = temp;
}

static void game_quit(Context* context) {
    #if CORTEX_PLATFORM_WEB
        emscripten_cancel_main_loop();
    #else
        context->IsRunning = false;
    #endif
}

static void input_set_keystate(KeyState& keystate, bool isDown, bool isRepeat) {
    if (isDown != keystate.IsDown) {
        keystate.TransitionCount ++;
    }
    keystate.IsDown = isDown;
    keystate.IsRepeat = isRepeat;
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
    Rendering.
*/

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
            u32 cell = context->Game->Field[j][i - 1];
            Rect2D rect = Rect2D(left + (i * 32), bottom + (j * 32), 32, 32);
            Vec4 color = s_Shapes[cell].Color;
            draw_quad_filled(context, color, rect);
            if (cell) {
                draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, rect);
            }
        }
    }

    // Draw the players active shape.
    f32 offsetX = (f32)((context->Game->PlayerX + 1) * 32);
    f32 offsetY = (f32)(context->Game->PlayerY * 32);
    shape_render(context, context->Game->ActiveShape, offsetX, offsetY);
}

static void game_render_shape_preview(Context* context, i32 left, i32 bottom) {
    draw_quad_filled(context, s_Shapes[0].Color, {(f32)left, (f32)bottom, 128.0, 128.0});
    draw_quad_outline(context, {0.0, 0.0, 0.0, 1.0}, {(f32)left, (f32)bottom, 128.0, 128.0});
    shape_render(context, context->Game->NextShape, (f32)left, (f32)bottom);
    draw_text_centered(
        context, 
        context->Game->MainFontMedium,
        "next",
        {1.0, 1.0, 1.0, 1.0},
        left + 64,
        bottom
    );
}

/*
    Logic helpers.
*/

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

static void game_next_shape(Context* context, u32 ID) {
    context->Game->PlayerX = 3;
    context->Game->PlayerY = 14;
    context->Game->ActiveShape = context->Game->NextShape;
    context->Game->NextShape = s_Shapes[ID];
    context->Game->CanSwap = true;
}

static bool game_check_collision(Context* context, Shape shape, i32 shapeX, i32 shapeY) {
    for (i32 i = 0; i < 4; i++) {
        for (i32 j = 0; j < 4; j++) {
            i32 row = (3 - j) + shapeY;
            i32 col = i + shapeX;
            bool isBoundary = (col >= FIELD_WIDTH || col < 0) || (row < 0);
            bool fieldVal = isBoundary;
            if (row < FIELD_HEIGHT) {
                fieldVal |= (bool)(context->Game->Field[row][col]);
            }
            bool collision = (bool)shape.Data[(j * 4) + i] && fieldVal;
            if (collision) { 
                return true;
            }
        }
    }
    return false;
}

static void game_zero_field(Context* context) {
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        for (i32 i = 0; i < FIELD_WIDTH; i++) {
            context->Game->Field[j][i] = 0;
        }
    }
}

static void game_clear_lines(Context* context) {
    u32 lineCount = 0;
    for (i32 j = FIELD_HEIGHT - 1; j >= 0; j--) {
        if (field_check_line(context->Game->Field[0], j)) {
            // Delete the line and move everything down to adjust.
            // Iterate upwards setting each row to the value of the row above.
            for (i32 row = j; row < FIELD_HEIGHT - 1; row++) {
                memcpy(context->Game->Field[row], context->Game->Field[row + 1], sizeof(u32) * FIELD_WIDTH);
            }
            // Zero out the top row to simulate pulling an empty row from above.
            memset(context->Game->Field[FIELD_HEIGHT - 1], 0, sizeof(u32) * FIELD_WIDTH);

            // Increment linecount for scoring.
            lineCount ++;
        }
    }
}

/*
    Transitions for each state.
*/

static void game_start_play(Context* context) {
    context->Game->CanSwap = true;
    context->Game->NextShape = s_Shapes[RandU32(1, 7)];
    context->Game->GameState = GameState::Playing;
    game_next_shape(context, RandU32(1, 7));
    context->Game->ElapsedGameTime = 0.0;
    context->Game->ElapsedSinceLastMoveDown = 0.0;
    context->Game->TimeToMoveDown = 0.5;
}

/*
    Input processing for each state.
*/

static void gamestate_start_update(Context* context) {
    if (input_key_was_pressed_this_frame(context->Inputs->Space)) {
        game_start_play(context);
    }
}

static void gamestate_playing_update(Context* context) {
    if (input_key_was_pressed_this_frame(context->Inputs->Swap) && context->Game->CanSwap) {
        game_swap_shapes(context);
        context->Game->CanSwap = false;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Up)) {
        Shape shape = context->Game->ActiveShape;
        shape_rotate(shape);
        if (!game_check_collision(context, shape, context->Game->PlayerX, context->Game->PlayerY)) {
            shape_rotate(context->Game->ActiveShape);
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Right)) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX + 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX ++;
        }
    } else if (context->Inputs->Right.IsRepeat) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX + 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX ++;
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Left)) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX - 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX --;
        }
    } else if (context->Inputs->Left.IsRepeat) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX - 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX --;
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Down)) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX, 
            context->Game->PlayerY - 1
            )
        ) {
            context->Game->PlayerY --;
        } else {
            field_place_shape(
                context->Game->Field[0], 
                context->Game->ActiveShape,
                context->Game->PlayerX,
                context->Game->PlayerY
            );
            game_next_shape(context, RandU32(1, 7));
        }

        context->Game->ElapsedSinceLastMoveDown = 0.0;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Space)) {
        i32 dy = 1;
        while(!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX, 
            context->Game->PlayerY - dy
        )) {
            dy ++;
        }
        context->Game->PlayerY -= dy - 1;
        field_place_shape(
            context->Game->Field[0], 
            context->Game->ActiveShape,
            context->Game->PlayerX,
            context->Game->PlayerY
        );
        game_next_shape(context, RandU32(1, 7));

        context->Game->ElapsedSinceLastMoveDown = 0.0;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Back)) {
        context->Game->GameState = GameState::Paused;
        return;
    }

    if (context->Game->ElapsedSinceLastMoveDown > context->Game->TimeToMoveDown) {
        if (!game_check_collision(
            context, 
            context->Game->ActiveShape, 
            context->Game->PlayerX, 
            context->Game->PlayerY - 1
            )
        ) {
            context->Game->PlayerY --;
        } else {
            field_place_shape(
                context->Game->Field[0], 
                context->Game->ActiveShape,
                context->Game->PlayerX,
                context->Game->PlayerY
            );
            game_next_shape(context, RandU32(1, 7));
        }

        context->Game->ElapsedSinceLastMoveDown = 0.0;
    }

    game_clear_lines(context);
}

static void gamestate_paused_update(Context* context) {
    if (input_key_was_pressed_this_frame(context->Inputs->Back)) {
        context->Game->GameState = GameState::Playing;
    }
}

static void gamestate_gameover_update(Context* context) {
    if (input_key_was_pressed_this_frame(context->Inputs->Space)) {
        context->Game->GameState = GameState::Start;
    }
}

void game_init(Context* context) {
    context->Game->MainFontLarge = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_LARGE);
    CX_ASSERT(context->Game->MainFontLarge != NULL, "Failed to load font!");

    context->Game->MainFontMedium = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_MEDIUM);
    CX_ASSERT(context->Game->MainFontMedium != NULL, "Failed to load font!");

    context->Game->MainFontSmall = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_SMALL);
    CX_ASSERT(context->Game->MainFontSmall != NULL, "Failed to load font!");
    
    game_zero_field(context);

    context->Game->DeltaTime = 1.0/60.0;
    context->MainClock->Tick();

    context->Game->GameState = GameState::Start;
}

void game_update_and_render(void* mem) {
    Context* context = (Context*)mem;
    
    game_handle_events(context);

    if (context->Inputs->Space.IsRepeat) {
        CX_INFO("REPEAT!");
    } else if (input_key_was_pressed_this_frame(context->Inputs->Space)) {
        CX_INFO("AWWW");
    }

    switch (context->Game->GameState) {
        case GameState::Start:
            gamestate_start_update(context);
            break;
        case GameState::Playing:
            gamestate_playing_update(context);
            break;
        case GameState::Paused:
            gamestate_paused_update(context);
            break;
        case GameState::GameOver:
            gamestate_gameover_update(context);
            break;

    }

    game_render_field(context, 0, 0);
    game_render_shape_preview(context, 480, 224);

    if (context->Game->GameState == GameState::Paused) {
        draw_text_centered(
            context,
            context->Game->MainFontLarge,
            "paused",
            {1.0, 1.0, 1.0, 1.0},
            192, 544
        );
    }

    draw_text_centered(
        context,
        context->Game->MainFontLarge,
        "tetris!",
        {1.0, 1.0, 1.0, 1.0},
        544, 544
    );

    game_swap_buffers(context);

    context->Game->DeltaTime = context->MainClock->Tick();
    context->Game->ElapsedGameTime += context->Game->DeltaTime;
    context->Game->ElapsedSinceLastMoveDown += context->Game->DeltaTime;
}