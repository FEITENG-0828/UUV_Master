#ifndef ___FEITENG_CONTROLLER_H
#define ___FEITENG_CONTROLLER_H

#include <QObject>
#include <QTimer>

#include "./JoysticksProxy.h"
#include "./RobotData.h"

namespace FEITENG
{
    class Controller : public QObject
    {
            Q_OBJECT

        private:
            qint16 m_polling_period;
            QTimer* m_timer_ptr;

            JoysticksProxy* m_joystick_devices_ptr;
            QList<double> m_joystick_axes;
            QList<bool> m_joystick_buttons;
            QList<int> m_joystick_povs;

            RobotData m_robot_data;

            void fetchJoystickData();
            void transcribeData();

        public:
            Controller(QObject* = nullptr);
            ~Controller();

            void setJoysticksProxy(JoysticksProxy* const);

        signals:
            void joystickDataSended(const QList<double>&,
                                    const QList<bool>&,
                                    const QList<int>&);
            void robotDataSended(const FEITENG::RobotData&); // 自定义参数类型名一定要写完整

        public slots:
            void clearData();
            void refreshData();
    };
} // namespace FEITENG

#endif // ___FEITENG_CONTROLLER_H
