#include "game.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void get_mouse_pos(struct Game *g, float *x, float *y);

void update_level_text(struct Game *g, int number) {

    char the_string[32];
    sprintf(the_string, "Level: %d", number);

    SDL_Color text_color = {.r = 255, .g = 255, .b = 255, .a = 255};
    g->level_number_surface = TTF_RenderText_Blended(
        g->font, the_string, strlen(the_string), text_color);
    g->last_cached_level_number = number;
    if (g->level_number_surface) {
        g->level_number_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->level_number_surface);
        if (g->level_number_texture) {
            SDL_SetTextureScaleMode(g->level_number_texture,
                                    SDL_SCALEMODE_LINEAR);
        }
    }
}

void update_timer(struct Game *g, float number) {

    char the_string[32];
    snprintf(the_string, sizeof(the_string), "Time: %.1f", number);

    SDL_Color text_color = {.r = 255, .g = 255, .b = 255, .a = 255};
    g->timer_text_surface = TTF_RenderText_Blended(
        g->font, the_string, strlen(the_string), text_color);
    g->last_cached_timer_number = number;
    if (g->timer_text_surface) {
        g->timer_text_texture =
            SDL_CreateTextureFromSurface(g->renderer, g->timer_text_surface);
        if (g->timer_text_texture) {
            SDL_SetTextureScaleMode(g->timer_text_texture,
                                    SDL_SCALEMODE_LINEAR);
        }
    }
}

