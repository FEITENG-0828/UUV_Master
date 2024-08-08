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
                                    const QList<bool>& buttons)
{
    if(axes.size() >= 4)
    {
        ui_ptr->left_x_bar->setValue((axes.at(0) + 1) * 50);
        ui_ptr->left_y_bar->setValue((axes.at(1) + 1) * 50);
        ui_ptr->right_x_bar->setValue((axes.at(2) + 1) * 50);
        ui_ptr->right_y_bar->setValue((axes.at(3) + 1) * 50);
    }
    else
    {
        ui_ptr->left_x_bar->setValue(0);
        ui_ptr->left_y_bar->setValue(0);
        ui_ptr->right_x_bar->setValue(0);
        ui_ptr->right_y_bar->setValue(0);
    }
    if(buttons.size() >= 4)
    {
        ui_ptr->button_1_check_box->setChecked(buttons.at(0));
        ui_ptr->button_2_check_box->setChecked(buttons.at(1));
    }
    else
    {
        ui_ptr->button_1_check_box->setChecked(false);
        ui_ptr->button_2_check_box->setChecked(false);
    }
}

void MainWindow::updateRobotData()
{
    qDebug()<<"updateRobotData";
    // TODO
}
