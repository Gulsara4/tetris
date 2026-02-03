#include <gtest/gtest.h>

#include "../../brick_game/snake/game.h"
using namespace s21;
class MockApple : public Apple {
 public:
  MockApple(int x_, int y_) : Apple(x_, y_) {}

  void relocate(Snake &snake) override {}  // ничего не делаем для теста
 private:
  int _x, _y;
};

class MockSnake : public Snake {
 public:
  MockSnake(const std::vector<Cell> &cells) : Snake(cells) {}
};

TEST(MemoryTest, ResetDynamicFieldAllocatesAndFrees) {
  resetDynamicField(0);  // аллокация
  GameInfo_t *info = updateCurrentState();
  EXPECT_NE(info->field, nullptr);
  for (int i = 0; i < height1; i++) {
    EXPECT_NE(info->field[i], nullptr);
  }

  resetDynamicField(1);
}

TEST(AAA, Apple) {
  Apple app = Apple(3, 4);
  EXPECT_EQ(app.getX(), 3);
  EXPECT_EQ(app.getY(), 4);
}
TEST(BBB, Apple) {
  Snake snk = Snake({{5, 9}, {5, 8}});
  EXPECT_EQ(snk.get_length_of_snake(), 2);
  snk.increase_length_of_snake();
  EXPECT_EQ(snk.get_length_of_snake(), 3);
}
TEST(UserFunctions, UserInput1) {
  State *st = whichState();
  *st = State_start;
  userInput(Start);
  EXPECT_EQ(*st, State_move);
  userInput(Up);
  EXPECT_EQ(*st, State_move);
  userInput(Terminate);
  EXPECT_EQ(*st, 1);
}
TEST(SnakeMoveTest, AdvanceHead) {
  int currentDirection = 3;
  int previousDirection = 2;

  Cell newHead = {5, 4};  // имитируем сдвиг влево
  my_snake.advanceHead(currentDirection, previousDirection, newHead);

  EXPECT_EQ(my_snake.body[0].y, 5 - 1);
}
TEST(SnakeMoveTest, UpdateTailDirection2) {
  size_t oldSize = my_snake.body.size();
  my_snake.updateTail();
  // размер не изменится, но последний элемент сдвинется ближе
  EXPECT_EQ(my_snake.body.size(), oldSize);
  EXPECT_EQ(my_snake.body.back().y, 5);
}
TEST(FieldTest, ResetFieldClearsExceptApple) {
  GameInfo_t *info = updateCurrentState();
  // Заполним поле произвольными числами
  resetDynamicField(0);
  for (int i = 0; i < height1; ++i)
    for (int j = 0; j < width1; ++j) info->field[i][j] = 7;

  // Поставим "яблоко" в поле
  info->field[2][3] = 3;

  resetField();

  // Проверяем, что все кроме 3 обнуляются
  for (int i = 0; i < height1; ++i) {
    for (int j = 0; j < width1; ++j) {
      if (i == 2 && j == 3)
        EXPECT_EQ(info->field[i][j], 3);
      else
        EXPECT_EQ(info->field[i][j], 0);
    }
  }
  resetDynamicField(1);
}

// ------------------ Тесты для fillField -------------------
TEST(FieldTest, FillFieldMarksSnakeAndApple) {
  GameInfo_t *info = updateCurrentState();
  resetDynamicField(0);

  std::vector<Cell> snake_cells = {{0, 0}, {0, 1}, {0, 2}};
  MockSnake snake(snake_cells);
  MockApple apple(2, 2);

  fillField(snake, apple);

  // Проверяем тело змеи
  for (const auto &c : snake_cells) {
    EXPECT_EQ(info->field[c.x][c.y], 1);
  }

  // Проверяем яблоко
  EXPECT_EQ(info->field[apple.getX()][apple.getY()], 3);

  resetDynamicField(1);
}

// ------------------ Тесты для isColliding -------------------
TEST(SnakeMoveTest, CollisionDetection) {
  std::vector<Cell> snake_cells = {{0, 0}, {0, 1}};
  MockSnake snake(snake_cells);

  // Проверка выхода за границы
  EXPECT_FALSE(isColliding(Cell{-1, 0}, snake));
  EXPECT_FALSE(isColliding(Cell{height1, 0}, snake));
  EXPECT_FALSE(isColliding(Cell{0, -1}, snake));
  EXPECT_FALSE(isColliding(Cell{0, width1}, snake));

  // Проверка пересечения с телом змеи
  EXPECT_FALSE(isColliding(Cell{0, 0}, snake));
  EXPECT_TRUE(isColliding(Cell{0, 1}, snake));

  // Проверка свободной клетки
  EXPECT_TRUE(isColliding(Cell{1, 1}, snake));
}

// ------------------ Тесты для checkAppleCollision -------------------
TEST(SnakeMoveTest, AppleCollision1) {
  std::vector<Cell> snake_cells = {{0, 0}};
  MockSnake snake(snake_cells);
  MockApple apple(0, 1);
  Cell head = {0, 1};

  bool eaten = checkAppleCollision(snake, apple, head);
  EXPECT_TRUE(eaten);  // яблоко съедено
}
TEST(SnakeMoveTest, AppleCollision2) {
  std::vector<Cell> snake_cells = {{2, 3}, {2, 8}};
  MockSnake snake(snake_cells);
  MockApple apple(1, 3);
  bool eaten = processSnakeMove(Left);

  EXPECT_TRUE(eaten);  // яблоко съедено
}

TEST(UserFunctions, ActionIncreasesSpeed) {
  State *st = whichState();
  GameInfo_t *info = updateCurrentState();
  *st = State_move;
  info->speed = 500;
  userInput(Action);
  EXPECT_LT(info->speed, 500);  // скорость уменьшилась
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // кастомный код
  std::cout << "Tests starting...\n";

  int res = RUN_ALL_TESTS();

  std::cout << "Tests finished.\n";
  atexit([]() {
    std::cout << "EXIT HANDLER\n";
    std::cout.flush();
  });
  return res;
}