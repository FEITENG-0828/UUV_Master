#include "RobotData.h"

#include <QtMath>

namespace FEITENG
{
    RobotData::RobotData()
    {
        clear();
    }

    RobotData::~RobotData()
    { }

    void RobotData::clear()
    {
        for(float& axis : m_axes)
        {
            axis = 0;
        }
        m_actuators.clear();
        m_actuators.append({1, {.as_float = 0.5}});
    }

    void RobotData::setFromJoystickData(const QList<double>& axes,
                                        const QList<bool>& buttons,
                                        [[maybe_unused]] const QList<int>& povs)
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

        if(axes.size() >= 4)
        {
            m_axes[0] = curve(3000, axes.at(2));
            m_axes[1] = curve(-4000, axes.at(1));
            m_axes[2] = curve(2500, axes.at(3));
            m_axes[5] = curve(900, axes.at(0));
        }
        else
        {
            clear();
        }
        if(buttons.size() > 0)
        {
            if(buttons.at(4))
            {
             m_actuators[0].value.as_float = 0.45;
            }
            if(buttons.at(5))
            {
             m_actuators[0].value.as_float = 0.9;
            }
        }
        else
        {
            clear();
        }
    }

    float RobotData::getAxis(const AxisType axis_type) const
    {
        return m_axes[static_cast<quint8>(axis_type)];
    }

    const QVector<RobotData::Actuator>& RobotData::getActuators() const
    {
        return m_actuators;
    }
} // namespace FEITENG
