#include "tetris.h"

#include <time.h>

#include "../gui/cli/fsm.h"
#include "../gui/cli/tetris_gui.h"

figure update_current_figure() {
  static figure tetr;
  if (tetr.current_shape == NULL)
    tetr.current_shape = create_field(FIGURE_ROWS, FIGURE_COLS);
  return tetr;
}

int** create_field(int rows, int cols) {
  int** field = (int**)calloc(rows, sizeof(int*));
  for (int i = 0; i < rows; ++i) field[i] = (int*)calloc(cols, sizeof(int));
  return field;
}

void clear_field(int*** field, int rows) {
  if (*field != NULL && **field != NULL) {
    for (int i = 0; i < rows; i++) {
      free((*field)[i]);
      (*field)[i] = NULL;
    }
    free(*field);
    *field = NULL;
  }
}

void picked_matrix(int** matr,
                   const int matr_pattern[FIGURE_ROWS][FIGURE_COLS]) {
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++)
      matr[row][col] = matr_pattern[row][col];
  }
}

void transporate_matrix(figure* tetr) {
  int temp[3][3];
  int rows = 3,
      cols = 3;  // except for I, only 3 rows need to be flipped for the others
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++)
      temp[col][(rows - 1) - row] = tetr->current_shape[row][col];
  }
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++)
      tetr->current_shape[row][col] = temp[row][col];
  }
}

void fix_figure(GameInfo_t* frame, figure* tetr) {
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++)
      if (tetr->current_shape[row][col]) {
        int new_x = tetr->x + col;
        int new_y = tetr->y + row;
        if (new_y >= 0 && new_y < MAP_ROWS && new_x >= 0 && new_x < MAP_COLS)
          frame->field[new_y][new_x] = 2;
      }
  }
}

int collision_with_sides(GameInfo_t* frame, figure* tetr) {
  int collision = 0;
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      if (tetr->current_shape[row][col]) {
        int new_x = tetr->x + col;
        int new_y = tetr->y + row;
        if (new_x < 0 || new_x >= MAP_COLS ||
            (new_y >= 0 && new_y < MAP_ROWS && frame->field[new_y][new_x] == 2))
          collision = 1;
      }
    }
  }
  return collision;
}

int collision_with_bottom(GameInfo_t* frame, figure* tetr) {
  int collision = 0;
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      if (tetr->current_shape[row][col]) {
        int new_x = tetr->x + col;
        int new_y = tetr->y + row;
        if (new_y >= MAP_ROWS ||
            (new_y >= 0 && new_x >= 0 && new_x < MAP_COLS &&
             frame->field[new_y][new_x] == 2))
          collision = 1;
      }
    }
  }
  return collision;
}

int attaching(GameInfo_t* frame, figure* tetr) {
  int attaching = 0;
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      if (tetr->current_shape[row][col]) {
        int new_x = tetr->x + col;
        int new_y = tetr->y + row;
        if (new_y + 1 >= MAP_ROWS ||
            (new_y + 1 >= 0 && new_x >= 0 && new_x < MAP_COLS &&
             frame->field[new_y + 1][new_x] == 2))
          attaching = 1;
      }
    }
  }
  return attaching;
}

void move_figure(GameInfo_t* frame, figure* tetr) {
  clear_past_state(frame, tetr);
  if (collision_with_sides(frame, tetr) == 0)
    tetr->last_x = tetr->x;
  else
    tetr->x = tetr->last_x;
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      if (tetr->current_shape[row][col]) {
        int new_y = tetr->y + row;
        int new_x = tetr->x + col;
        if (new_y >= 0 && new_y < MAP_ROWS && new_x >= 0 && new_x < MAP_COLS &&
            frame->field[new_y][new_x] != 2) {
          frame->field[new_y][new_x] = 1;
        }
      }
    }
  }
}

void delete_row(GameInfo_t* frame) {
  int lines = 0;
  for (int row = MAP_ROWS - 1; row >= 0; row--) {
    int count = 0;
    for (int col = 0; col < MAP_COLS; ++col)
      if (frame->field[row][col] == 2) count++;
    if (count == MAP_COLS) {
      pull_rows(frame, row);
      row++;
      lines++;
    }
  }
  score(frame, lines, NO_RESTART);
}

void score(GameInfo_t* frame, int lines, int restart) {
  static int score = 0;
  if (restart == 1) {
    score = 0;
  }
  int const score_points[4] = {100, 300, 700, 1500};
  if (lines > 0) score += score_points[lines - 1];
  frame->score = score;
  if (frame->level < 10) frame->level = (frame->score / 600) + 1;
  if (frame->score > frame->high_score) record(frame->score);
  frame->speed = frame->level;
}

void pull_rows(GameInfo_t* frame, int before_row) {
  for (int row = before_row; row > 0; --row)
    memmove(frame->field[row], frame->field[row - 1], sizeof(int) * MAP_COLS);
  memset(frame->field[0], 0, sizeof(int) * MAP_COLS);
}

int record(int record) {
  int err = 0;
  FILE* file = fopen("record.txt", "w");
  if (file == NULL) {
    err = 1;
    perror("Ошибка открытия файла");
  } else {
    fprintf(file, "%d", record);
  }
  if (err == 1 || fclose(file) != 0) {
    perror("Ошибка закрытия файла");
    err = 1;
  }
  return err;
}

