#include "RobotDataTransmitter.h"

#include <QTimer>
#include <QSharedPointer>

#include "ConfigManager.h"
#include "Constants.h"
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

            QSharedPointer<ConfigManager> settings_ptr = ConfigManager::getInstance();
            setHostIp(settings_ptr->getValue("robot_data_transmitter/remote_ip").toString());
            setHostPort(settings_ptr->getValue("robot_data_transmitter/remote_port").toUInt());
        }
    }

    void RobotDataTransmitter::transmitData(const RobotData& robot_data) const
    {
        // static QSharedPointer<RobotDataFormatter> formatter_ptr =
        //     RobotDataFormatterFactory::createFormatter(
        //         Constants::ROBOTDATA_FORMAT_TYPE,
        //         const_cast<RobotDataTransmitter*>(this));
        // FIXME: pure virtual method called
        if(m_be_inited)
        {
            QSharedPointer<RobotDataFormatter> formatter_ptr =
                RobotDataFormatterFactory::createFormatter(
                    Constants::ROBOTDATA_FORMAT_TYPE,
                    const_cast<RobotDataTransmitter*>(this));
            QByteArray datagram = formatter_ptr->serializeData(robot_data);
            if(!datagram.isEmpty())
            {
                m_socket_ptr->writeDatagram(datagram,
                                            QHostAddress(m_host_ip),
                                            m_host_port);
            }
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
