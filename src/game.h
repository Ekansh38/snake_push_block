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

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

enum BlockType {
    GREEN,
    RED,
    BLUE,
    PINK,
    YELLOW,
};

struct SnekSegment {
    int x;
    int y;
};

struct Block {
    int x;
    int y;
    enum BlockType type;
};

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

    // level number text

    int last_cached_level_number;
    SDL_Surface *level_number_surface;
    SDL_Texture *level_number_texture;

    // timer text

    float timer_value;
    float last_cached_timer_number;
    SDL_Surface *timer_text_surface;
    SDL_Texture *timer_text_texture;

    // actual snake
    
    int cell_size;

    struct SnekSegment snake[64];
    float movement_timer;
    float movement_interval;
    int snake_length;
    enum Direction direction;
    bool dead;

    // blocks/sokoban logic

    struct Block current_blocks[512];
    struct Block goal_blocks[512];
    int num_of_blocks;

    bool stopped;

    // loss screen, death_scene remembers the level for retry
    int death_scene;
    SDL_Surface *death_surface;
    SDL_Texture *death_texture;
    SDL_Surface *home_surface;
    SDL_Texture *home_texture;
    SDL_Surface *retry_surface;
    SDL_Texture *retry_texture;
};

bool game_init_sdl(struct Game *g);
bool game_new(struct Game **game);
void game_free(struct Game **game);
void game_events(struct Game *g);
void game_draw(struct Game *g);
void game_run(struct Game *g);

#endif
