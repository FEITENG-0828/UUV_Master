#ifndef ___FEITENG_ROBOTDATATRANSMITTER_H
#define ___FEITENG_ROBOTDATATRANSMITTER_H

#include <QObject>
#include <QUdpSocket>

#include "RobotData.h"

namespace FEITENG
{
    class RobotDataTransmitter : public QObject
    {
            Q_OBJECT

        private:
            QString m_remote_ip;
            quint16 m_remote_port;
            QUdpSocket* m_socket_ptr;

        public:
            explicit RobotDataTransmitter(QObject* = nullptr);
            ~RobotDataTransmitter();

        public slots:
            void transmitData(const RobotData&) const;
            void setRemoteIp(const QString&);
            void setRemotePort(const quint16);
    };
} // namespace FEITENG

#endif // ___FEITENG_ROBOTDATATRANSMITTER_H
