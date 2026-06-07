#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void get_mouse_pos(struct Game *g, float *x, float *y);

void game_events(struct Game *g) {

  while (SDL_PollEvent(&g->event)) {
    SDL_ConvertEventToRenderCoordinates(g->renderer, &g->event);
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
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      // jonathan blow
      if (g->event.button.button == 1) { // left click
        g->scene = 1; // go to level 1
      }
      break;

    default:
      break;
    }
  }
}

void game_draw(struct Game *g) {
  SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255); // black background
  SDL_RenderClear(g->renderer);

  if (g->scene == 0) {
    SDL_FRect dst_rect = {
        .x = 0, .y = 0, .w = WINDOW_WIDTH, .h = WINDOW_HEIGHT};
    SDL_RenderTexture(g->renderer, g->title_screen, NULL, &dst_rect);

    float xpos_play = 700;
    float ypos_play = 500;
    SDL_Texture *play_button = g->play_button_reg;
    SDL_FRect play_dst_rect = {
        .x = xpos_play, .y = ypos_play, .w = 180, .h = 60};
    float mosx;
    float mosy;
    get_mouse_pos(g, &mosx, &mosy);

    if (mosx >= xpos_play && mosx <= xpos_play + 180 && mosy >= ypos_play &&
        mosy <= ypos_play + 60) {
        // change to hover
        play_button = g->play_button_hover;
    }

    SDL_RenderTexture(g->renderer, play_button, NULL, &play_dst_rect);

  }
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

void get_mouse_pos(struct Game *g, float *x, float *y) {
  float mx, my;

  SDL_GetMouseState(&mx, &my);

  SDL_RenderCoordinatesFromWindow(g->renderer, mx, my, x, y);
}
