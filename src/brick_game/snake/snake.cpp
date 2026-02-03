
#include "game.h"
namespace s21 {
GameInfo_t currentGameState;
State currentMachineState;
GameInfo_t *updateCurrentState() {
  static GameInfo_t params;
  return &params;
}
State *whichState() {
  static State params;
  return &params;
}
int getDirectionBetweenVectors(Cell one, Cell two) {
  int result = -1;
  if (one.y == two.y) {
    if (one.x < two.x)
      result = 0;
    else
      result = 1;
  }
  if (one.x == two.x) {
    if (one.y < two.y)
      result = 2;
    else
      result = 3;
  }
  return result;
}
inline long long encodeCell(int x, int y) {
  return (static_cast<long long>(x) << 32) | (static_cast<unsigned int>(y));
}
std::unordered_set<long long> getOccupiedCells(Snake &my) {
  std::unordered_set<long long> occupied;
  for (const auto &cell : my.body) {
    occupied.insert(encodeCell(cell.x, cell.y));
  }
  int l = my.body.size();
  for (int i = 0; i < l - 1; i++) {
    int currentDirection =
        getDirectionBetweenVectors(my.body[i], my.body[i + 1]);
    Cell one = my.body[i], two = my.body[i + 1];
    if (currentDirection == 1 || currentDirection == 3) std::swap(one, two);
    if (currentDirection == 0 || currentDirection == 1) {
      for (int j = one.x; j <= two.x; j++)
        occupied.insert(encodeCell(j, one.y));
    } else {
      for (int j = one.y; j <= two.y; j++)
        occupied.insert(encodeCell(one.x, j));
    }
  }
  return occupied;
}

Cell getNextHeadPosition(const Cell &currentHead, char direction) {
  Cell newHead = currentHead;
  if (direction == Left) newHead.y -= 1;
  if (direction == Right) newHead.y += 1;
  if (direction == Up) newHead.x -= 1;
  if (direction == Down) newHead.x += 1;
  return newHead;
}

bool isColliding(const Cell &newHead, Snake &my) {
  if (newHead.x < 0 || newHead.x > height1 - 1 || newHead.y < 0 ||
      newHead.y > width1 - 1) {
    return false;
  }
  if (newHead.x == my.body[my.body.size() - 1].x &&
      newHead.y == my.body[my.body.size() - 1].y) {
    return true;
  }
  long long code = encodeCell(newHead.x, newHead.y);
  auto occupied = getOccupiedCells(my);
  for (long long c : occupied) {
    if (c == code) {
      return false;  // клетка занята
    }
  }
  return true;
}
bool checkAppleCollision(Snake &snake, Apple &apple, const Cell &newHead) {
  bool flag = false;
  if (newHead.x == apple.getX() && newHead.y == apple.getY()) {
    apple.relocate(snake);
    flag = true;  // съели яблоко
  }
  return flag;
}

bool processSnakeMove(UserAction_t t) {
  Snake &my = getSnake();
  Apple &app = getApple();
  GameInfo_t *info = updateCurrentState();
  bool flag_colliding;
  int previousDirection = getDirectionBetweenVectors(my.body[0], my.body[1]);
  if ((previousDirection == 0 && t == Down)) {
    t = Up;
  } else if (previousDirection == 1 && t == Up) {
    t = Down;
  } else if (previousDirection == 2 && t == Right) {
    t = Left;
  } else if (previousDirection == 3 && t == Left) {
    t = Right;
  }
  Cell newHead = getNextHeadPosition(my.body[0], t);
  if (isColliding(newHead, my) == true) {
    flag_colliding = true;
    bool appleCollision = checkAppleCollision(my, app, newHead);
    int currentDirection = getDirectionBetweenVectors(newHead, my.body[0]);
    my.advanceHead(currentDirection, previousDirection, newHead);
    if (appleCollision == false) {
      my.updateTail();
    } else {
      my.increase_length_of_snake();
      info->score = my.get_length_of_snake() - 4;
      if (info->score > info->high_score) {
        info->high_score = info->score;
        play_w_file("w", info->high_score);
      }
      if (info->level != (int)info->score / 5 + 1) {
        if (info->level < 10) {
          info->level = (int)info->score / 5 + 1;
        }
        if (info->speed > 100) {
          info->speed = info->speed - 100;
        }
      }
    }
    resetField();
    fillField(my, app);
  } else {
    flag_colliding = false;
  }
  return flag_colliding;
}
void initialization() {
  Snake &snake = getSnake();
  snake = Snake({{5, 5}, {8, 5}});
  Apple &app = getApple();
  GameInfo_t *info = updateCurrentState();
  resetDynamicField(0);
  app.relocate(snake);
  fillField(snake, app);
  info->score = snake.get_length_of_snake() - 4;
  info->high_score = play_w_file("r", 0);
  info->level = 1;
  info->speed = 1000;
}
int play_w_file(const char *opt, int score) {
  int res = 0;
  FILE *fp = fopen("highscore_snake.txt", opt);
  char str[100];
  if (strcmp(opt, "r") == 0 && fp) {
    fgets(str, 100, fp);
    res = atoi(str);
  } else if (strcmp(opt, "w") == 0 && fp) {
    snprintf(str, sizeof(str), "%d", score);
    fputs(str, fp);
  }
  if (fp) fclose(fp);
  return res;
}
Snake &getSnake() { return my_snake; }
Snake::Snake(const std::vector<Cell> &cells) : body(cells) {
  length_of_snake = getOccupiedCells(*this).size();
}
void Snake::addVector(Cell new1) {
  std::vector<Cell> copy = {new1};
  copy.insert(copy.end(), body.begin(), body.end());
  body = copy;
}
void Snake::advanceHead(int currentDirection, int previousDirection,
                        Cell &newHead) {
  if (previousDirection != currentDirection)  // добавляем вектор
  {
    addVector(newHead);
  } else {
    if (currentDirection == 0)
      body[0].x -= 1;
    else if (currentDirection == 1)
      body[0].x += 1;
    else if (currentDirection == 2)
      body[0].y -= 1;
    else if (currentDirection == 3)
      body[0].y += 1;
  }
}
void Snake::updateTail() {
  size_t bodySize = body.size();  // убираем посл клетку или переносим
  if (abs(body[bodySize - 2].x - body[bodySize - 1].x) +
          abs(body[bodySize - 2].y - body[bodySize - 1].y) ==
      1) {
    body.pop_back();
  } else {
    int currentDirection =
        getDirectionBetweenVectors(body[bodySize - 2], body[bodySize - 1]);
    if (currentDirection == 0) {
      body[bodySize - 1].x -= 1;
    } else if (currentDirection == 1) {
      body[bodySize - 1].x += 1;
    } else if (currentDirection == 2) {
      body[bodySize - 1].y -= 1;
    } else if (currentDirection == 3) {
      body[bodySize - 1].y += 1;
    }
  }
}
size_t Snake::get_length_of_snake() const { return length_of_snake; }
void Snake::increase_length_of_snake() { length_of_snake++; }
}  // namespace s21