#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include "QJoysticks.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QJoysticks* instance = QJoysticks::getInstance();
    instance->setVirtualJoystickEnabled(false);

    QPushButton * btn = new QPushButton(this);
    btn->setText("test");
    btn->move(100,100);
    connect(btn,&QPushButton::clicked,this,&MainWindow::updateJS);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::updateJS()
{
    QJoysticks* instance = QJoysticks::getInstance();

    qDebug()<<instance->nonBlacklistedCount();
    qDebug()<<instance->deviceNames();
}
