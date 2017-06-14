#ifndef WORKER_H
#define WORKER_H

#include "util/utama.h"
#include "util/config.h"
#include "model/init_mysql.h"

#include <QObject>
#include <QModbusServer>
#include <QModbusTcpServer>
#include <QUrl>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

private:
    QModbusServer *modbusDevice;
    void createServer();
    void setupDeviceData();

    QStringList regMapping;
    config cfg;

    init_mysql mysql;
    QSqlDatabase db;

    QStringList titik_ukur;
    QStringList value;

private slots:
    void get_data();

private Q_SLOTS:
    void dataWritten(QModbusDataUnit::RegisterType table, int address, int size);
//    void stateChanged(int status);
    void errorOccurred(QModbusDevice::Error newError);

signals:

public slots:
};

#endif // WORKER_H
