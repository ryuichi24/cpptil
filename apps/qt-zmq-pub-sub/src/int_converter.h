#ifndef INT_CONVERTER_H
#define INT_CONVERTER_H

#include <QByteArray>

class IntConverter
{
public:
    enum class ByteOrder
    {
        BigEndian,
        LittleEndian
    };

    IntConverter(ByteOrder order);

    // to
    QByteArray toUnit16Bytes(uint16_t value) const;

    // from
    uint16_t fromUnit16Bytes(const QByteArray &bytes) const;

private:
    ByteOrder m_byteOrder;

    // to
    template <typename T>
    QByteArray toBytes(T value) const;

    // from
    template <typename T>
    T fromBytes(const QByteArray &bytes) const;
};

#endif /* INT_CONVERTER_H */