#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QTimer>
#include <QWidget>

#include "../../../brick_game/tetris/s21_tetris.h"  // тут твои userInput, printField и т.д.

class GameWindow : public QWidget {
  Q_OBJECT

 public:
  GameWindow(QWidget *parent = nullptr);

 protected:
  void paintEvent(QPaintEvent *event) override;   // отрисовка поля
  void keyPressEvent(QKeyEvent *event) override;  // управление с клавиатуры

 private slots:
  void updateGame();  // слот для таймера

 private:
  QTimer *timer;
  void updateSpeed();
  bool userMoved = false;
};

#endif  // GAMEWINDOW_H
