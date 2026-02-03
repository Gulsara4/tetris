#include "s21_tetris.h"

int main() {
  initscr();
  start_color();
  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  cbreak();
  noecho();
  nodelay(stdscr, TRUE);
  curs_set(0);
  WINDOW *win = newwin(height1 + 2, width1 * 2 + 2, 0, 0);
  WINDOW *next = newwin(u1 + 2, u2, 0, width1 * 2 + 3);
  WINDOW *score_ = newwin(z1, u2 + 5, u1 + 3, width1 * 2 + 3);
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  dot->state = Start_st;
  char action = ERR;
  while (1) {
    refresh();

    action = getch();
    userInput(action, false);
    if (dot->state == Pause_st || info->pause == 1) {
      nodelay(stdscr, FALSE);
    }
    if ((dot->state == Moving) &&
        (action == Left || action == Right || action == Up || action == Down ||
         action == Action)) {
      timeout(0);
    } else if (dot->state == Attaching || dot->state == Shifting ||
               dot->state == Spawn) {
      timeout(0);
    } else {
      int delay = 1000 - info->speed;
      timeout(delay);
    }
    finishprint(win, next, score_);
    if (dot->state == Exit_st) {
      break;
    }
  }
  delwin(win);
  delwin(score_);
  delwin(next);

  endwin();
  return 0;
}
