#include "../inc/RemoterGuard.h"

namespace FEITENG
{
    RemoterGuard::RemoterGuard(QObject* parent_ptr): QObject(parent_ptr)
    {
        Controller* controller_ptr =
            new Controller(dynamic_cast<MainWindow*>(parent_ptr));
        controller_ptr->moveToThread(&controller_thread);
        connect(this, &RemoterGuard::setControllerListeningState,
                controller_ptr, &Controller::updateListeningState);
        connect(&controller_thread, &QThread::finished,
                controller_ptr, &QObject::deleteLater);
        controller_thread.start();
        emit setControllerListeningState(Controller::ListeningState::RUNNING);
    }

    RemoterGuard::~RemoterGuard()
    {
        controller_thread.quit();
        controller_thread.wait();
    }
} // namespace FEITENG
