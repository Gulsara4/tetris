#ifndef HEADER_3_H
#define HEADER_3_H

#include <ncurses/ncurses.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>
namespace s21 {
struct Cell {
  int x;
  int y;
};

class Apple;

class Snake {
 private:
  size_t length_of_snake;

 public:
  std::vector<Cell> body;
  size_t get_length_of_snake() const;
  void increase_length_of_snake();
  Snake(const std::vector<Cell> &cells);

  void addVector(Cell new1);
  void print_body();
  void advanceHead(int des, int des_copy, Cell &newHead);
  void updateTail();
};
class Apple {
 private:
  int x, y;

 public:
  Apple(int x, int y);
  virtual int getX() const;
  virtual int getY() const;
  virtual void relocate(Snake &my_snake);
  virtual ~Apple() = default;
};

static Snake my_snake({{5, 5}, {8, 5}});
static Apple app(0, 0);
Snake &getSnake();
Apple &getApple();
#endif  // HEADER_3_H
}