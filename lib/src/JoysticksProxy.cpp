#include "JoysticksProxy.h"

#include <QTimer>

#include "QJoysticks.h"

namespace FEITENG
{
    JoysticksProxy::JoysticksProxy(QObject* parent_ptr): QObject(parent_ptr)
    {
        QJoysticks::getInstance()->setVirtualJoystickEnabled(false);

        m_be_inited = false;

        m_current_idx = -1;
        m_current_sdl_id = -1;

        connect(QJoysticks::getInstance(), &QJoysticks::countChanged,
                this, &JoysticksProxy::adjustByCount);

        QTimer::singleShot(0, this, &JoysticksProxy::init);
    }

    JoysticksProxy::~JoysticksProxy()
    { }

    void JoysticksProxy::init()
    {
        if(!m_be_inited)
        {
            m_be_inited = true;

            if(QJoysticks::getInstance()->count() > 0)
            {
                m_current_idx = 0;
                m_current_sdl_id =
                    QJoysticks::getInstance()->getInputDevice(0)->instanceID;
                emit devicesChanged(QJoysticks::getInstance()->deviceNames(),
                                    m_current_idx);
            }
        }
    }

    void JoysticksProxy::adjustByCount()
    {
        if(m_be_inited)
        {
            m_current_idx = -1;
            const int joysticks_cnt = QJoysticks::getInstance()->count();

            const QJoystickDevice* current_device_ptr = nullptr;
            for(int joystick_idx = 0; joystick_idx < joysticks_cnt; ++joystick_idx)
            {
                current_device_ptr = QJoysticks::getInstance()->getInputDevice(joystick_idx);
                if (m_current_sdl_id == current_device_ptr->instanceID)
                {
                    m_current_idx = current_device_ptr->id;
                    break;
                }
            }
            if(m_current_idx == -1)
            {
                if(joysticks_cnt > 0)
                {
                    m_current_idx = 0;
                    m_current_sdl_id =
                        QJoysticks::getInstance()->getInputDevice(0)->instanceID;
                }
                else
                {
                    m_current_sdl_id = -1;
                }
            }

            emit devicesChanged(QJoysticks::getInstance()->deviceNames(), m_current_idx);
        }
    }

    void JoysticksProxy::adjustByChoice(const QString& device_str)
    {
        if(m_be_inited)
        {
            int device_idx = QJoysticks::getInstance()->deviceNames().indexOf(device_str);
            QJoystickDevice* new_device_ptr =
                QJoysticks::getInstance()->getInputDevice(device_idx);
            if(new_device_ptr)
            {
                m_current_idx = new_device_ptr->id;
                m_current_sdl_id = new_device_ptr->instanceID;
            }
            else
            {
                m_current_idx = -1;
                m_current_sdl_id = -1;
            }
        }
    }

    bool JoysticksProxy::fillData(QList<double>& axes,
                                  QList<bool>& buttons,
                                  QList<int>& povs) const
    {
        bool is_success = false;
        if(m_be_inited)
        {
            const QJoystickDevice* current_device_ptr =
                QJoysticks::getInstance()->getInputDevice(m_current_idx);
            if(current_device_ptr)
            {
                axes = current_device_ptr->axes;
                buttons = current_device_ptr->buttons;
                povs = current_device_ptr->povs;
                is_success = true;
            }
            else
            {
                axes.clear();
                buttons.clear();
                povs.clear();
            }
        }
        return is_success;
    }
} // namespace FEITENG
