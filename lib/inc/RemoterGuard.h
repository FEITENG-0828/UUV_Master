#ifndef ___FEITENG_REMOTERGUARD_H
#define ___FEITENG_REMOTERGUARD_H

#include <QObject>
#include <QThread>

#include "./Controller.h"

namespace FEITENG
{
    class RemoterGuard : public QObject
    {
            Q_OBJECT

        private:
            QThread m_controller_thread;
            QThread m_joystickdatatransmitter_thread;

        public:
            RemoterGuard(QObject* = nullptr);
            ~RemoterGuard();

        signals:
            // 自定义参数类型名一定要写完整
            void setControllerListeningState(const FEITENG::Controller::ListeningState&);
    };
} // namespace FEITENG

#endif // ___FEITENG_REMOTERGUARD_H
