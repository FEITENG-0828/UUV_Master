#include "../inc/Controller.h"

#ifdef MY_DEBUG
#   include <QDebug>
#endif

namespace FEITENG
{
    Controller::Controller(MainWindow* main_window)
    {
        qRegisterMetaType<ListeningState>();
        m_listening_state = ListeningState::UNSTARTED;

        m_polling_period = 50;

        m_timer_ptr = new QTimer(this);
        m_timer_ptr->setInterval(m_polling_period);
        connect(m_timer_ptr, &QTimer::timeout, this, &Controller::updateData);

        connect(this, &Controller::joystickDataSended, main_window, &MainWindow::updateJoystickData);
        connect(this, &Controller::robotDataSended, main_window, &MainWindow::updateRobotData);
    }

    Controller::~Controller()
    {
        delete m_timer_ptr;
        m_timer_ptr = nullptr;
    }

    void Controller::updateListeningState(const ListeningState& state_input)
    {
        switch(state_input)
        {
            case ListeningState::UNSTARTED:
#               ifdef MY_DEBUG
                qDebug()<<"\ndangerous input\n";
#               endif
                break;

            case ListeningState::RUNNING:
                if(!m_timer_ptr->isActive())
                {
                    m_timer_ptr->start();
                }
                break;

            case ListeningState::PAUSED:
                if(m_timer_ptr->isActive())
                {
                    m_timer_ptr->stop();
                }
                break;

            case ListeningState::TERMINATED:
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
        emit joystickDataSended();
        emit robotDataSended();
    }
} // namespace FEITENG
