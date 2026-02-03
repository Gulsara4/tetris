
#include "game.h"
namespace s21 {
void resetField() {
  GameInfo_t *info = updateCurrentState();
  for (int i = 0; i < height1; i++) {
    for (int j = 0; j < width1; j++) {
      if (info->field[i][j] != 3) {
        info->field[i][j] = 0;
      }
    }
  }
}
void fillField(Snake &snake, const Apple &app) {  // гдя яблоки и змея
  GameInfo_t *info = updateCurrentState();
  auto occupied = getOccupiedCells(snake);
  for (long long code : occupied) {
    int x = static_cast<int>(code >> 32);
    int y = static_cast<int>(code & 0xFFFFFFFF);
    info->field[x][y] = 1;
  }
  info->field[app.getX()][app.getY()] = 3;
}
void resetDynamicField(int a) {
  GameInfo_t *info = updateCurrentState();
  if (a == 0) {  //
    info->field = (int **)calloc((size_t)height1, sizeof(int *));
    for (int i = 0; i < height1; i++) {
      info->field[i] = (int *)calloc((size_t)width1, sizeof(int));
    }
  } else if (a == 1) {
    for (int i = 0; i < height1; i++) {
      if (info->field[i] != NULL) {
        free(info->field[i]);
        info->field[i] = NULL;
      }
    }
    if (info->field != NULL) {
      free(info->field);
      info->field = NULL;
    }
  }
}
bool isValid(UserAction_t a) {
  if (a == Up || a == Down || a == Left || a == Right || a == ERR) {
    return true;
  } else {
    return false;
  }
}
void userInput(UserAction_t action) {
  State *st = whichState();
  GameInfo_t *info = updateCurrentState();
  Snake &my = getSnake();
  if (action == Pause && *st == State_move) {
    *st = (State_pause);
  } else if (action == Terminate && *st == (State_pause)) {
    *st = (State_terminate);
    resetDynamicField(1);
  } else if (*st == (State_pause) && action == Pause) {
    *st = (State_move);
  } else if (*st == State_start && action == Start) {
    initialization();
    *st = (State_move);
  } else if (*st == State_terminate) {
    *st = State_start;
  } else if (action == Exit && (*st == (State_pause))) {
    resetDynamicField(1);
    *st = State_exit;
  } else if ((action == Up || action == Down || action == Left ||
              action == Right) &&
             *st == (State_move)) {
    if (processSnakeMove(action) == false) {
      *st = State_terminate;
    }
  } else if (action == Action && *st == (State_move)) {
    if (info->speed > 100) {
      info->speed = info->speed - 100;
    }
  } else if ((action == ERR && *st == State_move) ||
             (isValid(action) == false && *st == State_move)) {
    int previousDirection = getDirectionBetweenVectors(my.body[0], my.body[1]);
    if (previousDirection == 0) {
      action = Up;
    } else if (previousDirection == 1) {
      action = Down;
    } else if (previousDirection == 2) {
      action = Left;
    } else if (previousDirection == 3) {
      action = Right;
    }
    if (processSnakeMove(action) == false) {
      *st = State_terminate;
    }
  }

  if (my.get_length_of_snake() == 200) {
    *st = State_terminate;
  }
  info->pause = (*st == State_pause);
}
}  // namespace s21