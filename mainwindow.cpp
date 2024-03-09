#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(positionUpdate(int,int)), ui->widget, SLOT(onPositionUpdate(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_generateBtn_clicked()
{
    static int x = 0;
    static int y = 0;
    // x++;
   y++;
    emit positionUpdate(x, y);
}


