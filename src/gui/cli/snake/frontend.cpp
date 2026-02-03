
#include "../../../brick_game/snake/game.h"
namespace s21 {
void print_field(State *st, WINDOW *win1, WINDOW *win2) {
  GameInfo_t *info = updateCurrentState();
  refresh();
  werase(win1);
  werase(win2);
  box(win1, 0, 0);
  if (*st == State_start) {
    mvwprintw(win1, 1, 3, "Game Snake");
    mvwprintw(win1, 2, 1, "Press g to start");
    mvwprintw(win1, 3, 1, "a, s, d, w, z -");
    mvwprintw(win1, 4, 1, " to control");
    mvwprintw(win1, 5, 1, "p - pause");
    mvwprintw(win1, 6, 1, "q - to finish");
    mvwprintw(win1, 7, 1, "e - exit");

  } else if (*st == State_terminate) {
    mvwprintw(win1, 1, 1, "Game over");
  } else if (*st == State_move) {
    for (int i = 0; i < height1; i++) {
      for (int j = 0; j < width1; j++) {
        if (info->field[i][j] == 1) {
          wattron(win1, COLOR_PAIR(1));
          mvwprintw(win1, i + 1, j * 2 + 1, "  ");
          wattroff(win1, COLOR_PAIR(1));

        } else if (info->field[i][j] == 3) {
          wattron(win1, COLOR_PAIR(2));
          mvwprintw(win1, i + 1, j * 2 + 1, "  ");
          wattroff(win1, COLOR_PAIR(2));
        } else {
          mvwprintw(win1, i + 1, j * 2 + 1, "  ");
        }
      }
      std::cout << "\n";
    }

  } else if (*st == State_pause) {
    mvwprintw(win1, 1, 4, "Pause");
  }
  int y = 1;
  int x = 2;
  box(win2, 0, 0);
  mvwprintw(win2, y, x, "Score %d", info->score);
  mvwprintw(win2, y + 1, x, "%s %d", "High score", info->high_score);
  mvwprintw(win2, y + 2, x, "%s %d", "Level", info->level);
  mvwprintw(win2, y + 3, x, "Speed %d", (1000 - info->speed));
  mvwprintw(win2, y + 4, x, "Pause: %s", info->pause ? "on" : "off");
  wrefresh(win1);
  wrefresh(win2);
}
}  // namespace s21