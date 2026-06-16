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
void update_timer(struct Game *g, float number);

void load_level(struct Game *g, int level) {

    g->timer_value = 0;
    g->dead = false;
    g->stopped = false;

    if (level == 1) {
        g->movement_interval = 0.3;
        // setup snake itself (10x10 grid)
        g->snake[2] = (struct SnekSegment){.x = 3, .y = 4};
        g->snake[1] = (struct SnekSegment){.x = 4, .y = 4};
        g->snake[0] = (struct SnekSegment){.x = 5, .y = 4};
        g->snake_length = 3;
        g->direction = RIGHT;

        // Smiley face goal pattern
        // Eyes
        g->goal_blocks[0] = (struct Block){.x = 2, .y = 2, .type = YELLOW};
        g->goal_blocks[1] = (struct Block){.x = 7, .y = 2, .type = YELLOW};
        // Smile
        g->goal_blocks[2] = (struct Block){.x = 2, .y = 6, .type = YELLOW};
        g->goal_blocks[3] = (struct Block){.x = 3, .y = 7, .type = YELLOW};
        g->goal_blocks[4] = (struct Block){.x = 4, .y = 7, .type = YELLOW};
        g->goal_blocks[5] = (struct Block){.x = 5, .y = 7, .type = YELLOW};
        g->goal_blocks[6] = (struct Block){.x = 6, .y = 7, .type = YELLOW};
        g->goal_blocks[7] = (struct Block){.x = 7, .y = 6, .type = YELLOW};

        // Scattered face pieces (more random, including edges)
        g->current_blocks[0] = (struct Block){.x = 0, .y = 1, .type = YELLOW};
        g->current_blocks[1] = (struct Block){.x = 9, .y = 3, .type = YELLOW};
        g->current_blocks[2] = (struct Block){.x = 1, .y = 8, .type = YELLOW};
        g->current_blocks[3] = (struct Block){.x = 8, .y = 0, .type = YELLOW};
        g->current_blocks[4] = (struct Block){.x = 4, .y = 9, .type = YELLOW};
        g->current_blocks[5] = (struct Block){.x = 6, .y = 3, .type = YELLOW};
        g->current_blocks[6] = (struct Block){.x = 2, .y = 5, .type = YELLOW};
        g->current_blocks[7] = (struct Block){.x = 7, .y = 7, .type = YELLOW};

        g->cell_size = 64;
        g->num_of_blocks = 8;
    } else if (level == 2) {
        g->movement_interval = 0.25;
        // setup snake itself (20x20 grid)
        g->snake[3] = (struct SnekSegment){.x = 8, .y = 10};
        g->snake[2] = (struct SnekSegment){.x = 9, .y = 10};
        g->snake[1] = (struct SnekSegment){.x = 10, .y = 10};
        g->snake[0] = (struct SnekSegment){.x = 11, .y = 10};
        g->direction = RIGHT;
        g->snake_length = 4;

        // Tree pattern: trunk (RED) and leaves (GREEN)
        // Trunk
        g->goal_blocks[0] = (struct Block){.x = 10, .y = 12, .type = RED};
        g->goal_blocks[1] = (struct Block){.x = 10, .y = 13, .type = RED};
        g->goal_blocks[2] = (struct Block){.x = 10, .y = 14, .type = RED};
        // Leaves (triangle shape)
        g->goal_blocks[3] = (struct Block){.x = 10, .y = 10, .type = GREEN};
        g->goal_blocks[4] = (struct Block){.x = 9, .y = 11, .type = GREEN};
        g->goal_blocks[5] = (struct Block){.x = 10, .y = 11, .type = GREEN};
        g->goal_blocks[6] = (struct Block){.x = 11, .y = 11, .type = GREEN};
        g->goal_blocks[7] = (struct Block){.x = 8, .y = 12, .type = GREEN};
        g->goal_blocks[8] = (struct Block){.x = 9, .y = 12, .type = GREEN};
        g->goal_blocks[9] = (struct Block){.x = 11, .y = 12, .type = GREEN};
        g->goal_blocks[10] = (struct Block){.x = 12, .y = 12, .type = GREEN};

        // Scattered starting positions (avoiding edges, 3-16 range)
        g->current_blocks[0] = (struct Block){.x = 4, .y = 4, .type = RED};
        g->current_blocks[1] = (struct Block){.x = 15, .y = 5, .type = GREEN};
        g->current_blocks[2] = (struct Block){.x = 5, .y = 14, .type = RED};
        g->current_blocks[3] = (struct Block){.x = 14, .y = 15, .type = RED};
        g->current_blocks[4] = (struct Block){.x = 10, .y = 3, .type = GREEN};
        g->current_blocks[5] = (struct Block){.x = 6, .y = 16, .type = GREEN};
        g->current_blocks[6] = (struct Block){.x = 13, .y = 14, .type = GREEN};
        g->current_blocks[7] = (struct Block){.x = 4, .y = 10, .type = GREEN};
        g->current_blocks[8] = (struct Block){.x = 15, .y = 10, .type = GREEN};
        g->current_blocks[9] = (struct Block){.x = 8, .y = 6, .type = GREEN};
        g->current_blocks[10] = (struct Block){.x = 12, .y = 7, .type = RED};

        // very nice stuff here
        g->cell_size = 32;
        g->num_of_blocks = 11;
    } else if (level == 3) {
        g->movement_interval = 0.2;
        // setup snake itself (20x22 grid)
        g->snake[4] = (struct SnekSegment){.x = 8, .y = 18};
        g->snake[3] = (struct SnekSegment){.x = 9, .y = 18};
        g->snake[2] = (struct SnekSegment){.x = 10, .y = 18};
        g->snake[1] = (struct SnekSegment){.x = 11, .y = 18};
        g->snake[0] = (struct SnekSegment){.x = 12, .y = 18};
        g->direction = RIGHT;
        g->snake_length = 5;

        // Nested squares pattern with BLUE and PINK
        // Outer ring
        g->goal_blocks[0] = (struct Block){.x = 5, .y = 5, .type = BLUE};
        g->goal_blocks[1] = (struct Block){.x = 10, .y = 5, .type = PINK};
        g->goal_blocks[2] = (struct Block){.x = 15, .y = 5, .type = BLUE};
        g->goal_blocks[3] = (struct Block){.x = 15, .y = 10, .type = PINK};
        g->goal_blocks[4] = (struct Block){.x = 15, .y = 15, .type = BLUE};
        g->goal_blocks[5] = (struct Block){.x = 10, .y = 15, .type = PINK};
        g->goal_blocks[6] = (struct Block){.x = 5, .y = 15, .type = BLUE};
        g->goal_blocks[7] = (struct Block){.x = 5, .y = 10, .type = PINK};
        // Inner square
        g->goal_blocks[8] = (struct Block){.x = 8, .y = 8, .type = BLUE};
        g->goal_blocks[9] = (struct Block){.x = 12, .y = 8, .type = PINK};
        g->goal_blocks[10] = (struct Block){.x = 12, .y = 12, .type = BLUE};
        g->goal_blocks[11] = (struct Block){.x = 8, .y = 12, .type = PINK};

        // Scattered starting positions (avoiding edges, 3-16 x, 3-18 y)
        g->current_blocks[0] = (struct Block){.x = 4, .y = 4, .type = BLUE};
        g->current_blocks[1] = (struct Block){.x = 16, .y = 4, .type = PINK};
        g->current_blocks[2] = (struct Block){.x = 4, .y = 16, .type = BLUE};
        g->current_blocks[3] = (struct Block){.x = 16, .y = 16, .type = PINK};
        g->current_blocks[4] = (struct Block){.x = 6, .y = 6, .type = BLUE};
        g->current_blocks[5] = (struct Block){.x = 14, .y = 6, .type = PINK};
        g->current_blocks[6] = (struct Block){.x = 6, .y = 14, .type = BLUE};
        g->current_blocks[7] = (struct Block){.x = 14, .y = 14, .type = PINK};
        g->current_blocks[8] = (struct Block){.x = 10, .y = 3, .type = BLUE};
        g->current_blocks[9] = (struct Block){.x = 3, .y = 10, .type = PINK};
        g->current_blocks[10] = (struct Block){.x = 16, .y = 10, .type = BLUE};
        g->current_blocks[11] = (struct Block){.x = 10, .y = 17, .type = PINK};

        g->cell_size = 32;
        g->num_of_blocks = 12;
    } else if (level == 4) {
        g->movement_interval = 0.15;
        // setup snake itself (20x22 grid)
        g->snake[5] = (struct SnekSegment){.x = 8, .y = 18};
        g->snake[4] = (struct SnekSegment){.x = 9, .y = 18};
        g->snake[3] = (struct SnekSegment){.x = 10, .y = 18};
        g->snake[2] = (struct SnekSegment){.x = 11, .y = 18};
        g->snake[1] = (struct SnekSegment){.x = 12, .y = 18};
        g->snake[0] = (struct SnekSegment){.x = 13, .y = 18};
        g->direction = UP;
        g->snake_length = 6;

        // Final level: House with sun - all 5 colors
        // Sun (YELLOW)
        g->goal_blocks[0] = (struct Block){.x = 15, .y = 3, .type = YELLOW};
        g->goal_blocks[1] = (struct Block){.x = 16, .y = 3, .type = YELLOW};
        g->goal_blocks[2] = (struct Block){.x = 15, .y = 4, .type = YELLOW};
        g->goal_blocks[3] = (struct Block){.x = 16, .y = 4, .type = YELLOW};
        // Roof (RED)
        g->goal_blocks[4] = (struct Block){.x = 7, .y = 8, .type = RED};
        g->goal_blocks[5] = (struct Block){.x = 8, .y = 7, .type = RED};
        g->goal_blocks[6] = (struct Block){.x = 9, .y = 6, .type = RED};
        g->goal_blocks[7] = (struct Block){.x = 10, .y = 6, .type = RED};
        g->goal_blocks[8] = (struct Block){.x = 11, .y = 7, .type = RED};
        g->goal_blocks[9] = (struct Block){.x = 12, .y = 8, .type = RED};
        // House walls (BLUE)
        g->goal_blocks[10] = (struct Block){.x = 8, .y = 9, .type = BLUE};
        g->goal_blocks[11] = (struct Block){.x = 8, .y = 10, .type = BLUE};
        g->goal_blocks[12] = (struct Block){.x = 8, .y = 11, .type = BLUE};
        g->goal_blocks[13] = (struct Block){.x = 11, .y = 9, .type = BLUE};
        g->goal_blocks[14] = (struct Block){.x = 11, .y = 10, .type = BLUE};
        g->goal_blocks[15] = (struct Block){.x = 11, .y = 11, .type = BLUE};
        // Door (PINK)
        g->goal_blocks[16] = (struct Block){.x = 9, .y = 10, .type = PINK};
        g->goal_blocks[17] = (struct Block){.x = 9, .y = 11, .type = PINK};
        // Grass (GREEN)
        g->goal_blocks[18] = (struct Block){.x = 6, .y = 12, .type = GREEN};
        g->goal_blocks[19] = (struct Block){.x = 8, .y = 12, .type = GREEN};
        g->goal_blocks[20] = (struct Block){.x = 10, .y = 12, .type = GREEN};
        g->goal_blocks[21] = (struct Block){.x = 12, .y = 12, .type = GREEN};
        g->goal_blocks[22] = (struct Block){.x = 14, .y = 12, .type = GREEN};

        // Scattered starting positions (all 5 colors, avoiding edges)
        g->current_blocks[0] = (struct Block){.x = 4, .y = 3, .type = YELLOW};
        g->current_blocks[1] = (struct Block){.x = 14, .y = 8, .type = YELLOW};
        g->current_blocks[2] = (struct Block){.x = 5, .y = 16, .type = YELLOW};
        g->current_blocks[3] = (struct Block){.x = 15, .y = 15, .type = YELLOW};
        g->current_blocks[4] = (struct Block){.x = 3, .y = 6, .type = RED};
        g->current_blocks[5] = (struct Block){.x = 16, .y = 6, .type = RED};
        g->current_blocks[6] = (struct Block){.x = 4, .y = 10, .type = RED};
        g->current_blocks[7] = (struct Block){.x = 15, .y = 10, .type = RED};
        g->current_blocks[8] = (struct Block){.x = 6, .y = 4, .type = RED};
        g->current_blocks[9] = (struct Block){.x = 13, .y = 14, .type = RED};
        g->current_blocks[10] = (struct Block){.x = 3, .y = 11, .type = BLUE};
        g->current_blocks[11] = (struct Block){.x = 16, .y = 11, .type = BLUE};
        g->current_blocks[12] = (struct Block){.x = 6, .y = 8, .type = BLUE};
        g->current_blocks[13] = (struct Block){.x = 13, .y = 8, .type = BLUE};
        g->current_blocks[14] = (struct Block){.x = 5, .y = 14, .type = BLUE};
        g->current_blocks[15] = (struct Block){.x = 14, .y = 5, .type = BLUE};
        g->current_blocks[16] = (struct Block){.x = 7, .y = 5, .type = PINK};
        g->current_blocks[17] = (struct Block){.x = 12, .y = 15, .type = PINK};
        g->current_blocks[18] = (struct Block){.x = 8, .y = 14, .type = GREEN};
        g->current_blocks[19] = (struct Block){.x = 11, .y = 4, .type = GREEN};
        g->current_blocks[20] = (struct Block){.x = 4, .y = 17, .type = GREEN};
        g->current_blocks[21] = (struct Block){.x = 15, .y = 17, .type = GREEN};
        g->current_blocks[22] = (struct Block){.x = 10, .y = 8, .type = GREEN};

        g->cell_size = 32;
        g->num_of_blocks = 23;
    } else if (level == 5) {
        g->movement_interval = 0.12;
        // setup snake itself (20x22 grid)
        g->snake[6] = (struct SnekSegment){.x = 7, .y = 18};
        g->snake[5] = (struct SnekSegment){.x = 8, .y = 18};
        g->snake[4] = (struct SnekSegment){.x = 9, .y = 18};
        g->snake[3] = (struct SnekSegment){.x = 10, .y = 18};
        g->snake[2] = (struct SnekSegment){.x = 11, .y = 18};
        g->snake[1] = (struct SnekSegment){.x = 12, .y = 18};
        g->snake[0] = (struct SnekSegment){.x = 13, .y = 18};
        g->direction = RIGHT;
        g->snake_length = 7;

        // Diamond/Star pattern - all 5 colors
        // Center (YELLOW)
        g->goal_blocks[0] = (struct Block){.x = 10, .y = 10, .type = YELLOW};
        // Top point (RED)
        g->goal_blocks[1] = (struct Block){.x = 10, .y = 5, .type = RED};
        g->goal_blocks[2] = (struct Block){.x = 9, .y = 6, .type = RED};
        g->goal_blocks[3] = (struct Block){.x = 11, .y = 6, .type = RED};
        // Right point (GREEN)
        g->goal_blocks[4] = (struct Block){.x = 15, .y = 10, .type = GREEN};
        g->goal_blocks[5] = (struct Block){.x = 14, .y = 9, .type = GREEN};
        g->goal_blocks[6] = (struct Block){.x = 14, .y = 11, .type = GREEN};
        // Bottom point (BLUE)
        g->goal_blocks[7] = (struct Block){.x = 10, .y = 15, .type = BLUE};
        g->goal_blocks[8] = (struct Block){.x = 9, .y = 14, .type = BLUE};
        g->goal_blocks[9] = (struct Block){.x = 11, .y = 14, .type = BLUE};
        // Left point (PINK)
        g->goal_blocks[10] = (struct Block){.x = 5, .y = 10, .type = PINK};
        g->goal_blocks[11] = (struct Block){.x = 6, .y = 9, .type = PINK};
        g->goal_blocks[12] = (struct Block){.x = 6, .y = 11, .type = PINK};
        // Inner ring (mixed colors)
        g->goal_blocks[13] = (struct Block){.x = 8, .y = 8, .type = YELLOW};
        g->goal_blocks[14] = (struct Block){.x = 12, .y = 8, .type = RED};
        g->goal_blocks[15] = (struct Block){.x = 12, .y = 12, .type = GREEN};
        g->goal_blocks[16] = (struct Block){.x = 8, .y = 12, .type = BLUE};

        // Scattered starting positions (all 5 colors)
        g->current_blocks[0] = (struct Block){.x = 3, .y = 3, .type = RED};
        g->current_blocks[1] = (struct Block){.x = 17, .y = 3, .type = RED};
        g->current_blocks[2] = (struct Block){.x = 4, .y = 18, .type = RED};
        g->current_blocks[3] = (struct Block){.x = 16, .y = 17, .type = RED};
        g->current_blocks[4] = (struct Block){.x = 2, .y = 8, .type = GREEN};
        g->current_blocks[5] = (struct Block){.x = 18, .y = 12, .type = GREEN};
        g->current_blocks[6] = (struct Block){.x = 5, .y = 5, .type = GREEN};
        g->current_blocks[7] = (struct Block){.x = 15, .y = 15, .type = GREEN};
        g->current_blocks[8] = (struct Block){.x = 7, .y = 3, .type = BLUE};
        g->current_blocks[9] = (struct Block){.x = 13, .y = 4, .type = BLUE};
        g->current_blocks[10] = (struct Block){.x = 3, .y = 13, .type = BLUE};
        g->current_blocks[11] = (struct Block){.x = 17, .y = 14, .type = BLUE};
        g->current_blocks[12] = (struct Block){.x = 8, .y = 17, .type = PINK};
        g->current_blocks[13] = (struct Block){.x = 12, .y = 3, .type = PINK};
        g->current_blocks[14] = (struct Block){.x = 18, .y = 7, .type = PINK};
        g->current_blocks[15] = (struct Block){.x = 2, .y = 16, .type = YELLOW};
        g->current_blocks[16] = (struct Block){.x = 17, .y = 8, .type = YELLOW};

        g->cell_size = 32;
        g->num_of_blocks = 17;
    } else if (level == 6) {
        g->movement_interval = 0.1;
        // setup snake itself (20x22 grid)
        g->snake[7] = (struct SnekSegment){.x = 7, .y = 19};
        g->snake[6] = (struct SnekSegment){.x = 8, .y = 19};
        g->snake[5] = (struct SnekSegment){.x = 9, .y = 19};
        g->snake[4] = (struct SnekSegment){.x = 10, .y = 19};
        g->snake[3] = (struct SnekSegment){.x = 11, .y = 19};
        g->snake[2] = (struct SnekSegment){.x = 12, .y = 19};
        g->snake[1] = (struct SnekSegment){.x = 13, .y = 19};
        g->snake[0] = (struct SnekSegment){.x = 14, .y = 19};
        g->direction = UP;
        g->snake_length = 8;

        // Complex checkerboard pattern with letters/shapes
        // Top row pattern (YELLOW and RED)
        g->goal_blocks[0] = (struct Block){.x = 3, .y = 3, .type = YELLOW};
        g->goal_blocks[1] = (struct Block){.x = 5, .y = 3, .type = RED};
        g->goal_blocks[2] = (struct Block){.x = 7, .y = 3, .type = YELLOW};
        g->goal_blocks[3] = (struct Block){.x = 9, .y = 3, .type = RED};
        g->goal_blocks[4] = (struct Block){.x = 11, .y = 3, .type = YELLOW};
        g->goal_blocks[5] = (struct Block){.x = 13, .y = 3, .type = RED};
        g->goal_blocks[6] = (struct Block){.x = 15, .y = 3, .type = YELLOW};
        g->goal_blocks[7] = (struct Block){.x = 17, .y = 3, .type = RED};
        // Second row (GREEN and BLUE)
        g->goal_blocks[8] = (struct Block){.x = 3, .y = 6, .type = GREEN};
        g->goal_blocks[9] = (struct Block){.x = 6, .y = 6, .type = BLUE};
        g->goal_blocks[10] = (struct Block){.x = 9, .y = 6, .type = GREEN};
        g->goal_blocks[11] = (struct Block){.x = 12, .y = 6, .type = BLUE};
        g->goal_blocks[12] = (struct Block){.x = 15, .y = 6, .type = GREEN};
        g->goal_blocks[13] = (struct Block){.x = 17, .y = 6, .type = BLUE};
        // Center cross (PINK)
        g->goal_blocks[14] = (struct Block){.x = 10, .y = 9, .type = PINK};
        g->goal_blocks[15] = (struct Block){.x = 9, .y = 10, .type = PINK};
        g->goal_blocks[16] = (struct Block){.x = 10, .y = 10, .type = PINK};
        g->goal_blocks[17] = (struct Block){.x = 11, .y = 10, .type = PINK};
        g->goal_blocks[18] = (struct Block){.x = 10, .y = 11, .type = PINK};
        // Third row (YELLOW and RED)
        g->goal_blocks[19] = (struct Block){.x = 4, .y = 14, .type = YELLOW};
        g->goal_blocks[20] = (struct Block){.x = 7, .y = 14, .type = RED};
        g->goal_blocks[21] = (struct Block){.x = 10, .y = 14, .type = YELLOW};
        g->goal_blocks[22] = (struct Block){.x = 13, .y = 14, .type = RED};
        g->goal_blocks[23] = (struct Block){.x = 16, .y = 14, .type = YELLOW};
        // Bottom row (GREEN and BLUE)
        g->goal_blocks[24] = (struct Block){.x = 3, .y = 17, .type = GREEN};
        g->goal_blocks[25] = (struct Block){.x = 6, .y = 17, .type = BLUE};
        g->goal_blocks[26] = (struct Block){.x = 9, .y = 17, .type = GREEN};
        g->goal_blocks[27] = (struct Block){.x = 12, .y = 17, .type = BLUE};
        g->goal_blocks[28] = (struct Block){.x = 15, .y = 17, .type = GREEN};
        g->goal_blocks[29] = (struct Block){.x = 17, .y = 17, .type = BLUE};

        // Heavily scattered starting positions (all 5 colors, maximum chaos)
        g->current_blocks[0] = (struct Block){.x = 1, .y = 1, .type = YELLOW};
        g->current_blocks[1] = (struct Block){.x = 19, .y = 1, .type = RED};
        g->current_blocks[2] = (struct Block){.x = 0, .y = 5, .type = GREEN};
        g->current_blocks[3] = (struct Block){.x = 18, .y = 4, .type = BLUE};
        g->current_blocks[4] = (struct Block){.x = 2, .y = 9, .type = PINK};
        g->current_blocks[5] = (struct Block){.x = 19, .y = 8, .type = YELLOW};
        g->current_blocks[6] = (struct Block){.x = 1, .y = 12, .type = RED};
        g->current_blocks[7] = (struct Block){.x = 18, .y = 15, .type = GREEN};
        g->current_blocks[8] = (struct Block){.x = 0, .y = 18, .type = BLUE};
        g->current_blocks[9] = (struct Block){.x = 19, .y = 20, .type = PINK};
        g->current_blocks[10] = (struct Block){.x = 5, .y = 0, .type = YELLOW};
        g->current_blocks[11] = (struct Block){.x = 14, .y = 2, .type = RED};
        g->current_blocks[12] = (struct Block){.x = 8, .y = 21, .type = GREEN};
        g->current_blocks[13] = (struct Block){.x = 11, .y = 20, .type = BLUE};
        g->current_blocks[14] = (struct Block){.x = 4, .y = 8, .type = PINK};
        g->current_blocks[15] = (struct Block){.x = 16, .y = 12, .type = YELLOW};
        g->current_blocks[16] = (struct Block){.x = 7, .y = 7, .type = RED};
        g->current_blocks[17] = (struct Block){.x = 13, .y = 13, .type = GREEN};
        g->current_blocks[18] = (struct Block){.x = 2, .y = 20, .type = BLUE};
        g->current_blocks[19] = (struct Block){.x = 17, .y = 21, .type = PINK};
        g->current_blocks[20] = (struct Block){.x = 10, .y = 1, .type = YELLOW};
        g->current_blocks[21] = (struct Block){.x = 5, .y = 11, .type = RED};
        g->current_blocks[22] = (struct Block){.x = 15, .y = 9, .type = GREEN};
        g->current_blocks[23] = (struct Block){.x = 3, .y = 15, .type = BLUE};
        g->current_blocks[24] = (struct Block){.x = 12, .y = 5, .type = PINK};
        g->current_blocks[25] = (struct Block){.x = 8, .y = 13, .type = YELLOW};
        g->current_blocks[26] = (struct Block){.x = 14, .y = 18, .type = RED};
        g->current_blocks[27] = (struct Block){.x = 6, .y = 19, .type = GREEN};
        g->current_blocks[28] = (struct Block){.x = 11, .y = 7, .type = BLUE};
        g->current_blocks[29] = (struct Block){.x = 9, .y = 16, .type = PINK};

        g->cell_size = 32;
        g->num_of_blocks = 30;
    }
}
void go_to_loss_screen(struct Game *g) {
    g->dead = true;
    g->death_scene = g->scene;
    g->scene = -1;
    // bake the exact final time into the timer texture
    update_timer(g, g->timer_value);
}

