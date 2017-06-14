#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{
    modbusDevice = new QModbusTcpServer(this);

    QModbusDataUnitMap reg;
//    reg.insert(QModbusDataUnit::Coils, { QModbusDataUnit::Coils, 0, 10 });
//    reg.insert(QModbusDataUnit::DiscreteInputs, { QModbusDataUnit::DiscreteInputs, 0, 10 });
//    reg.insert(QModbusDataUnit::InputRegisters, { QModbusDataUnit::InputRegisters, 0, 10 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 65535 });

    modbusDevice->setMap(reg);

    connect(modbusDevice, &QModbusServer::dataWritten,
            this, &Worker::dataWritten);
//    connect(modbusDevice, &QModbusServer::stateChanged,
//            this, &Worker::stateChanged);
    connect(modbusDevice, &QModbusServer::errorOccurred,
            this, &Worker::errorOccurred);

    modbusDevice->setValue(QModbusServer::ListenOnlyMode, true);
    
    db = mysql.connect_db("Monita_Modbus_Server");
    
    for (int i = 0; i <= 65535; i++) {
        modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
            i,
            quint16(0)
        );
    }
    regMapping = cfg.read("CONFIG");
    createServer();
    get_data();

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(get_data()));
    t->start(3000);
}

Worker::~Worker()
{
    bool intendToConnect = (modbusDevice->state() == QModbusDevice::UnconnectedState);
    if (!intendToConnect) {
        modbusDevice->disconnectDevice();
    }
}

void Worker::createServer()
{
    bool intendToConnect = (modbusDevice->state() == QModbusDevice::UnconnectedState);

    if (intendToConnect) {
        const QUrl url = QUrl::fromUserInput("localhost:502");
        int slave_id = 1;

        modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

        modbusDevice->setServerAddress(slave_id);

        if (!modbusDevice->connectDevice()) {
            printf("%sModBus Server Failed Connecting ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());
        } else {
            printf("%sModBus Server Success Connecting ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());
        }
    }
}

void Worker::setupDeviceData()
{
//    if (!modbusDevice) return;

    printf("%sSetup Device Data ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());

    bool ok;
    for (int i = 0; i < titik_ukur.length(); i++) {
        if (titik_ukur.at(i).toInt() <= 65535) {
            for (int j = 0; j < regMapping.length(); j++) {
                if (titik_ukur.length() > 0 && value.length() > 0) {
                    QStringList list = regMapping.at(j).split(";");
                    if (titik_ukur.at(i) == list.at(1)) {
                        if (list.at(2) == "INT16") {
                            int temp = static_cast<int>(value.at(i).toFloat(&ok) + .5);
                            QString temp2 = QString::number(temp, 16);
                            quint16 val = temp2.toUInt(&ok, 16);

                            printf("%sreg : %s, titik ukur : %s, value : %s, hex : %s\n",
                                   QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(),
                                   list.at(0).toUtf8().data(),
                                   titik_ukur.at(i).toUtf8().data(),
                                   value.at(i).toUtf8().data(),
                                   temp2.toUtf8().data());
                            modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
                                list.at(0).toInt(),
                                val
                            );
                        } else if (list.at(2) == "INT32") {
                            int temp = static_cast<int>(value.at(i).toFloat(&ok) + .5);
                            QString hex_on_string = QString::number(temp, 16);

                            QString hex_part[2]; int cnt = 0;
                            for (int k = hex_on_string.length()-1; k >= 0; k--) {
                                if (cnt < 4) {
                                    hex_part[1].prepend(hex_on_string.at(k));
                                    cnt++;
                                } else {
                                    hex_part[0].prepend(hex_on_string.at(k));
                                }
                            }

                            quint16 hex_on_quint16_1 = hex_part[0].toUInt(&ok, 16);
                            quint16 hex_on_quint16_2 = hex_part[1].toUInt(&ok, 16);

                            printf("%sreg : %i, titik ukur : %i, value : %s, hex : %s\n",
                                   QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(),
                                   list.at(0).toInt(),
                                   titik_ukur.at(i).toInt(),
                                   value.at(i).toUtf8().data(),
                                   hex_part[0].toUtf8().data());
                            modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
                                list.at(0).toInt(),
                                hex_on_quint16_1
                            );
                            printf("%sreg : %i, titik ukur : %i, value : %s, hex : %s\n",
                                   QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(),
                                   list.at(0).toInt()+1,
                                   titik_ukur.at(i).toInt(),
                                   value.at(i).toUtf8().data(),
                                   hex_part[1].toUtf8().data());
                            modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
                                list.at(0).toInt()+1,
                                hex_on_quint16_2
                            );
                        } else if (list.at(2) == "INT64") {

                        } else if (list.at(2) == "FLOAT32") {
                            float f = value.at(i).toFloat(&ok);
                            QByteArray float_to_byteArray(reinterpret_cast<const char*>(&f), sizeof(f));
                            QByteArray byteArray_to_hex = float_to_byteArray.toHex();
                            QByteArray swap_hex;
                            for (int i = byteArray_to_hex.length(); i > 0 ; i-=2) {
                                swap_hex.append(byteArray_to_hex.at(i-2));
                                swap_hex.append(byteArray_to_hex.at(i-1));
                            }
                            QString hex_on_string = swap_hex;
                            QString hex_part[2];
                            for (int k = 0; k < hex_on_string.length(); k++) {
                                if (k < 4) {
                                    hex_part[0].append(hex_on_string.at(k));
                                } else {
                                    hex_part[1].append(hex_on_string.at(k));
                                }
                            }
                            quint16 hex_on_quint16_1 = hex_part[0].toUInt(&ok, 16);
                            quint16 hex_on_quint16_2 = hex_part[1].toUInt(&ok, 16);

                            printf("%sreg : %i, titik ukur : %i, value : %s, hex : %s\n",
                                   QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(),
                                   list.at(0).toInt(),
                                   titik_ukur.at(i).toInt(),
                                   value.at(i).toUtf8().data(),
                                   hex_part[0].toUtf8().data());
                            modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
                                list.at(0).toInt(),
                                hex_on_quint16_1
                            );
                            printf("%sreg : %i, titik ukur : %i, value : %s, hex : %s\n",
                                   QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(),
                                   list.at(0).toInt()+1,
                                   titik_ukur.at(i).toInt(),
                                   value.at(i).toUtf8().data(),
                                   hex_part[1].toUtf8().data());
                            modbusDevice->setData(QModbusDataUnit::HoldingRegisters,
                                list.at(0).toInt()+1,
                                hex_on_quint16_2
                            );
                        }
                    }
                }
            }
        }
    }

    printf("%sSelesai Setup Device Data ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());
}

