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
            MainWindow* m_main_window_ptr;

            QJoysticks* m_joysticks_instance_ptr;
            int m_current_joystick_device_idx;
            int m_current_joystick_sdl_id;
            bool m_has_inited_current_joystick;

            QList<double> axes;
            QList<bool> buttons;
            QList<int> povs;

            ListeningState m_listening_state;
            QTimer* m_timer_ptr;
            qint16 m_polling_period;

            QJoystickDevice* getJoystickDevice() const;
            void fetchJoystickData();
            void transcribeData();

        public:
            Controller(MainWindow*);
            ~Controller();

        signals:
            void joysticksChanged(const QStringList&, const int);
            void joystickDataSended(const QList<double>&, const QList<bool>&, const QList<int>&);
            void robotDataSended();

        private slots:
            void initCurrentJoystick();

        public slots:
            void fitChangedJoysticks();
            void changeJoystickByChoice();
            void updateListeningState(const ListeningState&);
            void refreshData();
    };
} // namespace FEITENG
Q_DECLARE_METATYPE(FEITENG::Controller::ListeningState)

#endif // ___FEITENG_CONTROLLER_H
