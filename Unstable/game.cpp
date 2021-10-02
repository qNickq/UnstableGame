#include "game.h"
#include "ui_mainwindow.h"

Game::Game(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

Game::~Game()
{
    delete ui;
}

