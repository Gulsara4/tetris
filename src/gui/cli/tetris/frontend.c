#include "../../../brick_game/tetris/s21_tetris.h"

#ifndef TEST
void finishprint(WINDOW *win, WINDOW *next, WINDOW *score_) {
  GameInfo_t info = updateCurrentState();
  wclear(win);
  wclear(next);
  wclear(score_);
  box(win, 0, 0);
  box(next, 0, 0);
  box(score_, 0, 0);
  Point *dot = get_point();

  if (dot->state == Start_st) {
    welcome(win);
  } else if (dot->state == Pause_st) {
    mvwprintw(win, 1, 4, "Pause");
    mvwprintw(win, 2, 2, "q - to terminate");
    mvwprintw(win, 3, 2, "e - to exit");

  } else if (dot->state == Game_Over) {
    mvwprintw(win, 1, 4, "Game Over");

  } else if (dot->state != Exit_st) {
    figure fig = *dot->kit[info.next];

    for (int i = 0; i < height1; i++) {
      for (int j = 0; j < width1; j++) {
        if (info.field[i][j] == 1) {
          wattron(win, COLOR_PAIR(1));
          mvwprintw(win, i + 1, j * 2 + 1, "  ");
          wattroff(win, COLOR_PAIR(1));

        } else if (info.field[i][j] == 2) {
          wattron(win, COLOR_PAIR(2));

          mvwprintw(win, i + 1, j * 2 + 1, "  ");
          wattroff(win, COLOR_PAIR(2));

        } else {
          mvwprintw(win, i + 1, j * 2 + 1, " ");
        }
      }
      // mvwprintw(win, i + 1, width, "%d", i+1);
    }

    mvwprintw(next, 1, 4, "next");
    for (int i = 0; i < 4; i++) {
      int x_f = fig.rotate[0][i][0];
      int y_f = fig.rotate[0][i][1];
      int p1 = x_f + u1 / 2;
      int p2 = y_f + u2 / 2;
      mvwprintw(next, p1, p2, "@");
    }
    wrefresh(next);
  }
  mvwprintw(score_, 1, 1, "score %d", info.score);
  mvwprintw(score_, 2, 1, "high score %d", info.high_score);
  mvwprintw(score_, 3, 1, "level %d", info.level);
  mvwprintw(score_, 4, 1, "speed %d", (1000 - info.speed));
  mvwprintw(score_, 5, 1, "pause: %s", info.pause ? "on" : "off");
  wrefresh(score_);
  wrefresh(win);
}
void welcome(WINDOW *win) {
  mvwprintw(win, 1, 4, "WELCOME");
  mvwprintw(win, 4, 3, "Control button:");
  mvwprintw(win, 5, 1, "a - left, d - rigth");
  mvwprintw(win, 6, 1, "w - rotate, s - down");
  mvwprintw(win, 7, 1, "p - pause");
  mvwprintw(win, 8, 1, "q - finish the game");
  mvwprintw(win, 9, 1, "e - exit");
  mvwprintw(win, 12, 1, "PRESS \"O\" TO START");
}
#endif