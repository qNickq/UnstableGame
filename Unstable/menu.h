#pragma once
#include <QObject>
#include <QtWidgets>

#include "game.h"

class Menu : public QWidget
{
    Q_OBJECT

public:
    Menu(QWidget * parent  = nullptr);
public slots:
    void startGame();
private:
    QPushButton  startgame;
    QPushButton  quit;

};


