#ifndef ___FEITENG_CONTROLLER_H
#define ___FEITENG_CONTROLLER_H

#include "../../mainwindow.h"
#include <QObject>
#include <QTimer>

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
            ListeningState m_listening_state;
            QTimer* m_timer_ptr;
            qint16 m_polling_period;

        public:
            Controller(MainWindow*);
            ~Controller();

        signals:
            void joystickDataSended();
            void robotDataSended();

        public slots:
            void updateListeningState(const ListeningState&);
            void updateData();
    };
} // namespace FEITENG
Q_DECLARE_METATYPE(FEITENG::Controller::ListeningState)

#endif // ___FEITENG_CONTROLLER_H
