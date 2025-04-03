#include "Controller.h"

#include "Constants.h"

namespace FEITENG
{
    Controller::Controller(QObject* parent_ptr): QObject(parent_ptr)
    {
        qRegisterMetaType<QList<double>>();
        qRegisterMetaType<QList<bool>>();
        qRegisterMetaType<QList<int>>();

        qRegisterMetaType<RobotData>();

        m_polling_period = Constants::CONTROLLER_POLLING_PERIOD;
        m_timer_ptr = new QTimer(this);
        m_timer_ptr->setInterval(m_polling_period);

        connect(m_timer_ptr, &QTimer::timeout, this, &Controller::refreshData);
        m_timer_ptr->start();
    }

    Controller::~Controller()
    { }

    void Controller::setJoysticksProxy(JoysticksProxy* const joysticks_proxy_ptr)
    {
        m_joystick_devices_ptr = joysticks_proxy_ptr;
    }

    void Controller::fetchJoystickData()
    {
        m_joystick_devices_ptr->fillData(m_joystick_axes,
                                     m_joystick_buttons,
                                     m_joystick_povs);
        for(double& value : m_joystick_axes)
        {
            value = qBound(-1.0, value, 1.0);
        }
    }

    void Controller::transcribeData()
    {
        m_robot_data.setFromJoystickData(m_joystick_axes,
                                         m_joystick_buttons,
                                         m_joystick_povs);
    }

    void Controller::clearData()
    {
        m_joystick_axes.clear();
        m_joystick_buttons.clear();
        m_joystick_povs.clear();

        m_robot_data.clear();
    }

    void Controller::refreshData()
    {
        fetchJoystickData();
        transcribeData();
        emit joystickDataSended(m_joystick_axes, m_joystick_buttons, m_joystick_povs);
        emit robotDataSended(m_robot_data);
    }
} // namespace FEITENG
