#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool game_init_sdl(struct Game *g) {
  if (!SDL_Init(SDL_FLAGS)) {
    fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
    return false;
  }

  g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
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
    return false;
  }

  g->running = true;

  return true;
}

void game_free(struct Game **game) {
  if (*game) {
    struct Game *g = *game;
    SDL_DestroyTexture(g->title_screen);
    SDL_DestroyTexture(g->play_button_reg);
    SDL_DestroyTexture(g->play_button_hover);

    if (g->renderer) {
      SDL_DestroyRenderer(g->renderer);
      g->renderer = NULL;
    }

    if (g->window) {
      SDL_DestroyWindow(g->window);
      g->window = NULL;
    }

    SDL_Quit();

    free(g);

    g = NULL;
    *game = NULL;
  }
}
