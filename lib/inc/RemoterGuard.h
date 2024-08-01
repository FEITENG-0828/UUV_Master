#ifndef ___FEITENG_REMOTERGUARD_H
#define ___FEITENG_REMOTERGUARD_H

#include "../../mainwindow.h"
#include <QObject>
#include <QThread>

#include "./Controller.h"

namespace FEITENG
{
    class RemoterGuard : public QObject
    {
            Q_OBJECT

        private:
            QThread controller_thread;

        public:
            RemoterGuard(MainWindow*);
            ~RemoterGuard();

        signals:
            // 自定义参数类型名一定要写完整
            void setControllerListeningState(const FEITENG::Controller::ListeningState&);
    };
} // namespace FEITENG

#endif // ___FEITENG_REMOTERGUARD_H
