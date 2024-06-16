#include "fsm.h"

#include "../../brick_game/tetris.h"
#include "tetris_gui.h"

int main() {
  UserAction_t action = Start;
  userInput(action, 0);
  endwin();
  return 0;
}

actions fsm_table[8][9] = {
    {start_game, start_game, exit_state, start_game, start_game, start_game,
     start_game, start_game, start_game},
    {NULL, game_pause, exit_state, move_left, move_right, move_up, move_down,
     rotate_figure, shift_figure},
    {shift_figure, shift_figure, exit_state, shift_figure, shift_figure,
     shift_figure, shift_figure, shift_figure, shift_figure},
    {check_over, check_over, exit_state, check_over, check_over, check_over,
     check_over, check_over, check_over},
    {init_figure, init_figure, exit_state, init_figure, init_figure,
     init_figure, init_figure, init_figure, init_figure},
    {game_pause, game_pause, exit_state, game_pause, game_pause, game_pause,
     game_pause, game_pause, game_pause},
    {game_over, game_over, game_over, game_over, game_over, game_over,
     game_over, game_over, game_over},
    {exit_state, exit_state, exit_state, exit_state, exit_state, exit_state,
     exit_state, exit_state, exit_state}};

void start_game(params_t* prms) {
  init_front(prms->frame);
  print_field(prms->frame);
  srand(time(NULL));
  print_start();
  int go = getch();
  while (go != ENTER && go != ESCAPE) go = getch();
  if (go == ENTER) {
    prms->tetr->next_type = rand() % 7;
    init_figure(prms);
    *(prms->state) = MOVE_FIGURE;
  } else {
    *(prms->state) = EXIT;
  }
}

void move_up(params_t* prms) { (void)prms; }

void move_down(params_t* prms) {
  clear_past_state(prms->frame, prms->tetr);
  while (attaching(prms->frame, prms->tetr) != 1) {
    print_stats(prms->frame, prms->tetr);
    down(prms->tetr);
  }
  *(prms->state) = SHIFT;
}

void move_right(params_t* prms) {
  right(prms->tetr);
  *(prms->state) = SHIFT;
  napms(50);
}

void move_left(params_t* prms) {
  left(prms->tetr);
  *(prms->state) = SHIFT;
  napms(50);
}

void rotate_figure(params_t* prms) {
  rotate(prms->frame, prms->tetr);
  *(prms->state) = SHIFT;
}

void shift_figure(params_t* prms) {
  shift(prms->frame, prms->tetr, prms->last_fall);
  refresh_scr(prms->frame, prms->tetr);
  *(prms->state) = CHECK;
}

void check_over(params_t* prms) {
  if (attaching(prms->frame, prms->tetr) == 1) {
    int game = game_end(prms->tetr);
    fix_figure(prms->frame, prms->tetr);
    if (game == 0) *(prms->state) = INIT_FIGURE;
    if (game == 1) *(prms->state) = GAME_OVER;
  } else {
    *(prms->state) = MOVE_FIGURE;
  }
}

void game_over(params_t* prms) {
  clear_sector(prms->frame->field, MAP_ROWS, MAP_COLS);
  clear_sector(prms->frame->next, FIGURE_ROWS, FIGURE_COLS);
  clear_sector(prms->tetr->current_shape, FIGURE_ROWS, FIGURE_COLS);
  prms->tetr->type = END;
  score(prms->frame, 0, RESTART);
  refresh_scr(prms->frame, prms->tetr);
  print_game_over();
  int button = getch();
  while (button != ENTER && button != ESCAPE) button = getch();
  if (button == ENTER)
    *(prms->state) = START;
  else
    *(prms->state) = EXIT;
}

void init_figure(params_t* prms) {
  init(prms->frame, prms->tetr);
  if (prms->state != Start) *(prms->state) = MOVE_FIGURE;
}

void game_pause(params_t* prms) {
  print_pause();
  int game_paused = 1, button = 0;
  nodelay(stdscr, FALSE);
  while (game_paused == 1) {
    button = getch();
    if (button == P || button == 'P' || button == ESCAPE) game_paused = 0;
  }
  if (button == P || button == 'P') {
    nodelay(stdscr, TRUE);
    *(prms->state) = MOVE_FIGURE;
  } else {
    *(prms->state) = EXIT;
  }
}

void exit_state(params_t* prms) { *(prms->state) = EXIT; }

UserAction_t get_signal(int user_input) {
  UserAction_t sig = Nosig;
  if (user_input == KEY_UP)
    sig = Up;
  else if (user_input == KEY_DOWN)
    sig = Down;
  else if (user_input == KEY_LEFT)
    sig = Left;
  else if (user_input == KEY_RIGHT)
    sig = Right;
  else if (user_input == ESCAPE)
    sig = Terminate;
  else if (user_input == P)
    sig = Pause;
  else if (user_input == SPACE)
    sig = Action;
  else if (user_input == ENTER)
    sig = Start;
  return sig;
}

void userInput(UserAction_t action, int hold) {
  (void)hold;
  states state = START;
  GameInfo_t frame = updateCurrentState(state);
  figure tetr = update_current_figure();
  struct timespec last_fall;
  clock_gettime(CLOCK_MONOTONIC, &last_fall);
  while (state != EXIT) {
    sigact(&action, &state, &frame, &tetr, &last_fall);
    if (state == MOVE_FIGURE || state == START) action = get_signal(getch());
    frame = updateCurrentState(state);
  }
  napms(
      200);  // For displaying figure drop, if only one block goes out of field
  clear_field(&frame.field, MAP_ROWS);
  clear_field(&frame.next, FIGURE_ROWS);
  clear_field(&tetr.current_shape, FIGURE_ROWS);
}

void sigact(UserAction_t* sig, states* state, GameInfo_t* frame, figure* tetr,
            struct timespec* last_fall) {
  actions act = fsm_table[*state][*sig];
  if (act != NULL) {
    params_t prms;
    prms.tetr = tetr;
    prms.frame = frame;
    prms.last_fall = last_fall;
    prms.state = state;
    prms.action = sig;
    act(&prms);
  }
}
