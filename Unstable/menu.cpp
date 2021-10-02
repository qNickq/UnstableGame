#include "menu.h"

Menu::Menu(QWidget *parent)
{
    QVBoxLayout * v_box = new QVBoxLayout(this);
    this->setLayout(v_box);

    setWindowTitle("Unstable");
    setFixedSize(300, 300);

    startgame.setText("&Start game");
    quit.setText("&Exit");

    v_box->addWidget(&startgame);
    v_box->addWidget(&quit);

    connect(&startgame, &QPushButton::clicked, this, &Menu::startGame);
    connect(&quit, &QPushButton::clicked, this, &QWidget::close);

    Q_UNUSED(parent);
}

void Menu::startGame()
{
    Game * game  = new Game(this);
    this->close();
    game->show();
}
