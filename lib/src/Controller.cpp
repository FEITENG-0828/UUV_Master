#include "../inc/Controller.h"

#ifdef MY_DEBUG
#   include <QDebug>
#endif

namespace FEITENG
{
    Controller::Controller(MainWindow* main_window)
    {
        m_joysticks_instance_ptr = QJoysticks::getInstance();
        m_joysticks_instance_ptr->setVirtualJoystickEnabled(false);
        m_current_joystick_ptr = nullptr;
        m_has_inited_current_joystick = false;
        connect(m_joysticks_instance_ptr, &QJoysticks::countChanged,
                this, &Controller::initCurrentJoystick);

        qRegisterMetaType<ListeningState>();
        m_listening_state = ListeningState::UNSTARTED;

        m_polling_period = 50;

        m_timer_ptr = new QTimer(this);
        m_timer_ptr->setInterval(m_polling_period);
        connect(m_timer_ptr, &QTimer::timeout, this, &Controller::updateData);

        connect(this, &Controller::joystickDataSended,
                main_window, &MainWindow::updateJoystickData);
        connect(this, &Controller::robotDataSended,
                main_window, &MainWindow::updateRobotData);
    }

    Controller::~Controller()
    {
        delete m_timer_ptr;
        m_timer_ptr = nullptr;
    }

    void Controller::initCurrentJoystick()
    {
        if(!m_has_inited_current_joystick)
        {
            disconnect(m_joysticks_instance_ptr, &QJoysticks::countChanged,
                       this, &Controller::initCurrentJoystick);
            m_has_inited_current_joystick = true;
            if(m_joysticks_instance_ptr->joystickExists(0))
            {
                m_current_joystick_ptr = m_joysticks_instance_ptr->getInputDevice(0);
            }
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
                if(m_listening_state != ListeningState::TERMINATED && !m_timer_ptr->isActive())
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
                disconnect(m_timer_ptr, &QTimer::timeout, this, &Controller::updateData);
                break;
        }
    }

    void Controller::updateData()
    {
        // if(m_current_joystick_ptr)
        // {
        qDebug()<<m_joysticks_instance_ptr->nonBlacklistedCount();
        qDebug()<<m_joysticks_instance_ptr->deviceNames();
        auto devicelist = m_joysticks_instance_ptr->inputDevices();
        qDebug()<<devicelist;
        for(auto iter = devicelist.begin(); iter != devicelist.end(); ++iter)
        {
            qDebug()<<(*iter)->id<<' '<<(*iter)->instanceID<<' '<<(*iter)->name;
        }
        emit joystickDataSended();
        // }
        emit robotDataSended();
    }
} // namespace FEITENG
