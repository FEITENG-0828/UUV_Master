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
            bool m_be_inited;

            QString m_host_ip;
            quint16 m_host_port;
            QUdpSocket* m_socket_ptr;

        public:
            RobotDataTransmitter(QObject* = nullptr);
            ~RobotDataTransmitter();

        signals:
            void hostIpChanged(const QString&);
            void hostPortChanged(const quint16);

        private slots:
            void init();

        public slots:
            void transmitData(const RobotData&) const;
            void setHostIp(const QString&);
            void setHostPort(const quint16);
    };
} // namespace FEITENG

#endif // ___FEITENG_ROBOTDATATRANSMITTER_H
