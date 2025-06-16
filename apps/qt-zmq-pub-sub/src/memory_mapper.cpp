#include "memory_mapper.h"
#include <QStandardPaths>
#include <QDir>

MemoryMapper::MemoryMapper(const QString &id, const qint64 size)
    : m_id(id), m_size(size)
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    m_memPath = QDir(tempDir).filePath(m_id);
    qDebug() << "Using shared memory file:" << m_memPath;
}

MemoryMapper::~MemoryMapper()
{
    dispose();
    qDebug() << "MemoryMapper disposed for ID:" << m_id;
    m_memPath.clear();
    m_id.clear();
    m_size = 0;
}

void MemoryMapper::write(const QByteArray &data)
{
    if (data.size() > m_size)
    {
        qWarning() << "Data size" << data.size() << "exceeds memory size" << m_size;
    }

    auto mem = memMapFileForWrite();

    if (data.size() > m_size)
    {
        qWarning() << "Data size exceeds memory size.";
    }

    memcpy(mem.address, data.constData(), data.size());
     qDebug() << "Successfully wrote" << data.size() << "bytes to memory-mapped file:" << m_memPath;

    unmapFile(&mem);
}

QByteArray MemoryMapper::read() const
{
    auto mem = memMapFileForRead();

    QByteArray data;
    data.resize(m_size);
    memcpy(data.data(), mem.address, m_size);
    if (data.isEmpty())
    {
        qWarning() << "No data read from memory-mapped file:" << m_memPath;
        return QByteArray();
    }

    unmapFile(&mem);

    return data;
}

void MemoryMapper::dispose()
{
    QFile file(m_memPath);
    if (file.exists())
    {
        if (!file.remove())
        {
            qWarning() << "Failed to remove memory-mapped file:" << m_memPath;
        }
        else
        {
            qDebug() << "Memory-mapped file removed:" << m_memPath;
        }
    }
    else
    {
        qDebug() << "Memory-mapped file does not exist:" << m_memPath;
    }
}

MemoryMapper::MemoryMap MemoryMapper::memMapFileForWrite() const
{
    MemoryMap mem;
    mem.file = std::make_unique<QFile>();
    mem.file->setFileName(m_memPath);

    if (!mem.file->open(QIODevice::ReadWrite))
    {
        qWarning() << "Failed to open memory-mapped file for read/write:" << m_memPath;
        return {};
    }

    if (!mem.file->resize(m_size))
    {
        qWarning() << "Failed to resize memory-mapped file:" << m_memPath;
    }

    mem.address = mem.file->map(0, m_size);

    if (!mem.address)
    {
        qWarning() << "Failed to map file into memory:" << m_memPath;
    }

    return mem;
}

MemoryMapper::MemoryMap MemoryMapper::memMapFileForRead() const
{
    MemoryMap mem;
    mem.file = std::make_unique<QFile>();
    mem.file->setFileName(m_memPath);
    if (!mem.file->open(QIODevice::ReadWrite))
    {
        qWarning() << "Failed to open memory-mapped file for read/write:" << m_memPath;
        return {};
    }

    if (!mem.file->resize(m_size))
    {
        qWarning() << "Failed to resize memory-mapped file:" << m_memPath;
    }

    mem.address = mem.file->map(0, m_size);

    if (!mem.address)
    {
        qWarning() << "Failed to map file into memory:" << m_memPath;
    }

    return mem;
}

void MemoryMapper::unmapFile(MemoryMap *mem) const
{
    mem->file->unmap(mem->address);
    mem->file->close();
}
