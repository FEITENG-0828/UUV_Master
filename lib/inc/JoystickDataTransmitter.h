#ifndef ___FEITENG_JOYSTICKDATATRANSMITTER_H
#define ___FEITENG_JOYSTICKDATATRANSMITTER_H

#include "../../mainwindow.h"
#include <QObject>
#include <QUdpSocket>

namespace FEITENG
{
    class JoystickDataTransmitter : public QObject
    {
            Q_OBJECT

        private:
            MainWindow* m_main_window_ptr;

            QString m_host_ip;
            quint16 m_host_port;
            QUdpSocket* m_socket_ptr;

        public:
            JoystickDataTransmitter(MainWindow*);
            ~JoystickDataTransmitter();

        public slots:
            void transmitData(const QList<float>&);
            void changeHostIpByEdit();
            void changeHostPortByEdit();
    };
} // namespace FEITENG

#endif // ___FEITENG_JOYSTICKDATATRANSMITTER_H
