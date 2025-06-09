#include <csignal>

#include <QCoreApplication>
#include <QDebug>

#include "int_converter.h"
#include "memory_mapper.h"

// shared memory name
#define SHARED_MEMORY_NAME "qt_zmq_pub_sub_shared_memory"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    IntConverter int_converter(IntConverter::ByteOrder::BigEndian);

    uint16_t port = 65535;
    QByteArray port_bytes = int_converter.toUnit16Bytes(port);
    qDebug() << "Port in bytes (Big Endian):" << port_bytes.toHex();
    uint16_t port_from_bytes = int_converter.fromUnit16Bytes(port_bytes);
    qDebug() << "Port from bytes:" << port_from_bytes;

    MemoryMapper memory_mapper(SHARED_MEMORY_NAME, sizeof(uint16_t));
    memory_mapper.write(port_bytes);
    QByteArray read_data = memory_mapper.read();
    if (!read_data.isEmpty())
    {
        uint16_t read_port = int_converter.fromUnit16Bytes(read_data);
        qDebug() << "Port read from shared memory:" << read_port;
    }
    else
    {
        qDebug() << "No data read from shared memory.";
    }
}
