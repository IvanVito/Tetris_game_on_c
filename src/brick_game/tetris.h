#ifndef TETRIS_H
#define TETRIS_H
#define _POSIX_C_SOURCE 199309L  // Announced for time interval measurements

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define ESCAPE 27
#define SPACE 32
#define ENTER '\n'
#define P 'p'
#define FIGURE_ROWS 4
#define FIGURE_COLS 4
#define FIGURES 10
#define RESTART 1
#define NO_RESTART 0

/**
 * @brief This is an array of figure shapes
 */
static const int shapes[FIGURES][FIGURE_ROWS][FIGURE_COLS] = {
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I 0
    {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // O
    {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // S
    {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // Z
    {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // L
    {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // J
    {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // T
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I 90
    {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},  // I 180
    {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}},  // I 270
};

/**
 * @brief Figure structure
 * @param current_shape Pointer to the array with the current figure shape
 * @param x Abscissa axis
 * @param y Ordinate axis
 * @param last_x Previous abscissa axis
 * @param type Figure type from the array of shapes
 * @param next_type Next figure type from the array of shapes
 */
typedef struct {
  int** current_shape;
  int x, y;
  int last_x;
  int type;
  int next_type;
} figure;

/**
 * @brief Field structure
 * @param field Pointer to the array with the game field
 * @param next Pointer to the array with the next figure shape
 * @param score Game score
 * @param high_score Game record
 * @param level Game level
 * @param speed Game speed. Coefficient that reduces the time between ticks
 */
typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
} GameInfo_t;

/**
 * @brief Provides the current figure instance
 * @return Figure copy
 */
figure update_current_figure();

/**
 * @brief Allocates memory for the field with a specified number of rows and
 * columns
 */
int** create_field(int rows, int cols);

/**
 * @brief Deallocates memory for the field previously allocated with a specified
 * number of rows
 */
void clear_field(int*** field, int rows);

/**
 * @brief Fills the passed matrix with a pattern from the array of templates
 * @param matr Fillable matrix
 * @param matr_pattern Pattern from the template array
 */
void picked_matrix(int** matr,
                   const int matr_pattern[FIGURE_ROWS][FIGURE_COLS]);

/**
 * @brief Rotates the current figure 90 degrees clockwise
 */
void transporate_matrix(figure* tetr);

/**
 * @brief Fixes the figure on the field if it cannot fall any further
 */
void fix_figure(GameInfo_t* frame, figure* tetr);

/**
 * @brief Checks collision on the sides
 * @return 1 if collision, otherwise 0"
 */
int collision_with_sides(GameInfo_t* frame, figure* tetr);

/**
 * @brief Checks collision with the bottom (with fixed figures or the field
 * bottom)
 * @return 1 if collision, otherwise 0"
 */
int collision_with_bottom(GameInfo_t* frame, figure* tetr);

/**
 * @brief Checking for figure fixation possibility
 * @return 1 if attach, otherwise 0"
 */
int attaching(GameInfo_t* frame, figure* tetr);

/**
 * @brief Moves the figure.Called if the movement is possible
 */
void move_figure(GameInfo_t* frame, figure* tetr);

/**
 * @brief "Responsible for updating the field. Removes filled rows and changes
 * the score, maximum score, and game speed
 */
void delete_row(GameInfo_t* frame);

/**
 * @brief Replaces the current row with the row above
 */
void pull_rows(GameInfo_t* frame, int before_row);

/**
 * @brief Writes the record to a file.Creates a file if it doesn't exist and
 * writes 0 to it
 * @param record Record score
 * @return Error. (1 - error exists, 0 - no error)
 */
int record(int record);

/**
 * @brief Reading game record from file.Creates a file if it doesn't exist and
 * writes 0 to it. After read it
 * @return Error. (1 - error exists, 0 - no error)
 */
int read_record(GameInfo_t* frame);

/**
 * @brief Deletes the previous figure from the field
 */
void clear_past_state(GameInfo_t* frame, figure* tetr);

/**
 * @brief Checks if the game is over. The game ends when the figure cannot move
 * and y coordinate is negative
 * @return 1 - if the game is over, else 0
 */
int game_end(figure* tetr);

/**
 * @brief Fills the selected area of the matrix with zeros
 * @param sector Pointer to matrix
 * @param rows Row up to which the matrix values are set to zero (exclusive)
 * @param cols Columns up to which the matrix values are set to zero (exclusive)
 */
void clear_sector(int** sector, int rows, int cols);

/**
 * @brief Checks for figure rotation possibility. If not possible, reverts
 * changes
 */
void rotate(GameInfo_t* frame, figure* tetr);

/**
 * @brief Sets initial figure coordinates, next and current shape
 */
void init(GameInfo_t* frame, figure* tetr);

/**
 * @brief Moves the figure downwards after a set time interval
 * @param last_fall Last figure drop time
 */
void shift(GameInfo_t* frame, figure* tetr, struct timespec* last_fall);

/**
 * @brief Function updates the field state and returns its copy
 */
GameInfo_t updateCurrentState();

/**
 * @brief Moving the figure down
 */
void down(figure* tetr);

/**
 * @brief Moving the figure right
 */
void right(figure* tetr);

/**
 * @brief Moving the figure left
 */
void left(figure* tetr);

/**
 * @brief Stub. Not in use
 */
void up(figure* tetr);

/**
 * @brief This function processes the score, records the high score, and
 * increases the level
 * @param lines number of filled rows for which points are awarded
 * @param restart game restart flag
 */
void score(GameInfo_t* frame, int lines, int restart);

#endif  // TETRIS_H
