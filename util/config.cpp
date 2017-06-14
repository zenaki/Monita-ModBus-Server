#include "config.h"

config::config()
{

}

QStringList config::read(QString obj)
{
    QStringList result;
    if (loadConfig(config::Json)) {
        QJsonDocument JsonDoc = QJsonDocument::fromJson(data_json);
        QJsonObject object = JsonDoc.object();
        QJsonValue value = object.value(obj);
        if (obj == "CONFIG") {
            if (object.value(obj).isArray()) {
                QJsonArray array = value.toArray();
                foreach (const QJsonValue & v, array) {
                    result.append(
                                QString::number(v.toObject().value("register").toInt()) +
                                ";" +
                                QString::number(v.toObject().value("alias").toInt()) +
                                ";" +
                                v.toObject().value("type").toString()
                                );
                }
            }
        }
    }

    return result;
}

void config::write(QJsonObject &json) const
{
    QJsonArray configArray;
    QJsonObject regObj;
    for (int i = 1; i <= 65535; i++) {
        regObj["register"] = i;
        regObj["alias"] = 0;
        regObj["type"] = "INT16";
//        regObj["type"] = "FLOAT32";
        configArray.append(regObj);
    }

    json["CONFIG"] = configArray;
}

bool config::loadConfig(config::SaveFormat saveFormat)
{
    QString path;
    if (saveFormat == Json) {
        path = PATH_CONFIGURATION_JSON;
    } else {
        path = PATH_CONFIGURATION_BIN;
    }
    QFile loadFile(path);
    if (!loadFile.exists()) {
        QDir dir;
        dir.mkpath(".MonBusServer_Config");
    }
    if (loadFile.open(QIODevice::ReadWrite)) {
        QByteArray readFile = loadFile.readAll();
        if (readFile.isEmpty()) {
            QJsonObject configObject;
            write(configObject);
            QJsonDocument saveDoc(configObject);
            loadFile.write(saveFormat == Json
                ? saveDoc.toJson()
                : saveDoc.toBinaryData());
        }
        data_json = readFile;
        return true;
    } else {
        return false;
    }
}

bool config::saveConfig(config::SaveFormat saveFormat) const
{
    QString path;
    if (saveFormat == Json) {
        path = PATH_CONFIGURATION_JSON;
    } else {
        path = PATH_CONFIGURATION_BIN;
    }
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't save config ..";
        return false;
    }

    QJsonObject configObject;
    write(configObject);
    QJsonDocument saveDoc(configObject);
    saveFile.write(saveFormat == Json
        ? saveDoc.toJson()
        : saveDoc.toBinaryData());

    return true;
}
