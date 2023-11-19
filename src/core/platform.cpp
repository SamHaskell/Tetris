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

    // PROBLEM
    SDL_Texture* tex = SDL_CreateTextureFromSurface(
        context->Renderer,
        surface
    );
    //

    SDL_FreeSurface(surface);

    i32 textWidth;
    i32 textHeight;

    SDL_QueryTexture(tex, NULL, NULL, &textWidth, &textHeight);

    SDL_Rect dst = {
        left,
        bottom,
        textWidth,
        textHeight
    };

    SDL_RenderCopy(context->Renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
}