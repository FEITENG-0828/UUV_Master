#include "./mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget* parent_ptr):
    QMainWindow(parent_ptr),
    ui_ptr(new Ui::main_window)
{
    ui_ptr->setupUi(this);

    this->setFixedSize(540, 900);
}

MainWindow::~MainWindow()
{
    delete ui_ptr;
    ui_ptr = nullptr;
}

void MainWindow::updateJoysticksList(const QStringList& joysticks_names,
                                     const int current_joystick_idx)
{
    QComboBox* joysticks_list_ptr = ui_ptr->joysticks_combo_box;
    joysticks_list_ptr->clear();
    joysticks_list_ptr->addItems(joysticks_names);
    joysticks_list_ptr->setCurrentIndex(current_joystick_idx);
}

void MainWindow::updateJoystickData(const QList<double>& axes,
                                    const QList<bool>& buttons,
                                    const QList<int>& povs)
{
    qDebug()<<axes;
    qDebug()<<buttons;
    qDebug()<<povs;
}

void MainWindow::updateRobotData()
{
    qDebug()<<"updateRobotData";
    // TODO
}