int read_record(GameInfo_t* frame) {
  FILE* file;
  int err = 0;
  file = fopen("record.txt", "r");
  if (file == NULL) {
    file = fopen("record.txt", "w+");
    if (file != NULL) {
      fprintf(file, "%d", 0);
      fseek(file, 0, SEEK_SET);
    }
    if (file == NULL) {
      perror("Ошибка создания файла");
      err = 1;
    }
  }
  if (fscanf(file, "%d", &(frame->high_score)) != 1) {
    file = freopen("record.txt", "w", file);
    fprintf(file, "%d", 0);
    fseek(file, 0, SEEK_SET);
  } else {
    fscanf(file, "%d", &(frame->high_score));
  }
  if (fclose(file) != 0) {
    perror("Ошибка закрытия файла");
    err = 1;
  }
  return err;
}

void clear_past_state(GameInfo_t* frame, figure* tetr) {
  for (int row = -2; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      int new_x = tetr->last_x + col;
      int new_y = tetr->y + row;
      if (new_y >= 0 && new_y < MAP_ROWS && new_x >= 0 && new_x < MAP_COLS &&
          frame->field[new_y][new_x] != 2)
        frame->field[new_y][new_x] = 0;
    }
  }
}

int game_end(figure* tetr) {
  int over = 0, first_y = -MAP_COLS;
  for (int row = 0; row < FIGURE_ROWS; row++) {
    for (int col = 0; col < FIGURE_COLS; col++) {
      if (tetr->current_shape[row][col])
        if (first_y == -MAP_COLS) first_y = tetr->y + row;
    }
  }
  if (first_y < 0) over = 1;
  return over;
}

void clear_sector(int** sector, int rows, int cols) {
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++) sector[row][col] = 0;
  }
}

void rotate(GameInfo_t* frame, figure* tetr) {
  if (tetr->type == 0) {  // 90 degrees
    tetr->type = 7;
    picked_matrix(tetr->current_shape, shapes[tetr->type]);
    if (collision_with_bottom(frame, tetr) == 1) {
      tetr->type = 0;
      picked_matrix(tetr->current_shape, shapes[tetr->type]);
    }
  } else if (tetr->type == 7) {  // 180 degrees
    tetr->x -= 1;
    tetr->type = 8;
    picked_matrix(tetr->current_shape, shapes[tetr->type]);
    if (collision_with_sides(frame, tetr) == 1) {
      tetr->x += 1;
      tetr->type = 7;
      picked_matrix(tetr->current_shape, shapes[tetr->type]);
    }
  } else if (tetr->type == 8) {  // 270 degrees
    tetr->x += 1;
    tetr->y -= 1;
    tetr->type = 9;
    picked_matrix(tetr->current_shape, shapes[tetr->type]);
    if (collision_with_sides(frame, tetr) == 1 ||
        collision_with_bottom(frame, tetr) == 1) {
      tetr->x -= 1;
      tetr->y += 1;
      tetr->type = 8;
      picked_matrix(tetr->current_shape, shapes[tetr->type]);
    }
  } else if (tetr->type == 9) {  // 360 degrees
    tetr->y += 1;
    tetr->type = 0;
    picked_matrix(tetr->current_shape, shapes[tetr->type]);
    if (collision_with_bottom(frame, tetr) == 1 ||
        collision_with_sides(frame, tetr) == 1) {
      tetr->y -= 1;
      tetr->type = 9;
      picked_matrix(tetr->current_shape, shapes[tetr->type]);
    }
  } else if (tetr->type != 1) {
    transporate_matrix(tetr);
    if (collision_with_sides(frame, tetr) == 1 ||
        collision_with_bottom(frame, tetr) == 1) {
      for (int i = 0; i < 3; i++) transporate_matrix(tetr);
    }
  }
}

void init(GameInfo_t* frame, figure* tetr) {
  tetr->x = (MAP_COLS - FIGURE_COLS) / 2;
  tetr->last_x = (MAP_COLS - FIGURE_COLS) / 2;
  tetr->y = -FIGURE_ROWS;  // start above the field"
  tetr->type = tetr->next_type;
  tetr->next_type = rand() % 7;
  picked_matrix(tetr->current_shape, shapes[tetr->type]);
  picked_matrix(frame->next, shapes[tetr->next_type]);
}

void shift(GameInfo_t* frame, figure* tetr, struct timespec* last_fall) {
  struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  if ((now.tv_sec - last_fall->tv_sec) +
          1e-9 * (now.tv_nsec - last_fall->tv_nsec) >=
      (1.0 / frame->speed * 0.5)) {
    tetr->y += 1;
    clock_gettime(CLOCK_MONOTONIC, last_fall);
  }
}

GameInfo_t updateCurrentState() {
  static GameInfo_t frame;
  if (frame.field == NULL) frame.field = create_field(MAP_ROWS, MAP_COLS);
  if (frame.next == NULL) frame.next = create_field(FIGURE_ROWS, FIGURE_COLS);
  read_record(&frame);
  delete_row(&frame);
  return frame;
}

void down(figure* tetr) { tetr->y++; }
void right(figure* tetr) { tetr->x++; }
void left(figure* tetr) { tetr->x--; }
void up(figure* tetr) { (void)tetr; }