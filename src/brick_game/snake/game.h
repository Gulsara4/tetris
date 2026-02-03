#include <cstring>
#include <string>

#include "classes_snake_apple.h"
namespace s21 {
#define height1 20
#define width1 10

typedef enum {
  Start = 'g',
  Pause = 'p',
  Terminate = 'q',
  Left = 'a',
  Right = 'd',
  Up = 'w',
  Down = 's',
  Action = 'z',
  Exit = 'e'
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;
typedef enum {

  State_start,
  State_move,
  State_pause,
  State_terminate,
  State_exit

} State;

extern GameInfo_t currentGameState;
extern State currentMachineState;

void userInput(UserAction_t action);
GameInfo_t *updateCurrentState();
State *whichState();
bool isValid(UserAction_t a);

bool processSnakeMove(UserAction_t t);
bool isColliding(const Cell &newHead, Snake &my);
bool checkAppleCollision(Snake &snake, Apple &apple, const Cell &newHead);
inline long long encodeCell(int x, int y);
std::unordered_set<long long> getOccupiedCells(Snake &my);
int getDirectionBetweenVectors(Cell one, Cell two);
Cell getNextHeadPosition(const Cell &currentHead, char direction);
void initialization();
int play_w_file(const char *opt, int score);

void fillField(Snake &snake, const Apple &app);
void print_field(State *st, WINDOW *win1, WINDOW *win2);
void resetField();
void resetDynamicField(int a);
}  // namespace s21