void push(struct Game *g, int number_of_columns, int number_of_rows) {

    if (g->stopped && !g->dead) {

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
        new_x = (new_x + number_of_columns) % number_of_columns;
        new_y = (new_y + number_of_rows) % number_of_rows;

        // self collision
        for (int i = 0; i < g->snake_length; i++) {
            if (g->snake[i].x == new_x && g->snake[i].y == new_y) {
                go_to_loss_screen(g);
                break;
            }
        }

        if (!g->dead) {

            // find the block directly in front of the head

            int front_block = -1;
            for (int i = 0; i < g->num_of_blocks; i++) {
                if (g->current_blocks[i].x == new_x &&
                    g->current_blocks[i].y == new_y) {
                    front_block = i;
                    break;
                }
            }

            if (front_block == -1) {
                // nothing in front
                g->stopped = false;
                return;
            }

            int dx = 0, dy = 0;
            switch (g->direction) {
            case UP:
                dy = -1;
                break;
            case DOWN:
                dy = 1;
                break;
            case LEFT:
                dx = -1;
                break;
            case RIGHT:
                dx = 1;
                break;
            }

            // chain
            int chain_indices[100];
            int chain_len = 1;
            chain_indices[0] = front_block;
            int check_x = new_x;
            int check_y = new_y;

            while (chain_len < 100) {
                check_x =
                    (check_x + dx + number_of_columns) % number_of_columns;

                check_y = (check_y + dy + number_of_rows) % number_of_rows;
                bool found = false;
                for (int j = 0; j < g->num_of_blocks; j++) {
                    if (g->current_blocks[j].x == check_x &&
                        g->current_blocks[j].y == check_y) {
                        chain_indices[chain_len] = j;
                        chain_len += 1;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    break;
            }

            bool can_push = true;
            for (int j = 0; j < g->snake_length; j++) {
                if (g->snake[j].x == check_x && g->snake[j].y == check_y) {
                    can_push = false;
                    break;
                }
            }

            if (can_push) {
                // Push all blocks in chain (from back to front)
                for (int j = chain_len - 1; j >= 0; j--) {
                    int idx = chain_indices[j];
                    g->current_blocks[idx].x =
                        (g->current_blocks[idx].x + dx + number_of_columns) %
                        number_of_columns;
                    g->current_blocks[idx].y =
                        (g->current_blocks[idx].y + dy + number_of_rows) %
                        number_of_rows;
                }

                // move snake, no need to set stopped to false cuz ur still in
                // front of it.
                for (int j = g->snake_length - 1; j > 0; j--) {
                    g->snake[j] = g->snake[j - 1];
                }
                g->snake[0].x = new_x;
                g->snake[0].y = new_y;

                int correct = 0;
                for (int i = 0; i < g->num_of_blocks; i++) {
                    for (int j = 0; j < g->num_of_blocks; j++) {
                        if (g->goal_blocks[i].x == g->current_blocks[j].x &&
                            g->goal_blocks[i].y == g->current_blocks[j].y) {
                            correct++;
                        }
                    }
                }
                if (correct == g->num_of_blocks) {
                    g->scene++;
                    load_level(g, g->scene);
                }
            }
        }
    }
}

void set_color(struct Game *g, enum BlockType type) {
    switch (type) {
    case GREEN:
        SDL_SetRenderDrawColor(g->renderer, 0, 255, 0, 255);
        break;
    case RED:
        SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255);
        break;
    case BLUE:
        SDL_SetRenderDrawColor(g->renderer, 0, 0, 255, 255);
        break;
    case PINK:
        SDL_SetRenderDrawColor(g->renderer, 255, 105, 180, 255);
        break;
    case YELLOW:
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 0, 255);
        break;
    default:
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
        break;
    }
}

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
            if (g->event.key.repeat)
                break;

            // esc pauses, esc again resumes
            if (g->event.key.scancode == SDL_SCANCODE_ESCAPE) {
                if (g->scene >= 1) {
                    g->death_scene = g->scene;
                    g->scene = -2;
                    // bake the current time into the timer texture
                    update_timer(g, g->timer_value);
                } else if (g->scene == -2) {
                    g->scene = g->death_scene;
                }
                break;
            }

            // level picker on home screen
            if (g->scene == 0) {
                switch (g->event.key.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    g->selected_level -= 1;
                    if (g->selected_level < 1)
                        g->selected_level = 6;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    g->selected_level += 1;
                    if (g->selected_level > 6)
                        g->selected_level = 1;
                    break;
                default:
                    break;
                }
                break;
            }

            // only steer while actually playing
            if (g->scene < 1)
                break;

            enum Direction new_direction = g->direction;
            bool is_direction_key = true;
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
                is_direction_key = false;
                break;
            }
            // reject 180 turns
            bool opposite = (new_direction == UP && g->direction == DOWN) ||
                            (new_direction == DOWN && g->direction == UP) ||
                            (new_direction == LEFT && g->direction == RIGHT) ||
                            (new_direction == RIGHT && g->direction == LEFT);
            if (!opposite) {
                g->direction = new_direction;
            }
            // only push for an accepted direction key, otherwise a rejected
            // 180 (or any random key) would shove in the old direction
            if (g->stopped && is_direction_key && !opposite) {
                push(g, 640 / g->cell_size, 640 / g->cell_size);
            }
        } break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            // jonathan blow
            if (g->event.button.button == 1) { // left click
                float mosx;
                float mosy;
                get_mouse_pos(g, &mosx, &mosy);

                if (g->scene == 0) {
                    float xpos_play = 700;
                    float ypos_play = 500;

                    if (mosx >= xpos_play && mosx <= xpos_play + 180 &&
                        mosy >= ypos_play && mosy <= ypos_play + 60) {
                        g->scene = g->selected_level;
                        load_level(g, g->selected_level);
                    }
                } else if (g->scene < 0) {
                    // loss and pause buttons, keep in sync with game_draw
                    float ypos_buttons = 480;

                    if (mosx >= WINDOW_WIDTH / 2 - 220 &&
                        mosx <= WINDOW_WIDTH / 2 - 20 && mosy >= ypos_buttons &&
                        mosy <= ypos_buttons + 70) {
                        g->scene = 0; // home
                    } else if (mosx >= WINDOW_WIDTH / 2 + 20 &&
                               mosx <= WINDOW_WIDTH / 2 + 220 &&
                               mosy >= ypos_buttons &&
                               mosy <= ypos_buttons + 70) {
                        g->scene = g->death_scene; // retry
                        load_level(g, g->death_scene);
                    }
                }
            }
            break;

        default:
            break;
        }
    }
}

