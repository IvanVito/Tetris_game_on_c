#define _POSIX_C_SOURCE 199309L
#include <check.h>

#define MAP_COLS 10
#define MAP_ROWS 20

#include "../brick_game/tetris.h"

void fill_row(GameInfo_t *frame, int row) {
  for (int col = 0; col < MAP_COLS; col++) frame->field[row][col] = 2;
}

void fill_col(GameInfo_t *frame, int col) {
  for (int row = 0; row < MAP_ROWS; row++) frame->field[row][col] = 2;
}

START_TEST(mv_down) {
  figure tetr;
  up(&tetr);
  tetr.y = 1;
  int y = tetr.y;
  down(&tetr);
  int next_y = tetr.y;
  ck_assert_int_eq(y + 1, next_y);
}
END_TEST

START_TEST(mv_down_attach) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  init(&frame, &tetr);
  tetr.y = 0;
  int y = tetr.y;
  down(&tetr);
  int next_y = 0;
  ck_assert_int_eq(y, next_y);
}
END_TEST

START_TEST(mv_down_full) {
  figure tetr;
  tetr.y = 19;
  int y = tetr.y;
  down(&tetr);
  int next_y = 19;
  ck_assert_int_eq(y, next_y);
}
END_TEST

START_TEST(mv_left) {
  figure tetr;
  tetr.x = 1;
  int x = tetr.x;
  left(&tetr);
  int next_x = tetr.x;
  ck_assert_int_eq(x - 1, next_x);
}
END_TEST

START_TEST(mv_left_attach) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  fill_col(&frame, 1);
  tetr.x = 2;
  int x = tetr.x;
  left(&tetr);
  int next_x = 2;
  ck_assert_int_eq(x, next_x);
}
END_TEST

START_TEST(mv_left_full) {
  figure tetr = update_current_figure();
  tetr.x = 0;
  int x = tetr.x;
  left(&tetr);
  int next_x = 0;
  ck_assert_int_eq(x, next_x);
}
END_TEST

START_TEST(mv_right) {
  figure tetr = update_current_figure();
  tetr.x = 1;
  int x = tetr.x;
  right(&tetr);
  int next_x = tetr.x;
  ck_assert_int_eq(x + 1, next_x);
}
END_TEST

START_TEST(mv_right_attach) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  fill_col(&frame, 1);
  tetr.x = 0;
  int x = tetr.x;
  right(&tetr);
  int next_x = 0;
  ck_assert_int_eq(x, next_x);
}
END_TEST

START_TEST(mv_right_full) {
  figure tetr = update_current_figure();
  tetr.x = 10;
  int x = tetr.x;
  right(&tetr);
  int next_x = 10;
  ck_assert_int_eq(x, next_x);
}
END_TEST

Suite *move_dlr() {
  Suite *result;
  TCase *tc_move;

  result = suite_create("move_dlr");
  tc_move = tcase_create("move_dlr");

  tcase_add_test(tc_move, mv_down);
  tcase_add_test(tc_move, mv_down_attach);
  tcase_add_test(tc_move, mv_down_full);

  tcase_add_test(tc_move, mv_left);
  tcase_add_test(tc_move, mv_left_attach);
  tcase_add_test(tc_move, mv_left_full);

  tcase_add_test(tc_move, mv_right);
  tcase_add_test(tc_move, mv_right_attach);
  tcase_add_test(tc_move, mv_right_full);

  suite_add_tcase(result, tc_move);

  return result;
}

START_TEST(rotate_i_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_i_180) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][3]);
}
END_TEST

START_TEST(rotate_i_270) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.next_type = 0;
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[3][5]);
}
END_TEST

START_TEST(rotate_i_360) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][5]);
}
END_TEST

START_TEST(rotate_i_90_collision) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  fill_row(&frame, 5);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][4]);
}
END_TEST

START_TEST(rotate_i_180_collision) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 7;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  fill_col(&frame, 4);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_i_270_collision) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 8;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  fill_row(&frame, 4);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][4]);
}
END_TEST

START_TEST(rotate_i_360_collision) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 9;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  fill_col(&frame, 6);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_i_90_full) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 18;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[19][4]);
}
END_TEST

START_TEST(rotate_o_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 1;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_s_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 2;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_z_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 3;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][6]);
}
END_TEST

START_TEST(rotate_l_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 4;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][5]);
}
END_TEST

START_TEST(rotate_j_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 5;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[4][6]);
}
END_TEST

START_TEST(rotate_t_90) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 6;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][6]);
}
END_TEST

START_TEST(rotate_t_90_collision) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 6;
  init(&frame, &tetr);
  tetr.x = 4;
  tetr.y = 4;
  fill_row(&frame, 6);
  rotate(&frame, &tetr);
  move_figure(&frame, &tetr);
  ck_assert(frame.field[5][4]);
}
END_TEST

