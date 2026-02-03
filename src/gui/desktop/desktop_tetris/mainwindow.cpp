#include "mainwindow.h"

#include <QKeyEvent>
#include <QPainter>

GameWindow::GameWindow(QWidget *parent) : QWidget(parent) {
  setFixedSize(400, 400);  // размер окна
  Point *dot = get_point();
  GameInfo_t *info = get_instance();

  dot->state = Start_st;
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &GameWindow::updateGame);
  int speed = 1000 - (info->level - 1) * 100;
  timer->start(speed);
}
void GameWindow::updateSpeed() {
  GameInfo_t *info = get_instance();
  Point *dot = get_point();
  int newSpeed = 1000 - (info->level - 1) * 100;
  if ((userMoved == true && dot->state == Moving) || dot->state == Attaching ||
      dot->state == Shifting || dot->state == Spawn) {
    newSpeed = 0;
  }
  timer->start(newSpeed);
}
void GameWindow::paintEvent(QPaintEvent *) {
  Point *dot = get_point();
  GameInfo_t *info = get_instance();
  QPainter painter(this);
  painter.setPen(Qt::black);
  painter.setFont(QFont("Arial", 18));
  int textX = 220;
  int textZ = 20;
  int textY = 30;
  int textU = 120;
  if (dot->state == Moving || dot->state == Attaching ||
      dot->state == Shifting || dot->state == Spawn) {
    painter.drawText(textX, textU, "Score " + QString::number(info->score));
    painter.drawText(textX, textU + 30,
                     "High score " + QString::number(info->high_score));
    painter.drawText(textX, textU + 60,
                     "Level " + QString::number(info->level));
    painter.drawText(textX, textU + 90,
                     QString("Pause %1").arg(info->pause == 1 ? "on" : "off"));
    int cellSize = 20;
    for (int y = 0; y < 20; ++y) {
      for (int x = 0; x < 10; ++x) {
        if (info->field[y][x] == 1) {
          painter.setBrush(Qt::green);
        } else if (info->field[y][x] == 2) {
          painter.setBrush(Qt::red);
        } else {
          painter.setBrush(Qt::white);
        }
        painter.drawRect(x * cellSize, y * cellSize, cellSize, cellSize);
      }
    }
    figure fig = *dot->kit[info->next];
    painter.drawText(textX, textY, "next");
    painter.setBrush(Qt::blue);
    for (int i = 0; i < 4; i++) {
      int x_f = fig.rotate[0][i][0];
      int y_f = fig.rotate[0][i][1];
      int p1 = x_f + u1 / 2;
      int p2 = y_f + u2 / 2;
      painter.drawRect((p1 + 10) * cellSize, (p2 - 2) * cellSize, cellSize,
                       cellSize);
    }
  } else if (dot->state == Start_st) {
    painter.drawText(textZ, textY, "Start");
    painter.drawText(textZ, textY + 30, "w, a, s, d - управление");
    painter.drawText(textZ, textY + 60, "p - пауза, o - to start");
  } else if (dot->state == Pause_st) {
    painter.drawText(textZ, textY, "Pause");
    painter.drawText(textZ, textY + 30, "q - завершить игру, e - выход");
  } else if (dot->state == Game_Over) {
    painter.drawText(textZ, textY, "Game over");
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
  else if (event->key() == Qt::Key_O)
    action = Start;
  else if (event->key() == Qt::Key_Z)
    action = Action;
  else if (event->key() == Qt::Key_E) {
    close();
    return;
  } else
    return;

  userInput(action, false);
  userMoved = true;  // пользователь сделал ход
  // updateSpeed();
  update();
}

void GameWindow::updateGame() {
  Point *dot = get_point();

  // if (dot->state==Moving || dot->state==Attaching || dot->state==Shifting ||
  // dot->state==Spawn) {
  //     if (!userMoved) {
  //         userInput(static_cast<UserAction_t>(ERR), false);
  //     }
  //     update();
  //     userMoved = false;
  // }
  if (!userMoved) {
    userInput(static_cast<UserAction_t>(ERR), false);
  }

  userMoved = false;
  update();
}
