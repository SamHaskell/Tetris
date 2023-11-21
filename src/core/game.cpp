#include "core/game.hpp"
#include "core/platform.hpp"
#include "maths/random.hpp"

#define COLOR_BACKGROUND {0.976, 0.90, 0.830, 1.0}
#define COLOR_WALLS {0.676, 0.50, 0.430, 1.0}
#define COLOR_OVERLAY {0.18, 0.1, 0.14, 0.8}

#define COLOR_TEXTBACKGROUND {0.8, 0.8, 0.8, 0.2}
#define COLOR_TEXT_DARK {0.1, 0.1, 0.16, 1.0}
#define COLOR_TEXT_LIGHT {0.9, 0.9, 0.84, 1.0}

#define COLOR_ACCENT {0.676, 0.50, 0.430, 1.0}

// Time it takes for the piece to move down one row when no inputs are pressed at the start of the game.
#define INIT_DROP_TIME 0.8

// Time it takes for the piece to move down one row when down is held.
#define QUICK_DROP_TIME 0.1

// Time it takes for the piece to slide to the side one col when left/right is held.
#define QUICK_SLIDE_TIME 0.1

static Vec4 s_Colors[8] = {
    COLOR_BACKGROUND,
    {0.000, 0.378, 0.576, 1.0},
    {0.004, 0.761, 0.706, 1.0},
    {0.957, 0.612, 0.184, 1.0},
    {0.914, 0.339, 0.227, 1.0},
    {0.792, 0.055, 0.347, 1.0},
    {0.812, 0.153, 0.890, 1.0},
    {0.298, 0.886, 0.414, 1.0},
};

static Shape s_Shapes[8] = {
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        .Color = s_Colors[0],
        .ID = 0,        
    },
    {
        .Data = {
            0, 0, 0, 0,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        },
        .Color = s_Colors[1],
        .ID = 1,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        .Color = s_Colors[2],
        .ID = 2,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 1,
            0, 0, 0, 0
        },
        .Color = s_Colors[3],
        .ID = 3,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 1,
            0, 1, 0, 0,
            0, 0, 0, 0
        },
        .Color = s_Colors[4],
        .ID = 4,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 1, 0,
            0, 1, 1, 1,
            0, 0, 0, 0
        },
        .Color = s_Colors[5],
        .ID = 5,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 0, 1, 1,
            0, 1, 1, 0,
            0, 0, 0, 0
        },
        .Color = s_Colors[6],
        .ID = 6,
    },
    {
        .Data = {
            0, 0, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 1,
            0, 0, 0, 0
        },
        .Color = s_Colors[7],
        .ID = 7,
    },
};

/*
    Rendering procedures.
*/

static void game_render_background(Context* context) {
    draw_quad_filled(
        context->Renderer,
        COLOR_BACKGROUND,
        {0, 0, (f32)context->WindowWidth, (f32)context->WindowHeight}
    );
}

static void game_render_decorations(Context* context) {

}

static void game_render_field(Context* context, i32 left, i32 top) {
    // Draw the walls.
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        Rect2D rectLeft = Rect2D(left, top + (j * 32), 32, 32);
        Rect2D rectRight = Rect2D(left + (FIELD_WIDTH + 1) * 32, top + (j * 32), 32, 32);
        draw_quad_filled(context->Renderer, COLOR_WALLS, rectLeft);
        draw_quad_filled(context->Renderer, COLOR_WALLS, rectRight);
        draw_quad_outline(context->Renderer, {0.0, 0.0, 0.0, 0.4}, rectLeft);
        draw_quad_outline(context->Renderer, {0.0, 0.0, 0.0, 0.4}, rectRight);
    }

    // Draw the cells of the field.
    for (i32 j = 0; j < FIELD_HEIGHT; j++) {
        for (i32 i = 1; i <= FIELD_WIDTH; i++) {
            u32 cell = context->Game->Field[j * FIELD_WIDTH + i - 1];
            Rect2D rect = Rect2D(left + (i * 32), top + ((FIELD_HEIGHT - j - 1) * 32), 32, 32);
            Vec4 color = s_Shapes[cell].Color;
            draw_quad_filled(context->Renderer, color, rect);
            if (cell) {
                draw_quad_outline(context->Renderer, {0.0, 0.0, 0.0, 0.4}, rect);
            }
            draw_quad_filled(context->Renderer, {1.0, 1.0, 1.0, 0.1}, rect);
        }
    }

    // Draw the players active shape.
    f32 offsetX = (f32)((context->Game->PlayerX + 1) * 32);
    f32 offsetY = (f32)((FIELD_HEIGHT - context->Game->PlayerY - 4) * 32);
    shape_render(context->Renderer, context->Game->CurrentShape, offsetX, offsetY);
}

static void game_render_score(Context* context, i32 left, i32 top) {
    // TODO: Render the score counter / statistics.
}

