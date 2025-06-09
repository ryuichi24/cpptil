#ifndef MEMORY_MAPPER_H
#define MEMORY_MAPPER_H

#include <QtGlobal>
#include <QString>
#include <QFile>

class MemoryMapper
{

    typedef struct MemoryMap
    {
        uchar *address = nullptr;
        std::unique_ptr<QFile> file;
    } MemoryMap;

public:
    MemoryMapper(const QString &id, qint64 size);
    ~MemoryMapper();

    void write(const QByteArray &data);
    QByteArray read() const;
    void dispose();

private:
    QString m_id;
    qint64 m_size;
    QString m_memPath;

    MemoryMap memMapFileForWrite() const;
    MemoryMap memMapFileForRead() const;
    void unmapFile(MemoryMap *mem) const;
};

#endif /* MEMORY_MAPPER_H */