#ifndef CONFIG_H
#define CONFIG_H

#include "util/utama.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class config
{
public:
    config();

    enum SaveFormat {
        Json, Binary
    };

    QByteArray data_json;

    QStringList read(QString obj);
    void write(QJsonObject &json) const;    //Default
    bool loadConfig(config::SaveFormat saveFormat);
    bool saveConfig(config::SaveFormat saveFormat) const;
};

#endif // CONFIG_H
