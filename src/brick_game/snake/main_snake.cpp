#include <iostream>

#include "game.h"
int main() {
  using namespace s21;
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, false);
  curs_set(0);
  start_color();
  init_pair(1, COLOR_RED, COLOR_RED);
  init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
  WINDOW *win1 = newwin(height1 + 2, 2 * width1 + 2, 0, 0);
  WINDOW *win2 = newwin(8, 15, 0, 2 * width1 + 3);
  State *st = whichState();
  *st = State_start;
  GameInfo_t *info = updateCurrentState();
  print_field(st, win1, win2);
  int ch = getch();
  while (1) {
    userInput((UserAction_t)ch);
    if (*st == State_move) {
      timeout(info->speed);
    }

    print_field(st, win1, win2);
    if (*st == State_exit) {
      break;
    }
    ch = getch();
    if (*st == State_pause || *st == State_terminate || *st == State_start) {
      nodelay(stdscr, false);
    } else if (*st == State_move) {
      nodelay(stdscr, true);
    }
  }
  refresh();
  delwin(win1);
  delwin(win2);
  endwin();
  return 0;
}