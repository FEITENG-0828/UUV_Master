#include "../inc/RemoterGuard.h"

#include <QComboBox>
#include <QLineEdit>

#include "../inc/JoysticksProxy.h"
#include "../inc/Controller.h"
#include "../inc/RobotDataTransmitter.h"

namespace FEITENG
{
    RemoterGuard::RemoterGuard(QObject* parent_ptr): QObject(parent_ptr)
    {
        m_main_window_ptr = dynamic_cast<MainWindow*>(parent_ptr);

        Controller* controller_ptr = new Controller();

        JoysticksProxy* joysticks_proxy_ptr = new JoysticksProxy(controller_ptr);
        connect(joysticks_proxy_ptr, &JoysticksProxy::devicesChanged,
                m_main_window_ptr, &MainWindow::updateJoysticksList);
        connect(m_main_window_ptr->findChildControl<QComboBox*>("joysticks_combo_box"),
                &QComboBox::currentTextChanged,
                joysticks_proxy_ptr,
                &JoysticksProxy::adjustByChoice);
        controller_ptr->setJoysticksProxy(joysticks_proxy_ptr);

        connect(controller_ptr, &Controller::joystickDataSended,
                m_main_window_ptr, &MainWindow::updateJoystickData);
        connect(controller_ptr, &Controller::robotDataSended,
                m_main_window_ptr, &MainWindow::updateRobotData);

        controller_ptr->moveToThread(&m_controller_thread);
        connect(&m_controller_thread, &QThread::finished,
                controller_ptr, &QObject::deleteLater);
        m_controller_thread.start();

        RobotDataTransmitter* robotdatatransmitter_ptr = new RobotDataTransmitter();

        connect(robotdatatransmitter_ptr, &RobotDataTransmitter::hostIpChanged,
                m_main_window_ptr, &MainWindow::updateHostIp);
        connect(robotdatatransmitter_ptr, &RobotDataTransmitter::hostPortChanged,
                m_main_window_ptr, &MainWindow::updateHostPort);
        connect(m_main_window_ptr->findChildControl<QLineEdit*>("host_ip_line_edit"),
                &QLineEdit::editingFinished,
                this,
                &RemoterGuard::onHostIpEditingFinished);
        connect(this, &RemoterGuard::hostIpEdited,
                robotdatatransmitter_ptr, &RobotDataTransmitter::setHostIp);
        connect(m_main_window_ptr->findChildControl<QLineEdit*>("host_port_line_edit"),
                &QLineEdit::editingFinished,
                this,
                &RemoterGuard::onHostPortEditingFinished);
        connect(this, &RemoterGuard::hostPortEdited,
                robotdatatransmitter_ptr, &RobotDataTransmitter::setHostPort);
        connect(controller_ptr, &Controller::robotDataSended,
                robotdatatransmitter_ptr, &RobotDataTransmitter::transmitData);

        robotdatatransmitter_ptr->moveToThread(&m_robotdatatransmitter_thread);
        connect(&m_robotdatatransmitter_thread, &QThread::finished,
                robotdatatransmitter_ptr, &QObject::deleteLater);
        m_robotdatatransmitter_thread.start();
    }

    RemoterGuard::~RemoterGuard()
    {
        m_controller_thread.quit();
        m_controller_thread.wait();

        m_robotdatatransmitter_thread.quit();
        m_robotdatatransmitter_thread.wait();
    }

    void RemoterGuard::onHostIpEditingFinished()
    {
        emit hostIpEdited(m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_ip_line_edit")
            ->text());
    }

    void RemoterGuard::onHostPortEditingFinished()
    {
        emit hostPortEdited(m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_port_line_edit")
            ->text().toULongLong());
    }
} // namespace FEITENG
