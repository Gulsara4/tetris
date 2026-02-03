
#include "s21_tetris.h"

void st_shifting() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  figure curr_fig = *dot->kit[dot->c];
  for (int i = 0; i < 4; i++) {
    int x = curr_fig.rotate[dot->mode][i][0] + dot->x;
    int y = curr_fig.rotate[dot->mode][i][1] + dot->y;

    info->field[x][y] = 1;
  }
}
void st_attaching() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  figure curr_fig = *dot->kit[dot->c];
  for (int i = 0; i < 4; i++) {
    int x = curr_fig.rotate[dot->mode][i][0] + dot->x;
    int y = curr_fig.rotate[dot->mode][i][1] + dot->y;
    info->field[x][y] = 2;
  }
}

int st_moving(UserAction_t action, bool hold) {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  bool flag;
  if ((int)action != ERR && dot->c_p < GET_SHIFTS(info->level)) {
    figure curr = *dot->kit[dot->c];

    if (action == Up && hold == false) {
      dot->mode = (dot->mode + rotate_able()) % curr.amount_rotate;
    } else if (action == Left) {
      if (check_border(0) == 0) dot->y--;
    } else if (action == Right) {
      if (check_border(1) == 0) dot->y++;
    } else if (action == Down) {
      if (check_border(2) == 0) dot->x++;
    } else if (action == Action) {
      while (check_border(2) == 0) dot->x++;
    }

    flag = stateonground(0);
    dot->c_p++;
  } else {
    flag = stateonground(1);
    if (check_border(2) == 0) {
      dot->x++;
      dot->c_p = 0;
    }
  }
  return flag;
}
GameInfo_t updateCurrentState() {
  GameInfo_t *curr = get_instance();
  return *curr;
}

void handling_start() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  work_with_field(0);
  init_data();
  full_clear();
  info->high_score = play_w_file("r", 0);
  info->score = 0;
  info->level = 1;
  info->speed = 0;
  info->pause = 0;
  set_info_next();
  dot->state = Spawn;
}
void userInput(UserAction_t action, bool hold) {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  switch (dot->state) {
    case Start_st:
      if (action == Start) {
        handling_start();
        dot->state = Spawn;
      }
      break;
    case Pause_st:
      if (action == Pause)
        dot->state = Moving;
      else if (action == Terminate)
        dot->state = Game_Over;
      else if (action == Exit) {
        work_with_field(1);
        dot->state = Exit_st;
      }
      break;
    case Spawn:
      dot->c = info->next;
      set_info_next();
      if (check_border(3) == 1)
        dot->state = Game_Over;
      else
        dot->state = Moving;
      break;
    case Moving: {
      if (action == Pause) {
        info->pause = 1;
        dot->state = Pause_st;
        break;
      }
      if (st_moving(action, hold) == 1) {
        dot->state = Attaching;
        dot->c_p = 0;
      } else {
        dot->state = Shifting;
      }
      break;
    }
    case Attaching:
      clear_field();
      st_attaching();
      analyze_field1();
      dot->c_p = 0;
      dot->state = Spawn;
      if (info->level == 10) dot->state = Game_Over;
      break;
    case Shifting:
      clear_field();
      st_shifting();
      dot->state = Moving;
      break;
    case Game_Over:
      dot->state = Start_st;
      work_with_field(1);
      break;
  }
  info->pause = (dot->state == Pause_st);
}