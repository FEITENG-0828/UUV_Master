#ifndef ___FEITENG_CONSTANTS_H
#define ___FEITENG_CONSTANTS_H

#include <QString>

#include "RobotDataFormatter.h"

namespace FEITENG
{
    class Constants
    {
        public:
            static const QString DEFAULT_CONFIG_PATH;
            static const QString INI_CODEC;

            static const qint16 CONTROLLER_POLLING_PERIOD;

            static const RobotDataFormatterFactory::FormatType ROBOTDATA_FORMAT_TYPE;

            static const QString RTCCLIENT_EXE_PATH;
            static const int VIDEOFRAME_READ_INTERVAL;
            static const QString FALLBACK_VIDEO_REMOTE_IP;
            static const quint16 FALLBACK_VIDEO_REMOTE_PORT;
    };
} // namespace FEITENG

#endif // ___FEITENG_CONSTANTS_H
