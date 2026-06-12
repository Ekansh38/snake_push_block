#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool game_init_sdl(struct Game *g) {
    if (!SDL_Init(SDL_FLAGS)) {
        fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
        return false;
    }

    g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT,
                                 SDL_WINDOW_RESIZABLE);
    if (!g->window) {
        fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
        return false;
    }

    g->renderer = SDL_CreateRenderer(g->window, NULL);
    if (!g->renderer) {
        fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderLogicalPresentation(g->renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    if (!TTF_Init()) {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", SDL_GetError());
        return false;
    }

    g->font = TTF_OpenFont("assets/font.ttf", 256);
    if (!g->font) {
        fprintf(stderr, "Error loading font: %s\n", SDL_GetError());
        return false;
    }

    // Goal text
    SDL_Color text_color = {.r = 255, .g = 255, .b = 255, .a = 255};
    g->goal_surface = TTF_RenderText_Blended(g->font, "Goal", 4, text_color);
    if (g->goal_surface) {
        g->goal_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->goal_surface);
        if (g->goal_texture) {
            SDL_SetTextureScaleMode(
                g->goal_texture,
                SDL_SCALEMODE_LINEAR); // see if this actually matters
        }
    }

    // loss screen text, rendered once cuz it never changes
    SDL_Color death_color = {.r = 255, .g = 80, .b = 80, .a = 255};
    g->death_surface = TTF_RenderText_Blended(g->font, "You Died", 8, death_color);
    if (g->death_surface) {
        g->death_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->death_surface);
        if (g->death_texture) {
            SDL_SetTextureScaleMode(g->death_texture, SDL_SCALEMODE_LINEAR);
        }
    }

    g->home_surface = TTF_RenderText_Blended(g->font, "Home", 4, text_color);
    if (g->home_surface) {
        g->home_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->home_surface);
        if (g->home_texture) {
            SDL_SetTextureScaleMode(g->home_texture, SDL_SCALEMODE_LINEAR);
        }
    }

    g->retry_surface = TTF_RenderText_Blended(g->font, "Retry", 5, text_color);
    if (g->retry_surface) {
        g->retry_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->retry_surface);
        if (g->retry_texture) {
            SDL_SetTextureScaleMode(g->retry_texture, SDL_SCALEMODE_LINEAR);
        }
    }

    g->pause_surface = TTF_RenderText_Blended(g->font, "Paused", 6, text_color);
    if (g->pause_surface) {
        g->pause_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->pause_surface);
        if (g->pause_texture) {
            SDL_SetTextureScaleMode(g->pause_texture, SDL_SCALEMODE_LINEAR);
        }
    }

    g->level_number_surface = TTF_RenderText_Blended(
        g->font, "Level: 1", 8,
        text_color);
    g->last_cached_level_number = 1;
    if (g->goal_surface) {
        g->level_number_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->level_number_surface);
        if (g->level_number_texture) {
            SDL_SetTextureScaleMode(g->level_number_texture,
                                    SDL_SCALEMODE_LINEAR);
        }
    }

    g->last_cached_timer_number = 0.0;
    // Timer text
    char timer_buffer[32];
    snprintf(timer_buffer, sizeof(timer_buffer), "Time: %.1f",
             g->last_cached_timer_number); // bye bye buffer overflow
    g->timer_text_surface = TTF_RenderText_Blended(g->font, timer_buffer, 9,
                                                   text_color); // nine of now
    if (g->timer_text_surface) {
        g->timer_text_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->timer_text_surface);
        if (g->timer_text_texture) {
            SDL_SetTextureScaleMode(g->timer_text_texture,
                                    SDL_SCALEMODE_LINEAR);
        }
    }

    g->title_screen = IMG_LoadTexture(g->renderer, "assets/title.png");
    if (g->title_screen == NULL) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return false;
    }
    SDL_SetTextureScaleMode(g->title_screen, SDL_SCALEMODE_NEAREST);

    // play button assets
    g->play_button_reg =
        IMG_LoadTexture(g->renderer, "assets/play_button_reg.png");
    if (g->play_button_reg == NULL) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return false;
    }
    SDL_SetTextureScaleMode(g->play_button_reg, SDL_SCALEMODE_NEAREST);

    g->play_button_hover =
        IMG_LoadTexture(g->renderer, "assets/play_button_hover.png");
    if (g->play_button_hover == NULL) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
        return false;
    }
    SDL_SetTextureScaleMode(g->play_button_hover, SDL_SCALEMODE_NEAREST);

    return true;
}

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game)); // zeros it nicely like a good boy
    if (*game == NULL) {
        fprintf(stderr, "Error Calloc of New Game.\n");
        return false;
    }
    struct Game *g = *game;

    if (!game_init_sdl(g)) {
        game_free(game);
        return false;
    }

    g->running = true;
    g->selected_level = 1;

    return true;
}

void game_free(struct Game **game) {
    if (*game) {
        struct Game *g = *game;
        if (g->title_screen)
            SDL_DestroyTexture(g->title_screen);
        if (g->play_button_reg)
            SDL_DestroyTexture(g->play_button_reg);
        if (g->play_button_hover)
            SDL_DestroyTexture(g->play_button_hover);

        SDL_DestroyTexture(g->goal_texture);
        SDL_DestroySurface(g->goal_surface);

        SDL_DestroyTexture(g->level_number_texture);
        SDL_DestroySurface(g->level_number_surface);

        SDL_DestroySurface(g->timer_text_surface);
        SDL_DestroyTexture(g->timer_text_texture);

        SDL_DestroyTexture(g->death_texture);
        SDL_DestroySurface(g->death_surface);

        SDL_DestroyTexture(g->home_texture);
        SDL_DestroySurface(g->home_surface);

        SDL_DestroyTexture(g->retry_texture);
        SDL_DestroySurface(g->retry_surface);

        SDL_DestroyTexture(g->pause_texture);
        SDL_DestroySurface(g->pause_surface);

        if (g->font)
            TTF_CloseFont(g->font);

        if (g->renderer) {
            SDL_DestroyRenderer(g->renderer);
            g->renderer = NULL;
        }

        if (g->window) {
            SDL_DestroyWindow(g->window);
            g->window = NULL;
        }

        TTF_Quit();
        SDL_Quit();

        free(g);

        g = NULL;
        *game = NULL;
    }
}
