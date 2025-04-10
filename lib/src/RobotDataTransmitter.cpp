#include "RobotDataTransmitter.h"

#include <QTimer>
#include <QSharedPointer>

#include "Constants.h"
#include "RobotDataFormatter.h"

namespace FEITENG
{
    RobotDataTransmitter::RobotDataTransmitter(QObject* parent_ptr): QObject(parent_ptr)
    {
        m_remote_ip.clear();
        m_remote_port = 0;
        m_socket_ptr = new QUdpSocket(this);
    }

    RobotDataTransmitter::~RobotDataTransmitter()
    { }

    void RobotDataTransmitter::transmitData(const RobotData& robot_data) const
    {
        // static QSharedPointer<RobotDataFormatter> formatter_ptr =
        //     RobotDataFormatterFactory::createFormatter(
        //         Constants::ROBOTDATA_FORMAT_TYPE,
        //         const_cast<RobotDataTransmitter*>(this));
        // FIXME: pure virtual method called
        if(!m_remote_ip.isEmpty() && m_remote_port != 0)
        {
            QSharedPointer<RobotDataFormatter> formatter_ptr =
                RobotDataFormatterFactory::createFormatter(
                    Constants::ROBOTDATA_FORMAT_TYPE,
                    const_cast<RobotDataTransmitter*>(this));
            QByteArray datagram = formatter_ptr->serializeData(robot_data);
            if(!datagram.isEmpty())
            {
                m_socket_ptr->writeDatagram(datagram,
                                            QHostAddress(m_remote_ip),
                                            m_remote_port);
            }
        }
    }

    void RobotDataTransmitter::setRemoteIp(const QString& new_ip)
    {
        m_remote_ip = new_ip;
    }

    void RobotDataTransmitter::setRemotePort(const quint16 new_port)
    {
        m_remote_port = new_port;
    }
} // namespace FEITENG
