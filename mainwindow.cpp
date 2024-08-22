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

void MainWindow::updateRobotData(const QList<float>& robot_data)
{
    ui_ptr->x_label->setText("x : " + QString::number(robot_data.at(0), 'f', 2));
    ui_ptr->y_label->setText("y : " + QString::number(robot_data.at(1), 'f', 2));
    ui_ptr->z_label->setText("z : " + QString::number(robot_data.at(2), 'f', 2));
    ui_ptr->roll_label->setText("roll : " + QString::number(robot_data.at(3), 'f', 2));
    ui_ptr->pitch_label->setText("pitch : " + QString::number(robot_data.at(4), 'f', 2));
    ui_ptr->yaw_label->setText("yaw : " + QString::number(robot_data.at(5), 'f', 2));
    ui_ptr->servo_label->setText("servo : " + QString::number(robot_data.at(6), 'f', 6));
}
