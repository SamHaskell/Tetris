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
        bottom - 24
    );
}

/*
    Logic helpers.
*/

static void game_next_shape(Context* context, u32 ID) {
    context->Game->PlayerX = 3;
    context->Game->PlayerY = 16;
    context->Game->ActiveShape = context->Game->NextShape;
    context->Game->NextShape = s_Shapes[ID];
    context->Game->CanSwap = true;
}

/*
    Following GameBoy Tetris rules:
    A loss occurs when a piece locks on its starting position twice in a row.
    It is possible to move the piece out from existing blocks.

    TODO: Implement lock delay so that this doesn't feel unfair.
*/

static bool game_check_lose(Context* context) {

    return false;
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

    context->Game->TimeToMoveDown *= pow(0.98, lineCount);
    CX_INFO("%lf", context->Game->TimeToMoveDown);
}

/*
    Transitions for each state.
*/

static void game_start_play(Context* context) {
    context->Game->CanSwap = true;
    context->Game->GameState = GameState::Playing;
    context->Game->ElapsedGameTime = 0.0;
    context->Game->ElapsedSinceLastMoveDown = 0.0;
    context->Game->TimeToMoveDown = 0.5;

    context->Game->NextShape = s_Shapes[RandU32(1, 7)];
    game_next_shape(context, RandU32(1, 7));

    field_clear(context->Game->Field[0]);
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
    
    // Check for a gameover.
    if (game_check_lose(context)) {
        context->Game->GameState = GameState::GameOver;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Swap) && context->Game->CanSwap) {
        shape_swap(context->Game->ActiveShape, context->Game->NextShape);
        context->Game->PlayerX = 3;
        context->Game->PlayerY = 16;
        context->Game->CanSwap = false;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Up)) {
        Shape shape = context->Game->ActiveShape;
        shape_rotate(shape);
        if (!field_check_collision(context->Game->Field[0], shape, context->Game->PlayerX, context->Game->PlayerY)) {
            shape_rotate(context->Game->ActiveShape);
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Right)) {
        if (!field_check_collision(
            context->Game->Field[0], 
            context->Game->ActiveShape, 
            context->Game->PlayerX + 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX ++;
        }
    } else if (context->Inputs->Right.IsRepeat) {
        if (!field_check_collision(
            context->Game->Field[0], 
            context->Game->ActiveShape, 
            context->Game->PlayerX + 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX ++;
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Left)) {
        if (!field_check_collision(
            context->Game->Field[0], 
            context->Game->ActiveShape, 
            context->Game->PlayerX - 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX --;
        }
    } else if (context->Inputs->Left.IsRepeat) {
        if (!field_check_collision(
            context->Game->Field[0], 
            context->Game->ActiveShape, 
            context->Game->PlayerX - 1, 
            context->Game->PlayerY
            )
        ) {
            context->Game->PlayerX --;
        }
    }

    context->Game->TimeToMoveDown = input_key_was_held_this_frame(context->Inputs->Down) ? 0.1 : 0.8;

    if (input_key_was_pressed_this_frame(context->Inputs->Down)) {
        if (!field_check_collision(
            context->Game->Field[0], 
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
        while(!field_check_collision(
            context->Game->Field[0], 
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
            context->AudioEngine.play(context->Game->KickSFX);
        game_next_shape(context, RandU32(1, 7));

        context->Game->ElapsedSinceLastMoveDown = 0.0;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Back)) {
        context->Game->GameState = GameState::Paused;
        return;
    }

    if (context->Game->ElapsedSinceLastMoveDown > context->Game->TimeToMoveDown) {
        if (!field_check_collision(
            context->Game->Field[0], 
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
            context->AudioEngine.play(context->Game->KickSFX);
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
        game_start_play(context);
    }
}

void game_init(Context* context) {
    context->Game->MainFontLarge = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_LARGE);
    CX_ASSERT(context->Game->MainFontLarge != NULL, "Failed to load font!");

    context->Game->MainFontMedium = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_MEDIUM);
    CX_ASSERT(context->Game->MainFontMedium != NULL, "Failed to load font!");

    context->Game->MainFontSmall = TTF_OpenFont("pico/pico-8.ttf", FONT_SIZE_SMALL);
    CX_ASSERT(context->Game->MainFontSmall != NULL, "Failed to load font!");

    context->Game->BGM.load("audio/realm.mp3");
    context->Game->BGM.setLooping(1);
    context->AudioEngine.play(context->Game->BGM);

    context->Game->KickSFX.load("audio/click2.wav");
    context->Game->KickSFX.setLooping(0);

    context->Game->GameState = GameState::Start;
}

void game_shutdown(Context* context) {
    // TODO: Clean up resources here.
}

void game_update_and_render(Context* context, f64 dt) {
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

    draw_text_centered(
        context,
        context->Game->MainFontLarge,
        "tetris!",
        {1.0, 1.0, 1.0, 1.0},
        544, 544
    );

    if (context->Game->GameState != GameState::Playing) {
        draw_quad_filled(
            context,
            {0.1, 0.1, 0.2, 0.6},
            {
                0, 0, (f32)context->WindowWidth, (f32)context->WindowHeight
            }
        );
    }

    if (context->Game->GameState == GameState::Start) {
        draw_text_centered(
            context,
            context->Game->MainFontLarge,
            "press space to begin",
            {1.0, 1.0, 1.0, 1.0},
            context->WindowWidth / 2,
            context->WindowHeight / 2
        );
    }

    if (context->Game->GameState == GameState::Paused) {
        draw_text_centered(
            context,
            context->Game->MainFontLarge,
            "paused",
            {1.0, 1.0, 1.0, 1.0},
            192, 544
        );
    }

    if (context->Game->GameState == GameState::GameOver) {
        draw_text_centered(
            context,
            context->Game->MainFontLarge,
            "game over",
            {1.0, 1.0, 1.0, 1.0},
            context->WindowWidth / 2,
            (context->WindowHeight / 2) + 24
        );

        draw_text_centered(
            context,
            context->Game->MainFontMedium,
            "press space to play again",
            {1.0, 1.0, 1.0, 1.0},
            context->WindowWidth / 2,
            (context->WindowHeight / 2) - 24
        );
    }

    context->Game->ElapsedGameTime += dt;
    context->Game->ElapsedSinceLastMoveDown += dt;
}