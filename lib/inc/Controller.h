#ifndef ___FEITENG_CONTROLLER_H
#define ___FEITENG_CONTROLLER_H

#include "../../mainwindow.h"
#include <QObject>
#include <QTimer>

#include "QJoysticks.h"

namespace FEITENG
{
    class Controller : public QObject
    {
            Q_OBJECT

        public:
            enum class ListeningState
            {
                UNSTARTED,
                RUNNING,
                PAUSED,
                TERMINATED
            };

        private:
            QJoysticks* m_joysticks_instance_ptr;
            QJoystickDevice* m_current_joystick_ptr;
            bool m_has_inited_current_joystick;

            ListeningState m_listening_state;
            QTimer* m_timer_ptr;
            qint16 m_polling_period;

        public:
            Controller(MainWindow*);
            ~Controller();

        signals:
            void joystickDataSended();
            void robotDataSended();

        private slots:
            void initCurrentJoystick();

        public slots:
            void updateListeningState(const ListeningState&);
            void updateData();
    };
} // namespace FEITENG
Q_DECLARE_METATYPE(FEITENG::Controller::ListeningState)

#endif // ___FEITENG_CONTROLLER_H