static void game_render_shape_preview(Context* context, i32 left, i32 top) {
    draw_quad_filled(context->Renderer, COLOR_ACCENT, {(f32)left - 16, (f32)top - 16, 160.0, 160.0});
    draw_quad_filled(context->Renderer, COLOR_BACKGROUND, {(f32)left - 12, (f32)top - 12, 152.0, 152.0});
    draw_quad_outline(context->Renderer, {0.0, 0.0, 0.0, 0.4}, {(f32)left - 16, (f32)top - 16, 160.0, 160.0});
    draw_quad_outline(context->Renderer, {0.0, 0.0, 0.0, 0.4}, {(f32)left - 12, (f32)top - 12, 152.0, 152.0});
    shape_render(context->Renderer, context->Game->NextShape, (f32)left, (f32)top);
    draw_text_centered(
        context->Renderer, 
        context->Game->MainFontMedium,
        "next",
        COLOR_TEXT_DARK,
        left + 64,
        top + 160 + 24
    );
}

static bool game_try_move(Context* context, i32 dx, i32 dy) {
    if (!field_check_collision(
        context->Game->Field, 
        context->Game->CurrentShape, 
        context->Game->PlayerX + dx, 
        context->Game->PlayerY + dy
        )
    ) {
        context->Game->PlayerX += dx;
        context->Game->PlayerY += dy;
        return true;
    }

    return false;
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
    u32 lineCount = field_clear_lines(context->Game->Field);
    context->Game->TimeToMoveDown *= pow(0.98, lineCount);
}

static void game_next_shape(Context* context, u32 ID) {
    context->Game->PlayerX = 3;
    context->Game->PlayerY = 16;
    context->Game->CurrentShape = context->Game->NextShape;
    context->Game->NextShape = s_Shapes[ID];
    context->Game->CanSwap = true;
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

    field_clear(context->Game->Field);
}

/*
    Input processing & logical update for each state.
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
        shape_swap(context->Game->CurrentShape, context->Game->NextShape);
        context->Game->PlayerX = 3;
        context->Game->PlayerY = 16;
        context->Game->CanSwap = false;
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Up)) {
        Shape shape = context->Game->CurrentShape;
        shape_rotate(shape);
        if (!field_check_collision(context->Game->Field, shape, context->Game->PlayerX, context->Game->PlayerY)) {
            shape_rotate(context->Game->CurrentShape);
        }
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Right)) {
        game_try_move(context, 1, 0);
    } else if (context->Inputs->Right.IsRepeat) {
        game_try_move(context, 1, 0);
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Left)) {
        game_try_move(context, -1, 0);
    } else if (context->Inputs->Left.IsRepeat) {
        game_try_move(context, -1, 0);
    }

    if (input_key_was_pressed_this_frame(context->Inputs->Down)) {
        if (!game_try_move(context, 0, -1)) {
            field_place_shape(
                context->Game->Field, 
                context->Game->CurrentShape,
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
            context->Game->Field, 
            context->Game->CurrentShape, 
            context->Game->PlayerX, 
            context->Game->PlayerY - dy
        )) {
            dy ++;
        }

        context->Game->PlayerY -= dy - 1;
        field_place_shape(
            context->Game->Field, 
            context->Game->CurrentShape,
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
        if (!game_try_move(context, 0, -1)) {
            field_place_shape(
                context->Game->Field, 
                context->Game->CurrentShape,
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

/*
    Main Game procedures.
*/

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

    // Run the base update for current state.

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

    // Rendering

    game_render_background(context);
    game_render_field(context, 0, 0);
    game_render_shape_preview(context, 480, 224);

    draw_text_centered(
        context->Renderer,
        context->Game->MainFontLarge,
        "tetris!",
        COLOR_TEXT_DARK,
        544, 48
    );

    game_render_decorations(context);

    if (context->Game->GameState != GameState::Playing) {
        draw_quad_filled(
            context->Renderer,
            COLOR_OVERLAY,
            { 0, 0, (f32)context->WindowWidth, (f32)context->WindowHeight }
        );
    }

    if (context->Game->GameState == GameState::Start) {
        draw_text_centered(
            context->Renderer,
            context->Game->MainFontLarge,
            "press space to begin",
            COLOR_TEXT_LIGHT,
            context->WindowWidth / 2,
            context->WindowHeight / 2
        );
    }

    if (context->Game->GameState == GameState::Paused) {
        draw_text_centered(
            context->Renderer,
            context->Game->MainFontLarge,
            "paused",
            COLOR_TEXT_LIGHT,
            context->WindowWidth / 2,
            context->WindowHeight / 2
        );
    }

    if (context->Game->GameState == GameState::GameOver) {
        draw_text_centered(
            context->Renderer,
            context->Game->MainFontLarge,
            "game over",
            COLOR_TEXT_LIGHT,
            context->WindowWidth / 2,
            (context->WindowHeight / 2) - 24
        );

        draw_text_centered(
            context->Renderer,
            context->Game->MainFontMedium,
            "press space to play again",
            COLOR_TEXT_LIGHT,
            context->WindowWidth / 2,
            (context->WindowHeight / 2) + 24
        );
    }

    context->Game->ElapsedGameTime += dt;
    context->Game->ElapsedSinceLastMoveDown += dt;
}