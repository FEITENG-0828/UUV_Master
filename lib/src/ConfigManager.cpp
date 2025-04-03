#include "ConfigManager.h"

#include <QTextCodec>

#include "Constants.h"

namespace FEITENG
{
    QMutex ConfigManager::hash_mutex;
    QHash<QString, QSharedPointer<ConfigManager>> ConfigManager::instances;

    ConfigManager::ConfigManager(const QString& config_name)
        : m_config_name(config_name),
          m_settings_ptr(new QSettings(config_name, QSettings::IniFormat, this))
    {
        m_settings_ptr->setIniCodec(QTextCodec::codecForName(Constants::INI_CODEC.toUtf8()));
    }

    ConfigManager::~ConfigManager()
    { }

    QSharedPointer<ConfigManager> ConfigManager::getInstance(const QString& name)
    {
        QString config_name = name.isEmpty()? Constants::DEFAULT_CONFIG_PATH : name;

        QMutexLocker locker(&hash_mutex);
        if (!instances.contains(config_name))
        {
            instances.insert(config_name,
                QSharedPointer<ConfigManager>(new ConfigManager(config_name)));
        }
        return instances.value(config_name);
    }

    void ConfigManager::setValue(const QString& key, const QVariant& value)
    {
        QMutexLocker locker(&m_mutex);
        m_settings_ptr->setValue(key, value);
        m_settings_ptr->sync();
    }

    QVariant ConfigManager::getValue(const QString& key, const QVariant& default_value) const
    {
        QMutexLocker locker(&m_mutex);
        return m_settings_ptr->value(key, default_value);
    }
}
