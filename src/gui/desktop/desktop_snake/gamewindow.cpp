// gamewindow.cpp
#include "gamewindow.h"

#include <QKeyEvent>
#include <QPainter>
using namespace s21;
GameWindow::GameWindow(QWidget *parent) : QWidget(parent) {
  setFixedSize(400, 400);  // размер окна
  State *st = whichState();
  GameInfo_t *info = updateCurrentState();
  *st = State_start;
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);

  timer->start(1000);
}
void GameWindow::updateSpeed() {
  GameInfo_t *info = updateCurrentState();

  timer->start(info->speed);
}
void GameWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  State *st = whichState();
  painter.setPen(Qt::black);
  painter.setFont(QFont("Arial", 18));
  int textX = 220;
  int textZ = 20;
  int textY = 30;

  GameInfo_t *info = updateCurrentState();
  if (*st == State_move) {
    painter.drawText(textX, textY, "Score " + QString::number(info->score));
    painter.drawText(textX, textY + 30,
                     "High score " + QString::number(info->high_score));
    painter.drawText(textX, textY + 60,
                     "Level " + QString::number(info->level));
    painter.drawText(textX, textY + 90,
                     "Score " + QString::number(1000 - info->speed));
    painter.drawText(textX, textY + 120,
                     QString("Pause %1").arg(info->pause == 1 ? "on" : "off"));
    int cellSize = 20;

    for (int y = 0; y < 20; ++y) {
      for (int x = 0; x < 10; ++x) {
        if (info->field[y][x] == 1) {
          painter.setBrush(Qt::green);
        } else if (info->field[y][x] == 3) {
          painter.setBrush(Qt::red);
        } else {
          painter.setBrush(Qt::white);
        }

        painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
      }
    }
  } else if (*st == State_start) {
    painter.drawText(textZ, textY, "Start");
    painter.drawText(textZ, textY + 30, "w, a, s, d - управление");
    painter.drawText(textZ, textY + 60, "q - завершить, p - пауза, e - выход ");

  } else if (*st == State_terminate) {
    painter.drawText(textZ, textY, "Game over");

  } else if (*st == State_pause) {
    painter.drawText(textZ, textY, "Pause");
  }
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
  UserAction_t action;
  if (event->key() == Qt::Key_W)
    action = Up;
  else if (event->key() == Qt::Key_S)
    action = Down;
  else if (event->key() == Qt::Key_A)
    action = Left;
  else if (event->key() == Qt::Key_D)
    action = Right;
  else if (event->key() == Qt::Key_P)
    action = Pause;
  else if (event->key() == Qt::Key_Q)
    action = Terminate;
  else if (event->key() == Qt::Key_G)
    action = Start;
  else if (event->key() == Qt::Key_Z) {
    action = Action;
  } else if (event->key() == Qt::Key_E) {
    close();
    return;
  } else
    return;

  userInput(action);
  updateSpeed();
  userMoved = true;  // пользователь сделал ход
  update();
}

void GameWindow::updateGame() {
  State *st = whichState();
  if (*st == State_move) {
    if (!userMoved) {
      userInput(static_cast<UserAction_t>(ERR));
    }
    update();
    userMoved = false;
  }
}
