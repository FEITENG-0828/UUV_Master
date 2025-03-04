#include "RobotDataTransmitter.h"

#include <QTimer>

#include "RobotDataFormatter.h"

namespace FEITENG
{
    RobotDataTransmitter::RobotDataTransmitter(QObject* parent_ptr): QObject(parent_ptr)
    {
        m_be_inited = false;

        m_host_ip.clear();
        m_host_port = -1;
        m_socket_ptr = new QUdpSocket(this);

        QTimer::singleShot(0, this, &RobotDataTransmitter::init);
    }

    RobotDataTransmitter::~RobotDataTransmitter()
    { }

    void RobotDataTransmitter::init()
    {
        if(!m_be_inited)
        {
            m_be_inited = true;

            setHostIp("192.168.11.254");
            setHostPort(8888);
        }
    }

    void RobotDataTransmitter::transmitData(const RobotData& robot_data) const
    {
        if(m_be_inited)
        {
            QSharedPointer<RobotDataFormatter> formatter_ptr =
                RobotDataFormatterFactory::createFormatter(
                    RobotDataFormatterFactory::FormatType::Usart,
                    const_cast<RobotDataTransmitter*>(this));
            m_socket_ptr->writeDatagram(formatter_ptr->serializeData(robot_data),
                                        QHostAddress(m_host_ip),
                                        m_host_port);
        }
    }

    void RobotDataTransmitter::setHostIp(const QString& new_ip)
    {
        if(m_host_ip != new_ip)
        {
            m_host_ip = new_ip;
            emit hostIpChanged(m_host_ip);
        }
    }

    void RobotDataTransmitter::setHostPort(const quint16 new_port)
    {
        if(m_host_port != new_port)
        {
            m_host_port = new_port;
            emit hostPortChanged(m_host_port);
        }
    }
} // namespace FEITENG
