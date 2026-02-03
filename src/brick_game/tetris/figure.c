#include "s21_tetris.h"

const figure square = {  //[which position ][which cube][x or y]
    "square",
    2,
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}};

const figure z_ref = {"s",
                      2,
                      {{{0, 0}, {0, 1}, {1, 0}, {1, -1}},
                       {{0, 0}, {-1, 0}, {0, 1}, {1, 1}},
                       {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                       {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}};
const figure z = {"z",
                  2,
                  {{{0, 0}, {0, -1}, {1, 0}, {1, 1}},
                   {{0, 0}, {1, 0}, {0, 1}, {-1, 1}},
                   {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                   {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}};
const figure l = {"L",
                  4,
                  {{{0, 0}, {0, -1}, {1, -1}, {0, 1}},
                   {{0, 0}, {-1, 0}, {1, 0}, {1, 1}},
                   {{0, 0}, {0, -1}, {0, 1}, {-1, 1}},
                   {{0, 0}, {1, 0}, {-1, 0}, {-1, -1}}}};
const figure l_ref = {"J",
                      4,
                      {{{0, 0}, {0, -1}, {0, 1}, {1, 1}},
                       {{0, 0}, {1, 0}, {-1, 0}, {-1, 1}},
                       {{0, 0}, {0, 1}, {0, -1}, {-1, -1}},
                       {{0, 0}, {-1, 0}, {1, 0}, {1, -1}}}};
const figure t = {"T",
                  4,
                  {{{0, 0}, {0, -1}, {0, 1}, {1, 0}},
                   {{0, 0}, {-1, 0}, {0, 1}, {1, 0}},
                   {{0, 0}, {0, -1}, {-1, 0}, {0, 1}},
                   {{0, 0}, {-1, 0}, {0, -1}, {1, 0}}}};
const figure i_fig = {"I",
                      2,
                      {{{0, 0}, {1, 0}, {2, 0}, {3, 0}},
                       {{0, 0}, {0, 1}, {0, 2}, {0, 3}},
                       {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
                       {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}};

void init_data() {
  Point *info2 = get_point();
  info2->kit[0] = &square;
  info2->kit[1] = &z_ref;
  info2->kit[2] = &z;
  info2->kit[3] = &l;
  info2->kit[4] = &l_ref;
  info2->kit[5] = &i_fig;
  info2->kit[6] = &t;
}

GameInfo_t *get_instance() {
  static GameInfo_t params;
  return &params;
}

Point *get_point() {
  static Point arg;
  return &arg;
}
void set_info_next() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  srand(time(NULL));
  int rnd = rand();
  int c = rnd % 7;
  c = rand() % 7;
  info->next = c;
  dot->x = source_x;
  dot->y = 5;
  dot->mode = 0;
}
// fl 1 истек таймер
int stateonground(int fl) {
  GameInfo_t *info = get_instance();
  int flag = 0;
  Point *dot = get_point();
  figure curr_fig = *dot->kit[dot->c];
  for (int i = 0; i < 4; i++) {
    int xn = curr_fig.rotate[dot->mode][i][0] + dot->x;
    int yn = curr_fig.rotate[dot->mode][i][1] + dot->y;
    if ((xn == 19 || info->field[xn + 1][yn] == 2) && fl == 1) {
      flag = 1;
      break;
    }
  }
  return flag;
}

int check_border(int side) {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  figure curr_fig = *dot->kit[dot->c];
  for (int i = 0; i < 4; i++) {
    int xn = curr_fig.rotate[dot->mode][i][0] + dot->x;
    int yn = curr_fig.rotate[dot->mode][i][1] + dot->y;
    if (side == 0) {
      if (info->field[xn][yn - 1] == 2 || yn == 0) {
        return 1;
      }
    } else if (side == 1) {
      if (info->field[xn][yn + 1] == 2 || yn == 9) {
        return 1;
      }
    } else if (side == 2) {
      if (xn < 0 || xn + 1 >= 20 || info->field[xn + 1][yn] == 2) {
        return 1;
      }
    } else if (side == 3) {  // up
      if (xn < 0 || info->field[xn][yn] == 2) {
        return 1;
      }
    }
  }
  return 0;
}
// 0 стоим на месте
int rotate_able() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  figure curr_fig = *dot->kit[dot->c];

  int which = (dot->mode + 1) % curr_fig.amount_rotate;
  for (int i = 0; i < 4; i++) {
    int x = curr_fig.rotate[which][i][0];
    int y = curr_fig.rotate[which][i][1];
    int new_x = dot->x + x;
    int new_y = dot->y + y;
    if (new_y <= -1 || new_y >= 10) {
      return 0;

    } else if (new_x <= -1 || new_x >= 20) {
      return 0;
    } else if (info->field[new_x][new_y] == 2) {
      return 0;
    }
  }
  return 1;
}