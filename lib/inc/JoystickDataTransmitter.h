#ifndef ___FEITENG_JOYSTICKDATATRANSMITTER_H
#define ___FEITENG_JOYSTICKDATATRANSMITTER_H

#include <QObject>
#include <QUdpSocket>

namespace FEITENG
{
    class JoystickDataTransmitter : public QObject
    {
            Q_OBJECT

        private:
            QString m_host_ip;
            quint16 m_host_port;
            QUdpSocket* m_socket_ptr;

        public:
            JoystickDataTransmitter(QObject* = nullptr);
            ~JoystickDataTransmitter();

        public slots:
            void transmitData(const QList<float>&);
    };
} // namespace FEITENG

#endif // ___FEITENG_JOYSTICKDATATRANSMITTER_H
