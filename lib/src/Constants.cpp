#include "Constants.h"

namespace FEITENG
{
    const QString Constants::DEFAULT_CONFIG_PATH = "./config.ini";
    const QString Constants::INI_CODEC = "UTF-8";

    const qint16 Constants::CONTROLLER_POLLING_PERIOD = 50;

    const RobotDataFormatterFactory::FormatType Constants::ROBOTDATA_FORMAT_TYPE =
        RobotDataFormatterFactory::FormatType::Usart;

    const QString Constants::RTCCLIENT_EXE_PATH = "./rtc_client/rtc_client.exe";
    const int Constants::VIDEOFRAME_READ_INTERVAL = 8;
    const QString Constants::FALLBACK_VIDEO_REMOTE_IP = "127.0.0.1";
    const quint16 Constants::FALLBACK_VIDEO_REMOTE_PORT = 20000;
} // namespace FEITENG
