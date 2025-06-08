#include <QByteArray>
#include <QDataStream>
#include <QIODevice>

#include "int_converter.h"

IntConverter::IntConverter(ByteOrder order)
    : m_byteOrder(order)
{
}

template <typename T>
QByteArray IntConverter::toBytes(T value) const
{
    QByteArray buffer(sizeof(T), Qt::Uninitialized);
    QDataStream stream(&buffer, QIODevice::WriteOnly);

    if (m_byteOrder == ByteOrder::BigEndian)
    {
        stream.setByteOrder(QDataStream::BigEndian);
    }
    else
    {
        stream.setByteOrder(QDataStream::LittleEndian);
    }

    stream << value;

    return buffer;
}

QByteArray IntConverter::toUnit16Bytes(uint16_t value) const
{
    return toBytes<uint16_t>(value);
}

template <typename T>
T IntConverter::fromBytes(const QByteArray &bytes) const
{
    if (bytes.size() != (int)sizeof(T))
    {
        throw std::runtime_error(QString("Buffer size %1 does not match size %2").arg(bytes.size()).arg(sizeof(T)).toStdString());
    }

    QDataStream readStream(bytes);

    if (m_byteOrder == ByteOrder::BigEndian)
    {
        readStream.setByteOrder(QDataStream::BigEndian);
    }
    else
    {
        readStream.setByteOrder(QDataStream::LittleEndian);
    }

    T value;
    readStream >> value;
    return value;
}

uint16_t IntConverter::fromUnit16Bytes(const QByteArray &bytes) const
{
    return fromBytes<uint16_t>(bytes);
}