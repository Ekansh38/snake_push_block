#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
};

bool game_init_sdl(struct Game *g);
bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_run(struct Game *g);

bool game_init_sdl(struct Game *g) {
  if (!SDL_Init(SDL_FLAGS)) {
    fprintf(stderr, "Error initializing SDL3: %s\n", SDL_GetError());
    return false;
  }

  g->window = SDL_CreateWindow(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  if (!g->window) {
    fprintf(stderr, "Error creating Window: %s\n", SDL_GetError());
    return false;
  }

  g->renderer = SDL_CreateRenderer(g->window, NULL);
  if (!g->renderer) {
    fprintf(stderr, "Error creating Renderer: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

bool game_new(struct Game **game) {
  *game = calloc(1, sizeof(struct Game));
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
  SDL_RenderClear(g->renderer);

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

int main(void) {
  bool exit_status = EXIT_FAILURE;

  struct Game *game = NULL;

  if (game_new(&game)) {
    game_run(game);

    exit_status = EXIT_SUCCESS;
  }

  game_free(&game);

  return exit_status;
}
