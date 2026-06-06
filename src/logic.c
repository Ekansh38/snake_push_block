#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


void game_events(struct Game *g) {
  while (SDL_PollEvent(&g->event)) {
    switch (g->event.type) {
    case SDL_EVENT_QUIT:
      g->running = false;
      break;
    case SDL_EVENT_KEY_DOWN:
      switch (g->event.key.scancode) {
      default:
        break;
      }
      break;
    default:
      break;
    }
  }
}

void game_draw(struct Game *g) {
  SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);

  SDL_RenderClear(g->renderer);

  SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);

  SDL_RenderPresent(g->renderer);
}

void game_run(struct Game *g) {
  Uint64 last_time = SDL_GetTicksNS();

  while (g->running) {
    Uint64 current_time = SDL_GetTicksNS();
    Uint64 delta_time = current_time - last_time;
    g->delta_time = (double)delta_time / SDL_NS_PER_SECOND;
    last_time = current_time;

    game_events(g);

    game_draw(g);
  }
}