void game_draw(struct Game *g) {
    int grid_y_start = 80;
    int grid_width = 640;
    int grid_height = 640;
    int top_area = 80;
    int number_of_columns = grid_width / g->cell_size; // 10
    int number_of_rows = grid_height / g->cell_size;   // 10

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

        // selected level under the play button, arrows to change it
        if (g->last_cached_level_number != g->selected_level) {
            update_level_text(g, g->selected_level);
        }
        float tw = g->level_number_surface->w / 8.0f;
        float th = g->level_number_surface->h / 8.0f;
        SDL_FRect picked_rect = {.x = xpos_play + 90 - (tw / 2),
                                 .y = ypos_play + 75,
                                 .w = tw,
                                 .h = th};
        SDL_RenderTexture(g->renderer, g->level_number_texture, NULL,
                          &picked_rect);
    } else if (g->scene >= 1) {

        // check win condition

        // draw top line
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
        SDL_FRect top_line_rect = {.x = 0, .y = 76, .w = WINDOW_WIDTH, .h = 4};
        SDL_RenderFillRect(g->renderer, &top_line_rect);

        // TODO!! Update title screen

        // draw middle line
        int width_of_line = 4;
        SDL_FRect dividing_line_rect = {.x = WINDOW_WIDTH / 2 -
                                             (width_of_line / 2),
                                        .y = 0,
                                        .w = width_of_line,
                                        .h = WINDOW_HEIGHT};
        SDL_RenderFillRect(g->renderer, &dividing_line_rect);

        //
        // draw blocks now

        // goal blocks

        for (int i = 0; i < g->num_of_blocks; i++) {
            int x = g->goal_blocks[i].x;
            int y = g->goal_blocks[i].y;

            set_color(g, g->goal_blocks[i].type);

            SDL_FRect block_rect = {.x = x * g->cell_size,
                                    .y = y * g->cell_size + grid_y_start,
                                    .w = g->cell_size,
                                    .h = g->cell_size};

            SDL_RenderFillRect(g->renderer, &block_rect);
        }

        for (int i = 0; i < g->num_of_blocks; i++) {
            int x = g->current_blocks[i].x;
            int y = g->current_blocks[i].y;

            set_color(g, g->current_blocks[i].type);

            SDL_FRect block_rect = {.x = x * g->cell_size + grid_width + 2,
                                    .y = y * g->cell_size + grid_y_start,
                                    .w = g->cell_size,
                                    .h = g->cell_size};

            SDL_RenderFillRect(g->renderer, &block_rect);
        }

        if (!g->dead && !g->stopped) {
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
                new_x = (new_x + number_of_columns) % number_of_columns;
                new_y = (new_y + number_of_rows) % number_of_rows;

                // self collision
                for (int i = 0; i < g->snake_length; i++) {
                    if (g->snake[i].x == new_x && g->snake[i].y == new_y) {
                        go_to_loss_screen(g);
                        break;
                    }
                }

                if (!g->dead) {

                    // check for block collision for movement logic

                    for (int i = 0; i < g->num_of_blocks; i++) {
                        int x = g->current_blocks[i].x;
                        int y = g->current_blocks[i].y;

                        if (new_x == x && new_y == y) {
                            g->stopped = true;
                            g->movement_timer =
                                g->movement_interval; // insta move on move
                        }
                    }

                    if (!g->stopped) {

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

        // draw the snake, head is red, body green

        SDL_SetRenderDrawColor(g->renderer, 255, 0, 0, 255); // red head
                                                             //
        SDL_FRect head_rect = {.x = (g->snake[0].x * g->cell_size) + grid_width,
                               .y = (g->snake[0].y * g->cell_size) + top_area,
                               .w = g->cell_size + 2,
                               .h = g->cell_size + 2};
        SDL_RenderFillRect(g->renderer, &head_rect);

        if (!g->dead) {
            SDL_SetRenderDrawColor(g->renderer, 0, 255, 0,
                                   255); // green body
        } else {
            SDL_SetRenderDrawColor(g->renderer, 0, 130, 0,
                                   255); // green body
        }

        for (int i = 1; i < g->snake_length; i++) {

            SDL_FRect body_rect = {
                .x = (g->snake[i].x * g->cell_size) + grid_width,
                .y = (g->snake[i].y * g->cell_size) + top_area,
                .w = g->cell_size + 2,
                .h = g->cell_size + 2};
            SDL_RenderFillRect(g->renderer, &body_rect);
        }

        SDL_SetRenderDrawColor(g->renderer, 70, 70, 70, 255);

        // columns
        int width_of_grid_line = 2;
        for (int i = 0; i < number_of_columns;
             i++) { // not <= cuz we already draw the middle line
            SDL_FRect line = {.x = i * g->cell_size,
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
                              .y = i * g->cell_size + grid_y_start + pad,
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
            SDL_FRect line = {.x = i * g->cell_size + grid_width + pad,
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
                              .y = i * g->cell_size + grid_y_start + pad,
                              .w = grid_width,
                              .h = width_of_grid_line};
            SDL_RenderFillRect(g->renderer, &line);
        }

        // draw goal block outlines on right grid
        for (int i = 0; i < g->num_of_blocks; i++) {
            int x = g->goal_blocks[i].x;
            int y = g->goal_blocks[i].y;

            set_color(g, g->goal_blocks[i].type);

            // top line
            SDL_FRect top = {.x = x * g->cell_size + grid_width + 2,
                             .y = y * g->cell_size + grid_y_start,
                             .w = g->cell_size,
                             .h = width_of_grid_line};
            SDL_RenderFillRect(g->renderer, &top);

            // bottom line
            SDL_FRect bottom = {.x = x * g->cell_size + grid_width + 2,
                                .y = (y + 1) * g->cell_size + grid_y_start -
                                     width_of_grid_line + 2,
                                .w = g->cell_size,
                                .h = width_of_grid_line};
            SDL_RenderFillRect(g->renderer, &bottom);

            // left line
            SDL_FRect left = {.x = x * g->cell_size + grid_width + 1,
                              .y = y * g->cell_size + grid_y_start,
                              .w = width_of_grid_line,
                              .h = g->cell_size + 2};
            SDL_RenderFillRect(g->renderer, &left);

            // right line
            SDL_FRect right = {.x = (x + 1) * g->cell_size + grid_width + 2 -
                                    width_of_grid_line,
                               .y = y * g->cell_size + grid_y_start,
                               .w = width_of_grid_line,
                               .h = g->cell_size};
            SDL_RenderFillRect(g->renderer, &right);
        }

        SDL_SetRenderDrawColor(g->renderer, 70, 70, 70, 255);

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

        // level text follows whatever scene we are in
        if (g->last_cached_level_number != g->scene) {
            update_level_text(g, g->scene);
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
    } else if (g->scene < 0) {

        // loss screen and pause menu share this layout

        float mosx;
        float mosy;
        get_mouse_pos(g, &mosx, &mosy);

        // big title, you died or paused
        SDL_Surface *title_surface =
            g->scene == -1 ? g->death_surface : g->pause_surface;
        SDL_Texture *title_texture =
            g->scene == -1 ? g->death_texture : g->pause_texture;
        float tw = title_surface->w / 3.0f;
        float th = title_surface->h / 3.0f;
        SDL_FRect died_rect = {
            .x = WINDOW_WIDTH / 2 - (tw / 2), .y = 130, .w = tw, .h = th};
        SDL_RenderTexture(g->renderer, title_texture, NULL, &died_rect);

        // time, texture got baked on death or pause
        tw = g->timer_text_surface->w / 5.0f;
        th = g->timer_text_surface->h / 5.0f;
        SDL_FRect time_rect = {
            .x = WINDOW_WIDTH / 2 - (tw / 2), .y = 330, .w = tw, .h = th};
        SDL_RenderTexture(g->renderer, g->timer_text_texture, NULL, &time_rect);

        // buttons, hitboxes live in game_events so keep em matching
        float ypos_buttons = 480;
        SDL_FRect home_rect = {
            .x = WINDOW_WIDTH / 2 - 220, .y = ypos_buttons, .w = 200, .h = 70};
        SDL_FRect retry_rect = {
            .x = WINDOW_WIDTH / 2 + 20, .y = ypos_buttons, .w = 200, .h = 70};

        bool home_hover =
            mosx >= home_rect.x && mosx <= home_rect.x + home_rect.w &&
            mosy >= home_rect.y && mosy <= home_rect.y + home_rect.h;
        bool retry_hover =
            mosx >= retry_rect.x && mosx <= retry_rect.x + retry_rect.w &&
            mosy >= retry_rect.y && mosy <= retry_rect.y + retry_rect.h;

        if (home_hover) {
            SDL_SetRenderDrawColor(g->renderer, 90, 90, 90, 255);
        } else {
            SDL_SetRenderDrawColor(g->renderer, 45, 45, 45, 255);
        }
        SDL_RenderFillRect(g->renderer, &home_rect);

        if (retry_hover) {
            SDL_SetRenderDrawColor(g->renderer, 90, 90, 90, 255);
        } else {
            SDL_SetRenderDrawColor(g->renderer, 45, 45, 45, 255);
        }
        SDL_RenderFillRect(g->renderer, &retry_rect);

        // button outlines
        SDL_SetRenderDrawColor(g->renderer, 255, 255, 255, 255);
        SDL_RenderRect(g->renderer, &home_rect);
        SDL_RenderRect(g->renderer, &retry_rect);

        // button labels, centered in their boxes
        tw = g->home_surface->w / 8.0f;
        th = g->home_surface->h / 8.0f;
        SDL_FRect home_text_rect = {
            .x = home_rect.x + (home_rect.w / 2) - (tw / 2),
            .y = home_rect.y + (home_rect.h / 2) - (th / 2),
            .w = tw,
            .h = th};
        SDL_RenderTexture(g->renderer, g->home_texture, NULL, &home_text_rect);

        tw = g->retry_surface->w / 8.0f;
        th = g->retry_surface->h / 8.0f;
        SDL_FRect retry_text_rect = {
            .x = retry_rect.x + (retry_rect.w / 2) - (tw / 2),
            .y = retry_rect.y + (retry_rect.h / 2) - (th / 2),
            .w = tw,
            .h = th};
        SDL_RenderTexture(g->renderer, g->retry_texture, NULL,
                          &retry_text_rect);
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
