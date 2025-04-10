#include "mainwindow.h"

#include "ui_mainwindow.h"
#include <QSharedPointer>

#include "ConfigManager.h"
#include "RemoterGuard.h"

#ifdef MY_DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget* parent_ptr):
    QMainWindow(parent_ptr),
    m_ui_ptr(new Ui::main_window),
    m_remoter_guard_ptr(nullptr)
{
    m_ui_ptr->setupUi(this);

    setupVideo();
}

MainWindow::~MainWindow()
{
    delete m_ui_ptr;
    m_ui_ptr = nullptr;
}

void MainWindow::setRemoterGuard(FEITENG::RemoterGuard* ptr)
{
    if (!m_remoter_guard_ptr)
    {
        m_remoter_guard_ptr = ptr;
        setupRobotRemoteAddr();
    }
    else
    {
#ifdef MY_DEBUG
        qDebug() << "This is a dangerous opearation.";
#endif
    }
}

void MainWindow::updateJoysticksList(const QStringList& joysticks_names,
                                     const int current_joystick_idx)
{
    QComboBox* joysticks_list_ptr = m_ui_ptr->joysticks_combo_box;
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
        m_ui_ptr->left_x_bar->setValue((axes.at(0) + 1) * 50);
        m_ui_ptr->left_y_bar->setValue((axes.at(1) + 1) * 50);
        m_ui_ptr->right_x_bar->setValue((axes.at(2) + 1) * 50);
        m_ui_ptr->right_y_bar->setValue((axes.at(3) + 1) * 50);
    }
    else
    {
        m_ui_ptr->left_x_bar->setValue(0);
        m_ui_ptr->left_y_bar->setValue(0);
        m_ui_ptr->right_x_bar->setValue(0);
        m_ui_ptr->right_y_bar->setValue(0);
    }
    if(buttons.size() > 0)
    {
        m_ui_ptr->button_1_check_box->setChecked(buttons.at(4));
        m_ui_ptr->button_2_check_box->setChecked(buttons.at(5));
    }
    else
    {
        m_ui_ptr->button_1_check_box->setChecked(false);
        m_ui_ptr->button_2_check_box->setChecked(false);
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
    m_ui_ptr->x_label->setText(getAxisStr(RobotAxis::X));
    m_ui_ptr->y_label->setText(getAxisStr(RobotAxis::Y));
    m_ui_ptr->z_label->setText(getAxisStr(RobotAxis::Z));
    m_ui_ptr->roll_label->setText(getAxisStr(RobotAxis::Roll));
    m_ui_ptr->pitch_label->setText(getAxisStr(RobotAxis::Pitch));
    m_ui_ptr->yaw_label->setText(getAxisStr(RobotAxis::Yaw));
    m_ui_ptr->servo_label->setText("servo : " +
        QString::number(robot_data.getActuators().at(0).value.as_float, 'f', 6));
}

void MainWindow::setupRobotRemoteAddr()
{
    connect(m_ui_ptr->host_ip_line_edit, &QLineEdit::editingFinished,
            this, &MainWindow::onRobotRemoteIpEditingFinished);
    connect(m_ui_ptr->host_port_line_edit, &QLineEdit::editingFinished,
            this, &MainWindow::onRobotRemotePortEditingFinished);

    QSharedPointer<FEITENG::ConfigManager> settings_ptr =
        FEITENG::ConfigManager::getInstance();
    m_ui_ptr->host_ip_line_edit->setText(
        settings_ptr->getValue("robot_data_transmitter/remote_ip").toString());
    m_ui_ptr->host_port_line_edit->setText(
        settings_ptr->getValue("robot_data_transmitter/remote_port").toString());
    emit m_ui_ptr->host_ip_line_edit->editingFinished();
    emit m_ui_ptr->host_port_line_edit->editingFinished();
}

void MainWindow::setupVideo()
{
    connect(m_ui_ptr->play_button, &QPushButton::clicked,
            m_ui_ptr->video_widget, &FEITENG::VideoWidget::play);
    connect(m_ui_ptr->pause_button, &QPushButton::clicked,
            m_ui_ptr->video_widget, &FEITENG::VideoWidget::pause);
    connect(m_ui_ptr->refresh_button, &QPushButton::clicked,
            m_ui_ptr->video_widget, &FEITENG::VideoWidget::refresh);
    connect(m_ui_ptr->video_addr_line_edit, &QLineEdit::editingFinished,
            this, &MainWindow::onVideoAddrEditingFinished);

    QSharedPointer<FEITENG::ConfigManager> settings_ptr =
        FEITENG::ConfigManager::getInstance();
    m_ui_ptr->video_addr_line_edit->setText(
        settings_ptr->getValue("video/remote_ip").toString() + ':' +
        settings_ptr->getValue("video/remote_port").toString());
    emit m_ui_ptr->video_addr_line_edit->editingFinished();
}

void MainWindow::onRobotRemoteIpEditingFinished()
{
    if (m_remoter_guard_ptr)
    {
        emit m_remoter_guard_ptr->remoteIpChanged(
            m_ui_ptr->host_ip_line_edit->text());
    }
    m_ui_ptr->host_ip_line_edit->clearFocus();
}

void MainWindow::onRobotRemotePortEditingFinished()
{
    if (m_remoter_guard_ptr)
    {
        emit m_remoter_guard_ptr->remotePortChanged(
            m_ui_ptr->host_port_line_edit->text().toUInt());
    }
    m_ui_ptr->host_port_line_edit->clearFocus();
}

void MainWindow::onVideoAddrEditingFinished()
{
    m_ui_ptr->video_widget->setUrl(m_ui_ptr->video_addr_line_edit->text());
    m_ui_ptr->video_addr_line_edit->clearFocus();
}
