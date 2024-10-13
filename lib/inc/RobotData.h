#ifndef ___FEITENG_ROBOTDATA_H
#define ___FEITENG_ROBOTDATA_H

#include <QtGlobal>
#include <QVector>
#include <QList>
#include <QMetaType>

namespace FEITENG
{
    class RobotData
    {
        public:
            enum class AxisType : quint8
            {
                X = 0, Y = 1, Z = 2, Roll = 3, Pitch = 4, Yaw = 5
            };

            union VariantType
            {
                bool as_bool;
                quint8 as_uint8;
                quint16 as_uint16;
                quint32 as_uint32;
                qint8 as_int8;
                qint16 as_int16;
                qint32 as_int32;
                float as_float;
            };

            struct Actuator
            {
                quint8 id;
                VariantType value;
            };

        private:
            float m_axes[6];
            QVector<Actuator> m_actuators;

        public:
            RobotData();
            ~RobotData();

            void clear();
            void setFromJoystickData(const QList<double>&,
                                     const QList<bool>&,
                                     const QList<int>&);
            float getAxis(const AxisType) const;
            const QVector<Actuator>& getActuators() const;
    };
} // namespace FEITENG

Q_DECLARE_METATYPE(FEITENG::RobotData)

#endif // ___FEITENG_ROBOTDATA_H
