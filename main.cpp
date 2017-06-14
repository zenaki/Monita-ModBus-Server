#include <QCoreApplication>
#include <QLoggingCategory>

#include "controller/worker.h"

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));
    QCoreApplication a(argc, argv);

//    //** INT TO HEX & HEX TO INT **//
//    bool ok;
//    int first_int = 655355;
//    QString hex_on_str = QString::number(first_int, 16);
//    quint16 quint16_from_hex = hex_on_str.toUInt(&ok, 16);

//    //** FLOAT TO HEX **//
//    float first_float = 255.255;
//    QByteArray float_to_byteArray(reinterpret_cast<const char*>(&first_float), sizeof(first_float));
//    QByteArray byteArray_to_hex = float_to_byteArray.toHex();
//    QByteArray swap_hex;
//    for (int i = byteArray_to_hex.length(); i > 0 ; i-=2) {
//        swap_hex.append(byteArray_to_hex.at(i-2));
//        swap_hex.append(byteArray_to_hex.at(i-1));
//    }
//    QString hex_on_string = swap_hex;
//    QString hex_part[2];
//    for (int i = 0; i < hex_on_string.length(); i++) {
//        if (i < 4) {
//            hex_part[0].append(hex_on_string.at(i));
//        } else {
//            hex_part[1].append(hex_on_string.at(i));
//        }
//    }
//    quint16 hex_on_quint16_1 = hex_part[0].toUInt(&ok, 16);
//    quint16 hex_on_quint16_2 = hex_part[1].toUInt(&ok, 16);

//    QString hex_from_quint16_1 = QString::number(hex_on_quint16_1, 16);
//    QString hex_from_quint16_2 = QString::number(hex_on_quint16_2, 16);

//    QString hex_from_quint16 = hex_from_quint16_1 + hex_from_quint16_2;

//    //** HEX TO FLOAT **//
//    int sign = 1;
//    QByteArray hex_to_bin = QByteArray::number(hex_from_quint16.toLongLong(&ok,16),2); //convert hex to binary -you don't need this since your incoming data is binary
//    if(hex_to_bin.length()==32) {
//        if(hex_to_bin.at(0)=='1')  sign =-1;                       // if bit 0 is 1 number is negative
//        hex_to_bin.remove(0,1);                                     // remove sign bit
//    }
//    QByteArray fraction =hex_to_bin.right(23);   //get the fractional part
//    double mantissa = 0;
//    for(int i=0;i<fraction.length();i++)     // iterate through the array to claculate the fraction as a decimal.
//        if(fraction.at(i)=='1')     mantissa += 1.0/(pow(2,i+1));
//    int exponent = hex_to_bin.left(hex_to_bin.length()-23).toLongLong(&ok,2)-127;     //claculate the exponent
//    QString float_on_string = QString::number( sign*pow(2,exponent)*(mantissa+1.0),'f', 5 );
//    float float_like_first_float = sign*pow(2,exponent)*(mantissa+1.0);

    Worker w;
    return a.exec();
}
