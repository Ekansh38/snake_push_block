#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef GAME_H
#define GAME_H

#define SDL_FLAGS SDL_INIT_VIDEO

#define WINDOW_TITLE "snake_push_block"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
  bool running;
  double delta_time;
  int scene; // 0 = start screen, 1 = level one, etc.
  SDL_Texture *title_screen;
  TTF_Font *font;

  // play button
  SDL_Texture *play_button_reg;
  SDL_Texture *play_button_hover;

  // goal text
  SDL_Surface *goal_surface;
  SDL_Texture *goal_texture;

  // last cached vars

  int last_cached_level_number;
  SDL_Surface *level_number_surface;
  SDL_Texture *level_number_texture;

};

bool game_init_sdl(struct Game *g);
bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_run(struct Game *g);

#endif
