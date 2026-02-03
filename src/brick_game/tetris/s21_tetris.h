
#include <math.h>
#include <ncurses/ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#define height1 20
#define width1 10
#define h2 25
#define w2 20
#define u1 5
#define u2 10
#define z1 13
#define GET_SHIFTS(level) (fmax(1, 4 - ceil((level) / 4.0)))
#define source_x 0

typedef struct {
  char name[50];
  int amount_rotate;
  int rotate[4][4][2];

} figure;
typedef struct {
  int x;
  int y;
  int mode;
  int c;
  const figure *kit[7];
  int state;
  int c_p;  // amount of shift on bottom
} Point;

typedef enum {
  Start = 'o',
  Pause = 'p',
  Terminate = 'q',
  Left = 'a',
  Right = 'd',
  Up = 'w',
  Down = 's',
  Action = 'z',
  Exit = 'e'
} UserAction_t;
typedef enum {
  Start_st,
  Pause_st,
  Spawn,
  Moving,
  Shifting,
  Game_Over,
  Attaching,
  Exit_st
} states;

typedef struct {
  int **field;
  int next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

extern const figure square;
extern const figure z_ref;
extern const figure z;
extern const figure t;
extern const figure l;
extern const figure l_ref;
extern const figure i_fig;

void init_data();
int stateonground(int fl);
int rotate_able();
void clear_field();
int check_border(int side);
#ifdef __cplusplus
extern "C" {
#endif

GameInfo_t *get_instance();
Point *get_point();
void userInput(UserAction_t action, bool hold);
void machine();
#ifdef __cplusplus
}
#endif
void analyze_field2(int i);
void analyze_field1();
void work_with_field(int a);
void full_clear();
int play_w_file(char *opt, int score);

GameInfo_t updateCurrentState();
void set_info_next();

int st_moving(UserAction_t action, bool hold);
void st_attaching();
void st_shifting();

void handling_start();

void finishprint(WINDOW *win, WINDOW *next, WINDOW *score_);
void welcome();
