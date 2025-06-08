#include <csignal>

#include <QCoreApplication>
#include <QDebug>

#include "int_converter.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    IntConverter int_converter(IntConverter::ByteOrder::BigEndian);

    uint16_t port = 65535;
    QByteArray port_bytes = int_converter.toUnit16Bytes(port);
    qDebug() << "Port in bytes (Big Endian):" << port_bytes.toHex();
    uint16_t port_from_bytes = int_converter.fromUnit16Bytes(port_bytes);
    qDebug() << "Port from bytes:" << port_from_bytes;
}