void Worker::get_data()
{
    printf("%sGet Data ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());

    titik_ukur.clear();
    value.clear();

    db.open();
    QSqlQuery q(QSqlDatabase::database(db.connectionName()));
    if (!q.exec("call get_data_jaman()")) {
        printf("%sGagal Get Data ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());
        return;
    } else {
        while(q.next()){
            titik_ukur.append(q.value(0).toString().toLatin1());
            value.append(q.value(2).toString().toLatin1());
        }
    }
    printf("%sSelesai Get Data ...\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data());
    setupDeviceData();
}

void Worker::dataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    for (int i = 0; i < size; ++i) {
        quint16 value;
        QString text;
        switch (table) {
        case QModbusDataUnit::Coils:
            modbusDevice->data(QModbusDataUnit::Coils, address + i, &value);
            break;
        case QModbusDataUnit::HoldingRegisters:
            modbusDevice->data(QModbusDataUnit::HoldingRegisters, address + i, &value);
//            printf("Write on address holdRegister : %s, value : %s\n",
//                   QString::number(address+i).toUtf8().data(),
//                   text.setNum(value, 16).toUtf8().data()
//                   );
            break;
        default:
            break;
        }
    }
}

//void Worker::stateChanged(int state)
//{

//}

void Worker::errorOccurred(QModbusDevice::Error newError)
{
    if (newError == QModbusDevice::NoError || !modbusDevice)
        return;

    printf("%s%s\n", QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss :: ").toUtf8().data(), modbusDevice->errorString().toUtf8().data());
}
