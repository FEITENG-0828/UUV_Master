#include "../inc/JoystickDataTransmitter.h"

#include <QLineEdit>

namespace FEITENG
{
    JoystickDataTransmitter::JoystickDataTransmitter(
        MainWindow* main_window_ptr) : QObject(nullptr)
    {
        m_main_window_ptr = main_window_ptr;

        m_host_ip = "192.168.16.102";
        m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_ip_line_edit")
            ->setText(m_host_ip);
        connect(m_main_window_ptr
                    ->findChildControl<QLineEdit*>("host_ip_line_edit"),
                &QLineEdit::editingFinished,
                this,
                &JoystickDataTransmitter::changeHostIpByEdit);

        m_host_port = 10086;
        m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_port_line_edit")
            ->setText(QString::number(m_host_port));
        connect(m_main_window_ptr
                    ->findChildControl<QLineEdit*>("host_port_line_edit"),
                &QLineEdit::editingFinished,
                this,
                &JoystickDataTransmitter::changeHostPortByEdit);

        m_socket_ptr = new QUdpSocket(this);

        qRegisterMetaType<QList<float>>();
    }

    JoystickDataTransmitter::~JoystickDataTransmitter()
    { }

    void JoystickDataTransmitter::transmitData(const QList<float>& robot_data)
    {
        QString robot_data_json_str = "{"
            "\"x\":" + QString::number(robot_data.at(0), 'f', 2) + ',' +
            "\"y\":" + QString::number(robot_data.at(1), 'f', 2) + ',' +
            "\"z\":" + QString::number(robot_data.at(2), 'f', 2) + ',' +
            "\"roll\":" + QString::number(robot_data.at(3), 'f', 2) + ',' +
            "\"pitch\":" + QString::number(robot_data.at(4), 'f', 2) + ',' +
            "\"yaw\":" + QString::number(robot_data.at(5), 'f', 2) + ',' +
            "\"servo0\":" + QString::number(robot_data.at(6), 'f', 6) + ',' +
            "\"servo1\":0,\"state\":0"
        "}";
        m_socket_ptr->writeDatagram(robot_data_json_str.toUtf8(),
                                    QHostAddress(m_host_ip),
                                    m_host_port);
    }

    void JoystickDataTransmitter::changeHostIpByEdit()
    {
        m_host_ip = m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_ip_line_edit")
            ->text();
    }

    void JoystickDataTransmitter::changeHostPortByEdit()
    {
        m_host_port = m_main_window_ptr
            ->findChildControl<QLineEdit*>("host_port_line_edit")
            ->text().toULongLong();
    }
} // namespace FEITENG
