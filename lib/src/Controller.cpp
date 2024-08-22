#include "../inc/Controller.h"

#include <QComboBox>
#include <QtMath>
#ifdef MY_DEBUG
#   include <QDebug>
#endif

namespace FEITENG
{
    Controller::Controller(MainWindow* main_window_ptr): QObject(nullptr)
    {
        m_main_window_ptr = main_window_ptr;
        connect(m_main_window_ptr
                    ->findChildControl<QComboBox*>("joysticks_combo_box"),
                &QComboBox::currentTextChanged,
                this,
                &Controller::changeJoystickByChoice);

        m_joysticks_instance_ptr = QJoysticks::getInstance();
        m_joysticks_instance_ptr->setVirtualJoystickEnabled(false);
        m_current_joystick_device_idx = -1;
        m_current_joystick_sdl_id = -1;
        m_has_inited_current_joystick = false;
        connect(m_joysticks_instance_ptr, &QJoysticks::countChanged,
                this, &Controller::initCurrentJoystick);

        connect(this, &Controller::joysticksChanged,
                m_main_window_ptr, &MainWindow::updateJoysticksList);

        qRegisterMetaType<QList<double>>();
        qRegisterMetaType<QList<bool>>();
        qRegisterMetaType<QList<int>>();
        qRegisterMetaType<QList<float>>();
        resetData();

        qRegisterMetaType<ListeningState>();
        m_listening_state = ListeningState::UNSTARTED;

        m_polling_period = 50;

        m_timer_ptr = new QTimer(this);
        m_timer_ptr->setInterval(m_polling_period);
        connect(m_timer_ptr, &QTimer::timeout, this, &Controller::refreshData);

        connect(this, &Controller::joystickDataSended,
                m_main_window_ptr, &MainWindow::updateJoystickData);
        connect(this, &Controller::robotDataSended,
                m_main_window_ptr, &MainWindow::updateRobotData);
    }

    Controller::~Controller()
    {
        delete m_timer_ptr;
        m_timer_ptr = nullptr;
    }

    QJoystickDevice* Controller::getJoystickDevice() const
    {
        const int joysticks_cnt = m_joysticks_instance_ptr->count();
        for(int joystick_idx = 0; joystick_idx < joysticks_cnt; ++joystick_idx)
        {
            QJoystickDevice* current_joystick_ptr =
                m_joysticks_instance_ptr->getInputDevice(joystick_idx);
            if (m_current_joystick_sdl_id == current_joystick_ptr->instanceID)
            {
                return current_joystick_ptr;
            }
        }
        return nullptr;
    }

    void Controller::resetData()
    {
        m_joystick_axes.clear();
        m_joystick_buttons.clear();
        m_joystick_povs.clear();

        m_robot_data = {0, 0, 0, 0, 0, 0, 0.5};
    }

    void Controller::fetchJoystickData()
    {
        QJoystickDevice* current_joystick_ptr = getJoystickDevice();
        if(current_joystick_ptr)
        {
            m_joystick_axes = current_joystick_ptr->axes;
            for(double& value : m_joystick_axes)
            {
                value = qBound(-1.0, value, 1.0);
            }
            m_joystick_buttons = current_joystick_ptr->buttons;
            m_joystick_povs = current_joystick_ptr->povs;
        }
    }

    void Controller::transcribeData()
    {
        if(m_current_joystick_sdl_id != -1)
        {
            auto curve = [](const float k, const float x)
            {
                float ans = 0;
                if(x >= 0)
                {
                    ans = k * qPow(x, 4);
                }
                else
                {
                    ans = -k * qPow(x, 4);
                }
                return ans;
            };
            m_robot_data[0] = curve(3000, m_joystick_axes.at(2));
            m_robot_data[1] = curve(-4000, m_joystick_axes.at(1));
            m_robot_data[2] = curve(2500, m_joystick_axes.at(3));
            m_robot_data[5] = curve(900, m_joystick_axes.at(0));
            if(m_joystick_buttons.at(0))
            {
             m_robot_data[6] = 0.45;
            }
            if(m_joystick_buttons.at(1))
            {
             m_robot_data[6] = 0.9;
            }
        }
    }

    void Controller::initCurrentJoystick()
    {
        if(!m_has_inited_current_joystick)
        {
            disconnect(m_joysticks_instance_ptr, &QJoysticks::countChanged,
                       this, &Controller::initCurrentJoystick);
            m_has_inited_current_joystick = true;
            if(m_joysticks_instance_ptr->count() > 0)
            {
                m_current_joystick_device_idx = 0;
                m_current_joystick_sdl_id =
                    m_joysticks_instance_ptr->getInputDevice(0)->instanceID;
            }

            connect(m_joysticks_instance_ptr, &QJoysticks::countChanged,
                    this, &Controller::fitChangedJoysticks);
            emit joysticksChanged(m_joysticks_instance_ptr->deviceNames(),
                                  m_current_joystick_device_idx);
        }
    }

    void Controller::fitChangedJoysticks()
    {
        QStringList joysticks_names = m_joysticks_instance_ptr->deviceNames();
        QJoystickDevice* current_joystick_ptr = getJoystickDevice();
        if(current_joystick_ptr)
        {
            m_current_joystick_device_idx = current_joystick_ptr->id;
        }
        else
        {
            resetData();
            if(m_joysticks_instance_ptr->count() > 0)
            {
                m_current_joystick_device_idx = 0;
                m_current_joystick_sdl_id =
                    m_joysticks_instance_ptr->getInputDevice(0)->instanceID;
            }
            else
            {
                m_current_joystick_device_idx = -1;
                m_current_joystick_sdl_id = -1;
            }
        }
        emit joysticksChanged(joysticks_names, m_current_joystick_device_idx);
    }

    void Controller::changeJoystickByChoice()
    {
        QJoystickDevice* new_joystick_ptr =
            m_joysticks_instance_ptr->getInputDevice(
                m_main_window_ptr
                    ->findChildControl<QComboBox*>("joysticks_combo_box")
                    ->currentIndex()
            );
        if(new_joystick_ptr)
        {
            m_current_joystick_device_idx = new_joystick_ptr->id;
            m_current_joystick_sdl_id = new_joystick_ptr->instanceID;
        }
        else
        {
            m_current_joystick_device_idx = -1;
            m_current_joystick_sdl_id = -1;
        }
    }

    void Controller::updateListeningState(const ListeningState& state)
    {
        switch(state)
        {
            case ListeningState::UNSTARTED:
#               ifdef MY_DEBUG
                qDebug()<<"\ndangerous input\n";
#               endif
                break;

            case ListeningState::RUNNING:
                if(m_listening_state != ListeningState::TERMINATED &&
                   !m_timer_ptr->isActive())
                {
                    m_listening_state = state;
                    m_timer_ptr->start();
                }
                break;

            case ListeningState::PAUSED:
                if(m_listening_state != ListeningState::TERMINATED)
                {
                    m_listening_state = state;
                    if(m_timer_ptr->isActive())
                    {
                        m_timer_ptr->stop();
                    }
                }
                break;

            case ListeningState::TERMINATED:
                m_listening_state = state;
                if(m_timer_ptr->isActive())
                {
                    m_timer_ptr->stop();
                }
                disconnect(m_timer_ptr, &QTimer::timeout,
                           this, &Controller::refreshData);
                break;
        }
    }

    void Controller::refreshData()
    {
        fetchJoystickData();
        transcribeData();
        emit joystickDataSended(m_joystick_axes, m_joystick_buttons, m_joystick_povs);
        emit robotDataSended(m_robot_data);
    }
} // namespace FEITENG
