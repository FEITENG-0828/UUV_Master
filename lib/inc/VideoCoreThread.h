#ifndef ___FEITENG_VIDEOCORETHREAD_H
#define ___FEITENG_VIDEOCORETHREAD_H

#include <QThread>
#include <QProcess>
#include <QRecursiveMutex>
#include <QTimer>
#include <QImage>

#include "windows.h"

namespace FEITENG
{
    class VideoCoreThread : public QThread
    {
            Q_OBJECT

        public:
            enum class State : quint8
            {
                NOT_STARTED = 0, ACTIVE = 1, INACTIVE = 2, REBOOTING = 3, SHUTDOWN = 4
            };

        private:
            QProcess* m_rtc_client_ptr;
            QStringList m_arguments;
            HANDLE m_pipe_handle;
            State m_state;
            QRecursiveMutex m_mutex;
            QTimer* m_connect_timer;
            QTimer* m_disconnect_timer;
            QTimer* m_read_timer;

            void connectServer();
            Q_INVOKABLE void disconnectServer();
            void readFrame();

        public:
            explicit VideoCoreThread(QObject* = nullptr);
            ~VideoCoreThread();

            void proceed();
            void suspend();
            void restart();

            void stopRunning();

            void updateStreamAddr(const QString&);

        protected:
            void run() override;

        signals:
            void sendLatestFrame(const QImage&);
    };
};

#endif // ___FEITENG_VIDEOCORETHREAD_H