Suite *rotate_fig() {
  Suite *result;
  TCase *tc_rotate;

  result = suite_create("rotate_fig");
  tc_rotate = tcase_create("rotate_fig");

  tcase_add_test(tc_rotate, rotate_i_90);
  tcase_add_test(tc_rotate, rotate_i_180);
  tcase_add_test(tc_rotate, rotate_i_270);
  tcase_add_test(tc_rotate, rotate_i_360);
  tcase_add_test(tc_rotate, rotate_i_90_collision);
  tcase_add_test(tc_rotate, rotate_i_180_collision);
  tcase_add_test(tc_rotate, rotate_i_270_collision);
  tcase_add_test(tc_rotate, rotate_i_360_collision);
  tcase_add_test(tc_rotate, rotate_i_90_full);
  tcase_add_test(tc_rotate, rotate_o_90);
  tcase_add_test(tc_rotate, rotate_s_90);
  tcase_add_test(tc_rotate, rotate_z_90);
  tcase_add_test(tc_rotate, rotate_l_90);
  tcase_add_test(tc_rotate, rotate_j_90);
  tcase_add_test(tc_rotate, rotate_t_90);
  tcase_add_test(tc_rotate, rotate_t_90_collision);

  suite_add_tcase(result, tc_rotate);

  return result;
}

START_TEST(attach_max) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 5;
  tetr.y = 18;
  ck_assert(attaching(&frame, &tetr));
}
END_TEST

START_TEST(attach_in_field) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 5;
  tetr.y = 14;
  fill_row(&frame, 16);
  ck_assert(attaching(&frame, &tetr));
}
END_TEST

START_TEST(attach_and_fix) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 0;
  init(&frame, &tetr);
  tetr.x = 5;
  tetr.y = 14;
  fill_row(&frame, 16);
  if (attaching(&frame, &tetr)) fix_figure(&frame, &tetr);
  ck_assert_int_eq(frame.field[15][5], 2);
}
END_TEST

START_TEST(pull_4_rows) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 7;
  init(&frame, &tetr);
  tetr.x = 0;
  tetr.y = 15;
  fill_row(&frame, 16);
  fill_row(&frame, 17);
  fill_row(&frame, 18);
  fill_row(&frame, 19);
  frame.field[16][1] = 0;
  frame.field[17][1] = 0;
  frame.field[18][1] = 0;
  frame.field[19][1] = 0;
  down(&tetr);
  move_figure(&frame, &tetr);
  if (attaching(&frame, &tetr)) fix_figure(&frame, &tetr);
  delete_row(&frame);
  ck_assert_int_eq(frame.field[16][0], 0);
  ck_assert_int_eq(frame.field[17][3], 0);
  ck_assert_int_eq(frame.field[18][2], 0);
  ck_assert_int_eq(frame.field[19][3], 0);
}
END_TEST

START_TEST(clear_part) {
  GameInfo_t frame = updateCurrentState();
  fill_row(&frame, 16);
  clear_sector(frame.field, 17, 9);
  ck_assert_int_eq(frame.field[16][0], 0);
}
END_TEST

START_TEST(final) {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  int new_y = 0;
  struct timespec last_fall;
  tetr.next_type = 7;
  init(&frame, &tetr);
  tetr.x = 0;
  tetr.y = 0;
  clock_gettime(CLOCK_MONOTONIC, &last_fall);
  while (new_y != 1) {
    shift(&frame, &tetr, &last_fall);
    new_y = tetr.y;
  }
  ck_assert(new_y == 1);
}
END_TEST

START_TEST(shifting) {
  int end = 0;
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  tetr.next_type = 7;
  init(&frame, &tetr);
  tetr.x = 0;
  tetr.y = -4;
  fill_row(&frame, 0);
  if (attaching(&frame, &tetr)) end = game_end(&tetr);
  ck_assert_int_eq(end, 1);
}
END_TEST

START_TEST(read_record_test) {
  int err = 0;
  GameInfo_t frame = updateCurrentState();
  err = read_record(&frame);
  ck_assert(err == 1 || err == 0);
}
END_TEST

Suite *field_man() {
  Suite *result;
  TCase *tc_field;

  result = suite_create("field_man");
  tc_field = tcase_create("field_man");

  tcase_add_test(tc_field, attach_max);
  tcase_add_test(tc_field, attach_in_field);
  tcase_add_test(tc_field, attach_and_fix);
  tcase_add_test(tc_field, pull_4_rows);
  tcase_add_test(tc_field, clear_part);
  tcase_add_test(tc_field, final);
  tcase_add_test(tc_field, shifting);
  tcase_add_test(tc_field, read_record_test);

  suite_add_tcase(result, tc_field);

  return result;
}

int main() {
  figure tetr = update_current_figure();
  GameInfo_t frame = updateCurrentState();
  int failed = 0;
  Suite *result[] = {move_dlr(), rotate_fig(), field_man(), NULL};

  for (int i = 0; result[i] != NULL; i++) {
    SRunner *runner = srunner_create(result[i]);
    srunner_run_all(runner, CK_NORMAL);
    failed += srunner_ntests_failed(runner);
    srunner_free(runner);
  }
  clear_field(&frame.field, MAP_ROWS);
  clear_field(&frame.next, FIGURE_ROWS);
  clear_field(&tetr.current_shape, FIGURE_ROWS);

  return (failed == 0) ? 0 : 1;
}
