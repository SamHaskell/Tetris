#include "core/platform.hpp"
#include "core/game.hpp"

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