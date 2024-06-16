#ifndef TETRIS_GUI_H
#define TETRIS_GUI_H

#include <ncurses.h>

#include "../../brick_game/tetris.h"

#define MAP_ROWS 20
#define MAP_COLS 10
#define BOARD_ROWS 12
#define BOARD_COLS 12
#define PADDING 1

/**
 * @brief Field rendering. Space if the array cell contains zero and zero, if
 * the cell contains a number greater than zero
 */
void print_field(GameInfo_t* frame);

/**
 * @brief Function for rendering current score, highest score, level, and next
 * figure
 */
void print_stats(GameInfo_t* stats, figure* tetr);

/**
 * @brief Draws a frame at the specified coordinates
 */
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);

/**
 * @brief Draws menu frames and text
 */
void menu();

/**
 * @brief Prints start text
 */
void print_start();

/**
 * @brief Draws word "PAUSE"
 */
void print_pause();

/**
 * @brief Print game field, set up terminal
 */
void init_front(GameInfo_t* frame);

/**
 * @brief Prints game_over text
 */
void print_game_over();

/**
 * @brief Updating game field, score, level, and figure position
 */
void refresh_scr(GameInfo_t* frame, figure* tetr);

#endif  // TETRIS_GUI_H
