#include "VideoCoreThread.h"

#include <QtEndian>

#ifdef MY_DEBUG
#include <QDebug>
#endif

namespace FEITENG
{
    VideoCoreThread::VideoCoreThread(QObject* parent_ptr)
        : QThread(parent_ptr),
          m_rtc_client_ptr(nullptr),
          m_arguments(),
          m_pipe_handle(INVALID_HANDLE_VALUE),
          m_state(State::NOT_STARTED),
          m_mutex(),
          m_connect_timer(nullptr),
          m_disconnect_timer(nullptr),
          m_read_timer(nullptr)
    { }

    VideoCoreThread::~VideoCoreThread()
    { }

    void VideoCoreThread::connectServer()
    {
        if(m_rtc_client_ptr || m_pipe_handle != INVALID_HANDLE_VALUE)
        {
            disconnectServer();
        }

        m_rtc_client_ptr = new QProcess();
        m_rtc_client_ptr->setWorkingDirectory("./");
        m_rtc_client_ptr->setProgram("./rtc_client/rtc_client.exe");
        m_rtc_client_ptr->setArguments(m_arguments);
        m_rtc_client_ptr->start();
        if(!m_rtc_client_ptr->waitForStarted())
        {
#ifdef MY_DEBUG
            qDebug() << "Failed to start rtc_client.";
#endif
            restart();
            return;
        }
        if(!m_rtc_client_ptr->waitForReadyRead())
        {
#ifdef MY_DEBUG
            qDebug() << "Failed to get pipe_ready signal.";
#endif
        }

        if(!WaitNamedPipe(TEXT(R"(\\.\pipe\video_pipe)"), NMPWAIT_WAIT_FOREVER))
        {
#ifdef MY_DEBUG
            qDebug() << "Failed to wait pipe. Error: " << GetLastError();
#endif
        }
        m_pipe_handle = CreateFile(TEXT(R"(\\.\pipe\video_pipe)"), // pipe name
            GENERIC_READ,                                          // read and write access
            0,                                                     // no sharing
            NULL,                                                  // default security attributes
            OPEN_EXISTING,                                         // opens existing pipe
            0,                                                     // default attributes
            NULL);                                                 // no template file
        if(m_pipe_handle == INVALID_HANDLE_VALUE)
        {
#ifdef MY_DEBUG
            qDebug() << "Failed to open pipe. Error: " << GetLastError();
#endif
            restart();
            return;
        }

        QMutexLocker locker(&m_mutex);
        if(m_state == State::NOT_STARTED || m_state == State::REBOOTING)
        {
            m_state = State::ACTIVE;
        }
        locker.unlock();

        m_read_timer->start(8);
    }

    void VideoCoreThread::disconnectServer()
    {
        m_read_timer->stop();

        if(m_rtc_client_ptr)
        {
            // 注释此代码块也能运行（不建议）
            m_rtc_client_ptr->write("close\n");
            m_rtc_client_ptr->waitForBytesWritten();
        }

        if(m_pipe_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_pipe_handle);
            m_pipe_handle = INVALID_HANDLE_VALUE;
        }
        if(m_rtc_client_ptr)
        {
            m_rtc_client_ptr->waitForFinished();
            delete m_rtc_client_ptr;
            m_rtc_client_ptr = nullptr;
        }

        QMutexLocker locker(&m_mutex);
        if(m_state == State::REBOOTING)
        {
            locker.unlock();
            m_connect_timer->start();
        }
    }

    void VideoCoreThread::readFrame()
    {
        if(m_state != State::ACTIVE)
        {
            return;
        }
        if(!m_pipe_handle || m_pipe_handle == INVALID_HANDLE_VALUE)
        {
            restart();
            return;
        }
        DWORD bytes_read = -1;
        quint32 frame_size = -1;
        QByteArray image_data;
        if(ReadFile(m_pipe_handle, &frame_size, sizeof(frame_size), &bytes_read, NULL))
        {
            if(bytes_read != sizeof(frame_size))
            {
                return;
            }
            frame_size = qFromBigEndian(frame_size);
            image_data.resize(frame_size);
            if(ReadFile(m_pipe_handle, image_data.data(), frame_size, &bytes_read, NULL))
            {
                if(bytes_read != frame_size)
                {
                    return;
                }
                emit sendLatestFrame(QImage::fromData(image_data, "JPG"));
            }
        }
    }

    void VideoCoreThread::proceed()
    {
        QMutexLocker locker(&m_mutex);
        if(m_state == State::INACTIVE)
        {
            m_state = State::ACTIVE;
        }
    }

    void VideoCoreThread::suspend()
    {
        QMutexLocker locker(&m_mutex);
        if(m_state == State::ACTIVE)
        {
            m_state = State::INACTIVE;
        }
    }

    void VideoCoreThread::restart()
    {
        QMutexLocker locker(&m_mutex);
        if(m_state != State::SHUTDOWN)
        {
            m_state = State::REBOOTING;
        }
        QMetaObject::invokeMethod(m_disconnect_timer, "start");
    }

    void VideoCoreThread::stopRunning()
    {
        QMutexLocker locker(&m_mutex);
        m_state = State::SHUTDOWN;
    }

    void VideoCoreThread::updateStreamAddr(const QString& address)
    {
        int index = address.lastIndexOf(':');
        QString ip = (index == -1) ?
            address : (index == 0) ?
                "127.0.0.1" : address.left(index);
        QString port = (index >= 0 && index < address.size() - 1) ?
            address.mid(index + 1) : "20000";

        m_arguments = QStringList({"-s", ip, "-p", port});
        restart();
    }

    void VideoCoreThread::run()
    {
        m_connect_timer = new QTimer();
        m_connect_timer->setSingleShot(true);
        m_connect_timer->callOnTimeout(
            this, &VideoCoreThread::connectServer, Qt::DirectConnection);

        m_disconnect_timer = new QTimer();
        m_disconnect_timer->setSingleShot(true);
        m_disconnect_timer->callOnTimeout(
            this, &VideoCoreThread::disconnectServer, Qt::DirectConnection);

        m_read_timer = new QTimer();
        m_read_timer->callOnTimeout(
            this, &VideoCoreThread::readFrame, Qt::DirectConnection);

        m_connect_timer->start();

        exec();

        disconnectServer();

        m_connect_timer->stop();
        delete m_connect_timer;
        m_connect_timer = nullptr;
        m_disconnect_timer->stop();
        delete m_disconnect_timer;
        m_disconnect_timer = nullptr;
        m_read_timer->stop();
        delete m_read_timer;
        m_read_timer = nullptr;
    }
} // namespace FEITENG
