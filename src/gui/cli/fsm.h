#ifndef FSM_H
#define FSM_H
#define END \
  -1  // To remove the display of the next figure after the game is over
#define _POSIX_C_SOURCE 199309L

#include "../../brick_game/tetris.h"

/**
 * @brief Structure of possible signals
 * @param Action This signal rotates the figure by 90 degrees
 * @param Nosig No signal
 * @param Terminate Exit signal
 */
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Nosig
} UserAction_t;

/**
 * @brief Structure of possible states of a finite state machine
 * @param START This stage handles terminal transition to 'game mode', game
 * field printing, and initial figure initialization
 * @param MOVE_FIGURE "This stage handles figure direction movement and rotation
 * @param SHIFT This stage performs the selected movement, handles time interval
 * movement, and updates the screen
 * @param CHECK This stage checks if the game is over and if the figure is
 * fixed. If it is fixed, a new figure is initialized, otherwise it transitions
 * to the move_figure
 * @param INIT_FIGURE This stage initializes the figure and transitions to the
 * MOVE_FIGURE
 * @param PAUSE From this stage, you can only exit the game or continue playing
 * @param GAME_OVER From this stage, you can start a new game or exit the game
 * @param EXIT This is the game over stage. There is nothing here except for
 * ending the game
 */
typedef enum {
  START,
  MOVE_FIGURE,
  SHIFT,
  CHECK,
  INIT_FIGURE,
  PAUSE,
  GAME_OVER,
  EXIT
} states;

/**
 * @brief Structure with all the parameters required by the functions
 */
typedef struct {
  figure* tetr;
  GameInfo_t* frame;
  struct timespec* last_fall;
  states* state;
  UserAction_t* action;
} params_t;

/**
 * @brief The function is called from the START stage. Function actions are
 * described in the stage structure description
 */
void start_game(params_t* prms);

/**
 * @brief Malfunction. Not operating
 */
void move_up(params_t* prms);

/**
 * @brief Drops figures down until fixed
 */
void move_down(params_t* prms);
void move_right(params_t* prms);
void move_left(params_t* prms);
void rotate_figure(params_t* prms);

/**
 * @brief Moving the figure down at time intervals and updating the screen
 */
void shift_figure(params_t* prms);

/**
 * @brief Description of the function action in the CHECK stage description of
 * the states structure
 */
void check_over(params_t* prms);

/**
 * @brief Description of the function action in the INIT_FIGURE stage
 * description of the states structure
 */
void init_figure(params_t* prms);

/**
 * @brief Description of the function action in the PAUSE stage description of
 * the states structure
 */
void game_pause(params_t* prms);

/**
 * @brief Ends the game
 */
void exit_state(params_t* prms);

/**
 * @brief Clears all fields, updates the screen, and offers to play again
 */
void game_over(params_t* prms);

/**
 * @brief Converts button press to signal
 * @param user_input Pressed button
 * @return Signal
 */
UserAction_t get_signal(int user_input);

/**
 * @brief Here, the params structure is initialized and the selected function is
 * executed based on the stage and signal passed
 * @param sig Signal pointer
 * @param state Stage pointer
 * @param frame Field pointer
 * @param tetr Figure pointer
 * @param last_fall Pointer to last figure drop time
 */
void sigact(UserAction_t* sig, states* state, GameInfo_t* frame, figure* tetr,
            struct timespec* last_fall);

/**
 * @brief Main function. Initial field, figure, and time-before-move-down
 * initialization is performed. The finite state machine operates until the EXIT
 * stage is reached. Memory is cleared before exiting the program
 */
void userInput(UserAction_t action, int hold);

/**
 * @brief This is a definition of a function pointer to a function that takes a
 * pointer to a 'params_t' structure as an argument and returns void
 */
typedef void (*actions)(params_t* prms);

#endif  // FSM_H
