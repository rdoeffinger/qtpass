#include <QDebug>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QString>
#include <QDir>
#include "util.h"

QProcessEnvironment Util::_env;
bool Util::_envInitialised;

/**
 * @brief Util::initialiseEnvironment
 */
void Util::initialiseEnvironment()
{
    if (!_envInitialised) {
        _env = QProcessEnvironment::systemEnvironment();
#ifdef __APPLE__
        // TODO checks here
        QString path = _env.value("PATH");

        if (!path.contains("/usr/local/MacGPG2/bin") && QFile("/usr/local/MacGPG2/bin").exists()) {
            path += ":/usr/local/MacGPG2/bin";
        }
        if (!path.contains("/usr/local/bin")) {
            path += ":/usr/local/bin";
        }
        _env.insert("PATH", path);
#endif
        _envInitialised = true;
    }
}

/**
 * @brief Util::findPasswordStore
 * @return
 */
QString Util::findPasswordStore()
{
    QString path;
    initialiseEnvironment();
    if (_env.contains("PASSWORD_STORE_DIR")) {
        path = _env.value("PASSWORD_STORE_DIR");
    } else {
        path = QDir::homePath()+"/.password-store/";
    }
    return Util::normalizeFolderPath(path);
}

/**
 * @brief Util::normalizeFolderPath
 * @param path
 * @return
 */
QString Util::normalizeFolderPath(QString path) {
    if (!path.endsWith("/") && !path.endsWith(QDir::separator())) {
        path += '/';
    }
    return path;
}


QString Util::findBinaryInPath(QString binary)
{
    initialiseEnvironment();

    QString ret = "";

    binary.prepend("/");

    if (_env.contains("PATH")) {
        QString path = _env.value("PATH");

        QStringList entries = path.split(':');
        if (entries.length() < 2) {
            entries = path.split(';');
        }

        foreach(QString entry, entries) {
            QScopedPointer<QFileInfo> qfi(new QFileInfo(entry.append(binary)));
            qDebug() << entry;

#ifdef Q_OS_WIN
            if (!qfi->exists()) {
                qfi.reset(new QFileInfo(entry.append(".exe")));
            }
#endif
            if (!qfi->isExecutable()) {
                continue;
            }

            ret = qfi->absoluteFilePath();
            break;
        }
    }

    return ret;
}

/**
 * @brief Util::checkConfig
 * @param passStore
 * @param passExecutable
 * @param gpgExecutable
 * @return
 */
bool Util::checkConfig(QString passStore, QString passExecutable, QString gpgExecutable)
{
    return !QFile(passStore + ".gpg-id").exists() || (!QFile(passExecutable).exists() && !QFile(gpgExecutable).exists());
}
