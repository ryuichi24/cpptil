#include <csignal>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

QFile file;
uchar *data = nullptr;

void cleanup()
{
    if (data)
    {
        file.unmap(data);
        data = nullptr;
    }
    if (file.isOpen())
    {
        file.close();
    }
    if (QFile::exists(file.fileName()))
    {
        QFile::remove(file.fileName());
        qDebug() << "Shared memory file deleted.";
    }
}

void signalHandler(int signum)
{
    qDebug() << "Interrupt signal (" << signum << ") received.";
    cleanup();
    exit(signum);
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();

    if (args.size() < 2)
    {
        qDebug().noquote() << QString("Error: Missing shared memory name argument.\n").arg(args[0]);
        qDebug().noquote() << QString("Usage: %1 <memory_mapped_file_name>").arg(args[0]);
        exit(EXIT_FAILURE);
    }

    const QString MEMORY_MAPPED_FILE = args.at(1);

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString fileName = tempPath + QDir::separator() + MEMORY_MAPPED_FILE;
    qDebug() << "Using shared memory file:" << fileName.toStdString();
    file.setFileName(fileName);

    bool opened = file.open(QIODevice::ReadWrite);
    if (!opened)
    {
        qCritical() << "Failed to open file:" << file.errorString();
        return EXIT_FAILURE;
    };

    // Ensure the file is 4 bytes in size
    if (file.size() < sizeof(quint32))
    {
        file.resize(sizeof(quint32));
    }

    // Map the file into memory
    data = file.map(0, sizeof(quint32));
    if (!data)
    {
        qCritical() << "Failed to map file into memory.";
        return EXIT_FAILURE;
    }

    // Write the port number
    quint32 port = 51111;
    memcpy(data, &port, sizeof(quint32));

    qDebug() << "Port number written to shared memory:" << port;
    qDebug() << "Press Ctrl+C to exit.";

    int ret = app.exec();
    cleanup();
    return ret;
}
