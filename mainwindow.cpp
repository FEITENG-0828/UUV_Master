#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QSharedPointer>

#include "ConfigManager.h"

MainWindow::MainWindow(QWidget* parent_ptr):
    QMainWindow(parent_ptr),
    ui_ptr(new Ui::main_window)
{
    ui_ptr->setupUi(this);

    connect(ui_ptr->play_button, &QPushButton::clicked,
            ui_ptr->video_widget, &FEITENG::VideoWidget::play);
    connect(ui_ptr->pause_button, &QPushButton::clicked,
            ui_ptr->video_widget, &FEITENG::VideoWidget::pause);
    connect(ui_ptr->refresh_button, &QPushButton::clicked,
            ui_ptr->video_widget, &FEITENG::VideoWidget::refresh);
    connect(ui_ptr->video_addr_line_edit, &QLineEdit::editingFinished,
            this, &MainWindow::onVideoAddrEditingFinished);

    QSharedPointer<FEITENG::ConfigManager> settings_ptr = FEITENG::ConfigManager::getInstance();
    ui_ptr->video_addr_line_edit->setText(
        settings_ptr->getValue("video/remote_ip").toString() + ':' +
        settings_ptr->getValue("video/remote_port").toString());
    emit ui_ptr->video_addr_line_edit->editingFinished();
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
                                    [[maybe_unused]] const QList<int>& povs)
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
    if(buttons.size() > 0)
    {
        ui_ptr->button_1_check_box->setChecked(buttons.at(4));
        ui_ptr->button_2_check_box->setChecked(buttons.at(5));
    }
    else
    {
        ui_ptr->button_1_check_box->setChecked(false);
        ui_ptr->button_2_check_box->setChecked(false);
    }
}

void MainWindow::updateRobotData(const FEITENG::RobotData& robot_data)
{
    using RobotAxis = FEITENG::RobotData::AxisType;

    auto getAxisStr = [&robot_data](const RobotAxis axis_type) -> QString
    {
        static const QVector<QString> axis_name
            = {"x", "y", "z", "roll", "pitch", "yaw"};
        return QString(R"(")") +
               axis_name.at(static_cast<quint8>(axis_type)) +
               R"(":)" +
               QString::number(robot_data.getAxis(axis_type), 'f', 2);
    };
    ui_ptr->x_label->setText(getAxisStr(RobotAxis::X));
    ui_ptr->y_label->setText(getAxisStr(RobotAxis::Y));
    ui_ptr->z_label->setText(getAxisStr(RobotAxis::Z));
    ui_ptr->roll_label->setText(getAxisStr(RobotAxis::Roll));
    ui_ptr->pitch_label->setText(getAxisStr(RobotAxis::Pitch));
    ui_ptr->yaw_label->setText(getAxisStr(RobotAxis::Yaw));
    ui_ptr->servo_label->setText("servo : " +
        QString::number(robot_data.getActuators().at(0).value.as_float, 'f', 6));
}

void MainWindow::updateHostIp(const QString& new_ip)
{
    ui_ptr->host_ip_line_edit->setText(new_ip);
}

void MainWindow::updateHostPort(const quint16 new_port)
{
    ui_ptr->host_port_line_edit->setText(QString::number(new_port));
}

void MainWindow::onVideoAddrEditingFinished()
{
    ui_ptr->video_widget->setUrl(ui_ptr->video_addr_line_edit->text());
    ui_ptr->video_addr_line_edit->clearFocus();
}
