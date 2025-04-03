#ifndef ___FEITENG_CONFIGMANAGER_H
#define ___FEITENG_CONFIGMANAGER_H

#include <QMutex>
#include <QHash>
#include <QSharedPointer>
#include <QSettings>
#include <QVariant>

namespace FEITENG
{
    class ConfigManager : public QObject
    {
            Q_OBJECT

        private:
            static QMutex hash_mutex;
            static QHash<QString, QSharedPointer<ConfigManager>> instances;

            QString m_config_name;
            QSettings* m_settings_ptr;
            mutable QMutex m_mutex;

            explicit ConfigManager(const QString&);

        public:
            ConfigManager(const ConfigManager&) = delete;
            ConfigManager(ConfigManager&&) = delete;
            ConfigManager& operator=(const ConfigManager&) = delete;
            ConfigManager& operator=(ConfigManager&&) = delete;

            ~ConfigManager();

            static QSharedPointer<ConfigManager> getInstance(const QString& = QString());
            void setValue(const QString&, const QVariant&);
            QVariant getValue(const QString&, const QVariant& = QVariant()) const;
    };
} // namespace FEITENG

#endif // ___FEITENG_CONFIGMANAGER_H
