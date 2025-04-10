#ifndef ___FEITENG_JOYSTICKSPROXY_H
#define ___FEITENG_JOYSTICKSPROXY_H

#include <QObject>

namespace FEITENG
{
    class JoysticksProxy : public QObject
    {
            Q_OBJECT

        private:
            bool m_be_inited;

            int m_current_idx;
            int m_current_sdl_id;

        public:
            explicit JoysticksProxy(QObject* = nullptr);
            ~JoysticksProxy();

            bool fillData(QList<double>&, QList<bool>&, QList<int>&) const;

        signals:
            void devicesChanged(const QStringList&, const int);

        private slots:
            void init();
            void adjustByCount();

        public slots:
            void adjustByChoice(const QString&);
    };
} // namespace FEITENG

#endif // ___FEITENG_JOYSTICKSPROXY_H
