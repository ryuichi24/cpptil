#include <csignal>

#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

#define MEMORY_MAPPED_FILE "qt_shared_memory"

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

QString getConfigDirPath(QString config_dir_name)
{
    QString homeDir = QDir::homePath();
    QString hiddenDirName = config_dir_name;
    QString hiddenDirPath = QDir(homeDir).filePath(hiddenDirName);

    QDir dir;
    if (!dir.exists(hiddenDirPath))
    {
        if (!dir.mkpath(hiddenDirPath))
        {
            qCritical() << "Failed to create hidden directory:" << hiddenDirPath;
            return QString();
        }
#ifdef Q_OS_WIN
        // Set hidden attribute on Windows
        SetFileAttributesW(reinterpret_cast<LPCWSTR>(hiddenDirPath.utf16()), FILE_ATTRIBUTE_HIDDEN);
#endif
    }

    return hiddenDirPath;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    QString homeDir = QDir::homePath();
    QString hiddenDirName = ".cpptil";
    QString hiddenDirPath = getConfigDirPath(hiddenDirName);
    // Get the current working directory
    QString currentDir = QDir::currentPath();
    QString fileName = hiddenDirPath + QDir::separator() + MEMORY_MAPPED_FILE;
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
