#include "RemoterGuard.h"

#include <QComboBox>
#include <QLineEdit>

#include "JoysticksProxy.h"
#include "Controller.h"
#include "RobotDataTransmitter.h"

namespace FEITENG
{
    RemoterGuard::RemoterGuard(QObject* parent_ptr): QObject(parent_ptr)
    {
        m_main_window_ptr = dynamic_cast<MainWindow*>(parent_ptr);

        Controller* controller_ptr = new Controller();

        JoysticksProxy* joysticks_proxy_ptr = new JoysticksProxy(controller_ptr);
        connect(joysticks_proxy_ptr, &JoysticksProxy::devicesChanged,
                m_main_window_ptr, &MainWindow::updateJoysticksList);
        connect(m_main_window_ptr->findChild<const QComboBox*>("joysticks_combo_box"),
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

        connect(this, &RemoterGuard::remoteIpChanged,
                robotdatatransmitter_ptr, &RobotDataTransmitter::setRemoteIp);
        connect(this, &RemoterGuard::remotePortChanged,
                robotdatatransmitter_ptr, &RobotDataTransmitter::setRemotePort);
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
} // namespace FEITENG