void game_events(struct Game *g) {

    while (SDL_PollEvent(&g->event)) {
        SDL_ConvertEventToRenderCoordinates(g->renderer, &g->event);
        switch (g->event.type) {
        case SDL_EVENT_QUIT:
            g->running = false;
            break;
        case SDL_EVENT_KEY_DOWN: {
            enum Direction new_direction = g->direction;
            switch (g->event.key.scancode) {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                new_direction = UP;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                new_direction = LEFT;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                new_direction = DOWN;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                new_direction = RIGHT;
                break;
            default:
                break;
            }
            // reject 180 turns
            if (new_direction != g->direction) {
                bool opposite =
                    (new_direction == UP && g->direction == DOWN) ||
                    (new_direction == DOWN && g->direction == UP) ||
                    (new_direction == LEFT && g->direction == RIGHT) ||
                    (new_direction == RIGHT && g->direction == LEFT);
                if (!opposite) {
                    g->direction = new_direction;
                }
            }
        } break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            // jonathan blow
            if (g->event.button.button == 1) { // left click
                float xpos_play = 700;
                float ypos_play = 500;
                float mosx;
                float mosy;
                get_mouse_pos(g, &mosx, &mosy);

                if (mosx >= xpos_play && mosx <= xpos_play + 180 &&
                    mosy >= ypos_play && mosy <= ypos_play + 60) {
                    // change to hover
                    g->scene = 1; // go to level 1

                    // setup level 1 stuff

                    g->timer_value = 0; // ik its already zero but who cares?
                    g->movement_interval = 0.1;
                    // setup snake itself
                    g->snake[3] = (struct SnekSegment){.x = 9, .y = 9};
                    g->snake[2] = (struct SnekSegment){.x = 10, .y = 9};
                    g->snake[1] = (struct SnekSegment){.x = 11, .y = 9};
                    g->snake[0] = (struct SnekSegment){.x = 12, .y = 9};
                    g->snake_length = 4;
                    g->direction = RIGHT;
                    g->dead = false;
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

        int top_area = 80;

        // TODO!! Update title screen

        // draw middle line
        int width_of_line = 4;
        SDL_FRect dividing_line_rect = {.x = WINDOW_WIDTH / 2 -
                                             (width_of_line / 2),
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
                                                        //
        // draw the snake, head is red, body green

        SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255); // red head
                                                             //
        SDL_FRect head_rect = {.x = (g->snake[0].x * cell_size) + grid_width,
                               .y = (g->snake[0].y * cell_size) + top_area,
                               .w = cell_size + 2,
                               .h = cell_size + 2};
        SDL_RenderFillRect(g->renderer, &head_rect);

        if (!g->dead) {
            SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255); // green body
        }

        for (int i = 1; i < g->snake_length; i++) {

            SDL_FRect body_rect = {.x =
                                       (g->snake[i].x * cell_size) + grid_width,
                                   .y = (g->snake[i].y * cell_size) + top_area,
                                   .w = cell_size + 2,
                                   .h = cell_size + 2};
            SDL_RenderFillRect(g->renderer, &body_rect);
        }

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
                              .w = grid_width - 2,
                              .h = width_of_grid_line};
            SDL_RenderFillRect(g->renderer, &line);
        }

        // draw right grid

        // columns
        pad = 0;
        for (int i = 1; i <= number_of_columns; i++) {
            if (i == number_of_columns) {
                pad = -2;
            }
            SDL_FRect line = {.x = i * cell_size + grid_width + pad,
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
            SDL_FRect line = {.x = grid_width + 2,
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
        SDL_FRect text_rect = {.x = WINDOW_WIDTH / 4 - (tw / 2),
                               .y = 76 / 2 - (th / 2),
                               .w = tw,
                               .h = th};
        SDL_RenderTexture(g->renderer, g->goal_texture, NULL, &text_rect);

        // implement the level text correctly
        if (g->scene == 1) {
            if (g->last_cached_level_number != 1) {
                update_level_text(g, 1);
            }
        } else if (g->scene == 2) {
            if (g->last_cached_level_number != 2) {
                update_level_text(g, 2);
            }
        }

        tw = g->level_number_surface->w / 8.0f;
        th = g->level_number_surface->h / 8.0f;
        SDL_FRect level_rect = {.x = (WINDOW_WIDTH / 2) +
                                     ((WINDOW_WIDTH / 2) / 3) - (tw / 2) - 30,
                                .y = 76 / 2 - (th / 2),
                                .w = tw,
                                .h = th};
        SDL_RenderTexture(g->renderer, g->level_number_texture, NULL,
                          &level_rect);

        // timer stuff
        g->timer_value += g->delta_time;
        if (fabs(g->timer_value - g->last_cached_timer_number) > 0.1f) {
            update_timer(g, g->timer_value);
        }

        tw = g->timer_text_surface->w / 8.0f;
        th = g->timer_text_surface->h / 8.0f;
        SDL_FRect timer_rect = {.x = (WINDOW_WIDTH / 2) +
                                     ((WINDOW_WIDTH / 2) / 3) * 2 - (tw / 2) +
                                     30,
                                .y = 76 / 2 - (th / 2),
                                .w = tw,
                                .h = th};
        SDL_RenderTexture(g->renderer, g->timer_text_texture, NULL,
                          &timer_rect);

        if (!g->dead) {
            g->movement_timer += g->delta_time;
            if (g->movement_timer >= g->movement_interval) {
                g->movement_timer = 0;

                int new_x = g->snake[0].x;
                int new_y = g->snake[0].y;

                switch (g->direction) {
                case UP:
                    new_y -= 1;
                    break;
                case DOWN:
                    new_y += 1;
                    break;
                case LEFT:
                    new_x -= 1;
                    break;
                case RIGHT:
                    new_x += 1;
                    break;
                }

                // wrap
                new_x = (new_x + 20) % 20;
                new_y = (new_y + 20) % 20;

                // self collision
                for (int i = 0; i < g->snake_length; i++) {
                    if (g->snake[i].x == new_x && g->snake[i].y == new_y) {
                        for (int i = g->snake_length - 1; i > 0; i--) {
                            g->snake[i] = g->snake[i - 1];
                        }
                        // move head
                        g->snake[0].x = new_x;
                        g->snake[0].y = new_y;
                        g->dead = true;
                        break;
                    }
                }

                if (!g->dead) {
                    // move body
                    for (int i = g->snake_length - 1; i > 0; i--) {
                        g->snake[i] = g->snake[i - 1];
                    }
                    // move head
                    g->snake[0].x = new_x;
                    g->snake[0].y = new_y;
                }
            }
        }
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
