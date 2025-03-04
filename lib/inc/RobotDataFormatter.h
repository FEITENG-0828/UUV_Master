#ifndef ___FEITENG_ROBOTDATAFORMATTER_H
#define ___FEITENG_ROBOTDATAFORMATTER_H

#include <QObject>
#include <QSharedPointer>

#include "RobotData.h"

namespace FEITENG
{
    class RobotDataFormatter : public QObject
    {
            Q_OBJECT

        public:
            RobotDataFormatter(QObject* = nullptr);
            virtual ~RobotDataFormatter();

            virtual QByteArray serializeData(const RobotData&) const = 0;
    };

    class JsonFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            JsonFormatter(QObject* = nullptr);
            virtual ~JsonFormatter();

            virtual QByteArray serializeData(const RobotData&) const override;
    };

    class UsartFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            UsartFormatter(QObject* = nullptr);
            virtual ~UsartFormatter();

            virtual QByteArray serializeData(const RobotData&) const override;
    };

    class CanfdFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            CanfdFormatter(QObject* = nullptr);
            virtual ~CanfdFormatter();

            virtual QByteArray serializeData(const RobotData&) const override;
    };

    class RobotDataFormatterFactory : public QObject
    {
            Q_OBJECT

        public:
            enum class FormatType : quint8
            {
                Json = 0, Usart = 1, Canfd = 2
            };

            RobotDataFormatterFactory(QObject* = nullptr);
            ~RobotDataFormatterFactory();

            static QSharedPointer<RobotDataFormatter>
                createFormatter(const FormatType, QObject* parent_ptr);
    };
} // namespace FEITENG

#endif // ___FEITENG_ROBOTDATAFORMATTER_H
