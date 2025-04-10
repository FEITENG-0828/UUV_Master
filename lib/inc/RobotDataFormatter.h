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
            explicit RobotDataFormatter(QObject* = nullptr);
            virtual ~RobotDataFormatter();

            virtual QByteArray serializeData(const RobotData&) const;
    };

    class JsonFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            explicit JsonFormatter(QObject* = nullptr);
            virtual ~JsonFormatter();

            virtual QByteArray serializeData(const RobotData&) const override;
    };

    class UsartFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            explicit UsartFormatter(QObject* = nullptr);
            virtual ~UsartFormatter();

            virtual QByteArray serializeData(const RobotData&) const override;
    };

    class CanfdFormatter : public RobotDataFormatter
    {
            Q_OBJECT

        public:
            explicit CanfdFormatter(QObject* = nullptr);
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

            explicit RobotDataFormatterFactory(QObject* = nullptr);
            ~RobotDataFormatterFactory();

            static QSharedPointer<RobotDataFormatter>
                createFormatter(const FormatType, QObject* parent_ptr);
    };
} // namespace FEITENG

#endif // ___FEITENG_ROBOTDATAFORMATTER_H
