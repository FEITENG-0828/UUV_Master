#ifndef ___FEITENG_REMOTERGUARD_H
#define ___FEITENG_REMOTERGUARD_H

#include "mainwindow.h"
#include <QObject>
#include <QThread>

namespace FEITENG
{
    class RemoterGuard : public QObject
    {
            Q_OBJECT

        private:
            MainWindow* m_main_window_ptr;

            QThread m_controller_thread;
            QThread m_robotdatatransmitter_thread;

        public:
            RemoterGuard(QObject* = nullptr);
            ~RemoterGuard();

        signals:
            void hostIpEdited(const QString&);
            void hostPortEdited(const quint16);

        public slots:
            void onHostIpEditingFinished();
            void onHostPortEditingFinished();
    };
} // namespace FEITENG

#endif // ___FEITENG_REMOTERGUARD_H
