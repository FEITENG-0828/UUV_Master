#include "../inc/RemoterGuard.h"
#include "../inc/JoystickDataTransmitter.h"

namespace FEITENG
{
    RemoterGuard::RemoterGuard(QObject* parent_ptr): QObject(parent_ptr)
    {
        Controller* controller_ptr =
            new Controller(dynamic_cast<MainWindow*>(parent_ptr));
        controller_ptr->moveToThread(&m_controller_thread);
        connect(this, &RemoterGuard::setControllerListeningState,
                controller_ptr, &Controller::updateListeningState);
        connect(&m_controller_thread, &QThread::finished,
                controller_ptr, &QObject::deleteLater);
        m_controller_thread.start();

        JoystickDataTransmitter* joystickdatatransmitter_ptr =
                new JoystickDataTransmitter(dynamic_cast<MainWindow*>(parent_ptr));
        joystickdatatransmitter_ptr->moveToThread(&m_joystickdatatransmitter_thread);
        connect(controller_ptr, &Controller::robotDataSended,
                joystickdatatransmitter_ptr, &JoystickDataTransmitter::transmitData);
        connect(&m_joystickdatatransmitter_thread, &QThread::finished,
                joystickdatatransmitter_ptr, &QObject::deleteLater);
        m_joystickdatatransmitter_thread.start();

        emit setControllerListeningState(Controller::ListeningState::RUNNING);
    }

    RemoterGuard::~RemoterGuard()
    {
        m_controller_thread.quit();
        m_controller_thread.wait();

        m_joystickdatatransmitter_thread.quit();
        m_joystickdatatransmitter_thread.wait();
    }
} // namespace FEITENG
