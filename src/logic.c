#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
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
        float xpos_play = 700;
        float ypos_play = 500;
        float mosx;
        float mosy;
        get_mouse_pos(g, &mosx, &mosy);

        if (mosx >= xpos_play && mosx <= xpos_play + 180 && mosy >= ypos_play &&
            mosy <= ypos_play + 60) {
          // change to hover
          g->scene = 1; // go to level 1
        }
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
  } else if (g->scene >= 1) {

    // draw top line
    SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
    SDL_FRect top_line_rect = {.x = 0, .y = 76, .w = WINDOW_WIDTH, .h = 4};
    SDL_RenderFillRect(g->renderer, &top_line_rect);

    // draw middle line
    int width_of_line = 2;
    SDL_FRect dividing_line_rect = {.x = WINDOW_WIDTH / 2 - (width_of_line / 2),
                                    .y = 0,
                                    .w = width_of_line,
                                    .h = WINDOW_HEIGHT};
    SDL_RenderFillRect(g->renderer, &dividing_line_rect);

    int cell_size = 32;
    int grid_y_start = 80;
    int grid_width = 640;
    int grid_height = 640;
    int number_of_columns = grid_width / cell_size; // 20
    int number_of_rows = grid_height / cell_size;   // 20

    SDL_SetRenderDrawColor(g->renderer, 70, 70, 70, 255);

    // columns
    int width_of_grid_line = 2;
    for (int i = 0; i < number_of_columns;
         i++) { // not <= cuz we already draw the middle line
      SDL_FRect line = {.x = i * cell_size,
                        .y = grid_y_start,
                        .w = width_of_grid_line,
                        .h = grid_height};
      SDL_RenderFillRect(g->renderer, &line);
    }
    // rows
    int pad = 0;
    for (int i = 1; i <= number_of_rows; i++) {
      if (i == number_of_rows) {
        pad = -width_of_grid_line;
      }
      SDL_FRect line = {.x = 0,
                        .y = i * cell_size + grid_y_start + pad,
                        .w = grid_width,
                        .h = width_of_grid_line};
      SDL_RenderFillRect(g->renderer, &line);
    }

    // draw right grid

    // columns
    for (int i = 1; i <= number_of_columns; i++) {
      SDL_FRect line = {.x = i * cell_size + grid_width,
                        .y = grid_y_start,
                        .w = width_of_grid_line,
                        .h = grid_height};
      SDL_RenderFillRect(g->renderer, &line);
    }
    // rows
    pad = 0;
    for (int i = 1; i <= number_of_rows; i++) {
      if (i == number_of_rows) {
        pad = -width_of_grid_line;
      }
      SDL_FRect line = {.x = grid_width,
                        .y = i * cell_size + grid_y_start + pad,
                        .w = grid_width,
                        .h = width_of_grid_line};
      SDL_RenderFillRect(g->renderer, &line);
    }

    // font drawing
    TTF_SetFontStyle(g->font, TTF_STYLE_NORMAL);
    TTF_SetFontOutline(g->font, 0);
    TTF_SetFontHinting(g->font, TTF_HINTING_NORMAL);

    float tw = g->goal_surface->w / 8.0f;
    float th = g->goal_surface->h / 8.0f;
    SDL_FRect text_rect = {
        .x = WINDOW_WIDTH / 4 - tw, .y = 76 / 2 - (th / 2), .w = tw, .h = th};
    SDL_RenderTexture(g->renderer, g->goal_texture, NULL, &text_rect);


    // implement the level text correctly
    // implement the timer text correctly

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
