#include "updater.h"
#include "unzipper.h"
#include "extern/qmicroz/src/qmicroz.h"
#include <QDir>
#include <QDebug>
#include <algorithm>

namespace RetroRewind {

Updater::Updater(PresetParser *presetParser, QObject *parent)
    : parser(presetParser), QObject(parent) {
    unzipper = new Unzipper;
    unzipper->moveToThread(&workerThread);
    
    connect(this, &Updater::unzipFile, unzipper, &Unzipper::unzip);
    connect(unzipper, &Unzipper::unzipStatus, this, &Updater::handleUnzip);
    connect(&workerThread, &QThread::finished, unzipper, &QObject::deleteLater);
    workerThread.start();

    getVersion();
}

Updater::~Updater() {
    if (m_file) {
        m_file->close();
        delete m_file;
    }
    if (update_file) {
        update_file->close();
        delete update_file;
    }
    if (zip_reply) zip_reply->deleteLater();
    if (update_reply) update_reply->deleteLater();

    workerThread.quit();
    workerThread.wait();
    // No need to delete unzipper; handled by deleteLater
}

void Updater::checkAndUpdate() {
    QString currentVersion = getCurrentVersion();
    qDebug() << "Current version:" << currentVersion;

    if (currentVersion.isEmpty() || compareVersions(currentVersion, "3.2.6") < 0) {
        qDebug() << "Base mod or old version detected, reinstalling";
        getFileSize(BASE_MOD_URL);
        downloadBaseMod();
    } else if (compareVersions(currentVersion, version) < 0) {
        qDebug() << "Update required from" << currentVersion << "to" << version;
        fetchDeleteList();
    } else {
        qDebug() << "RetroRewind is up to date!";
        emit modInstallFinished();
        parseAndSaveXml();
    }
}

void Updater::downloadBaseMod() {
    emit downloadingBaseMod();

    QUrl url(BASE_MOD_URL);
    QString fileName = url.fileName();
    if (fileName.isEmpty()) {
        qDebug() << "Failed to extract file name from URL:" << BASE_MOD_URL;
        fileName = "RetroRewind.zip";
    }

    QNetworkRequest req(url);
    zip_reply = manager.get(req);
    connect(zip_reply, &QNetworkReply::readyRead, this, &Updater::onChunkDownloaded);
    connect(zip_reply, &QNetworkReply::downloadProgress, this, &Updater::onDownloadProgress);
    connect(zip_reply, &QNetworkReply::finished, this, &Updater::onBaseModDownloaded);

    m_file = new QFile(fileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << fileName;
        delete m_file;
        m_file = nullptr;
    }
}

void Updater::handleBaseGame() {
    QDir dir(BASE_PATH);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qDebug() << "Failed to create folder";
            return;
        }
    } else {
        QFileInfoList files = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        for (const auto &f : files) {
            QString fpath = f.absoluteFilePath();
            if (f.isFile()) {
                if (!QFile::remove(fpath)) {
                    qDebug() << "Failed to remove file:" << fpath;
                    return;
                }
            } else if (f.isDir()) {
                QDir sdir(fpath);
                sdir.removeRecursively();
            }
        }
    }
    extractFolders();
}

void Updater::extractFolders() {
    emit unzippingBaseGame();
    QString fileName = QUrl(BASE_MOD_URL).fileName();
    if (fileName.isEmpty()) {
        fileName = "RetroRewind.zip";
    }
    emit unzipFile(fileName, BASE_PATH, UpdateType::BaseGame);
}

void Updater::onBaseModDownloaded() {
    if (zip_reply->error() == QNetworkReply::NoError) {
        qDebug() << "Base mod downloaded";
        if (m_file) {
            m_file->flush();
            m_file->close();
            delete m_file;
            m_file = nullptr;
        }
        handleBaseGame();
    } else {
        qDebug() << "Download failed:" << zip_reply->errorString();
        abortUpdate("Base mod download failed");
    }
    zip_reply->deleteLater();
    zip_reply = nullptr;
}

void Updater::fetchDeleteList() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleDeleteListReq);
    QNetworkRequest req{QUrl(DELETE_URL)};
    manager.get(req);

    qDebug() << "hello";
}

void Updater::handleDeleteListReq(QNetworkReply *reply) {
    deleteList.clear();
    if (reply->error() == QNetworkReply::NoError) {
        QString textData = QString::fromUtf8(reply->readAll());
        QStringList lines = textData.split("\n", Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                deleteList.append({parts[0], parts[1]});
            }
        }
        qDebug() << "Fetched delete list with" << deleteList.size() << "entries";
    } else {
        qDebug() << "Failed to fetch delete list:" << reply->errorString();
        abortUpdate("Failed to fetch delete list");
    }
    reply->deleteLater();
    fetchUpdateList();
}

void Updater::fetchUpdateList() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleUpdateListReq);
    QNetworkRequest req{QUrl(VERSION_URL)};
    manager.get(req);
}

bool Updater::applyFileDeletions(const QString &currentVersion, const QString &targetVersion,
                                const QList<DeleteInfo> &deleteList) {
    QList<DeleteInfo> deletionsToApply;
    QList<DeleteInfo> sortedDeleteList = deleteList;
    std::sort(sortedDeleteList.begin(), sortedDeleteList.end(),
              [this](const DeleteInfo &a, const DeleteInfo &b) {
                  return compareVersions(b.version, a.version) > 0;
              });

    for (const auto &deletion : sortedDeleteList) {
        if (compareVersions(deletion.version, currentVersion) > 0 &&
            compareVersions(deletion.version, targetVersion) <= 0) {
            deletionsToApply.append(deletion);
        }
    }

    std::sort(deletionsToApply.begin(), deletionsToApply.end(),
              [this](const DeleteInfo &a, const DeleteInfo &b) {
                  return compareVersions(a.version, b.version) < 0;
              });

    for (const auto &file : deletionsToApply) {
        QString filePath = QDir(RIIVOLUTION_PATH).filePath(file.path.mid(1));
        QFileInfo fileInfo(filePath);
        QString resolvedPath = fileInfo.absoluteFilePath();

        if (!resolvedPath.startsWith(RIIVOLUTION_PATH, Qt::CaseInsensitive) || filePath.contains("..")) {
            qDebug() << "Invalid file path detected:" << resolvedPath;
            return false;
        }

        qDebug() << "Deleting file" << file.path << "for version" << file.version;
        if (fileInfo.exists()) {
            if (fileInfo.isFile()) {
                if (!QFile::remove(filePath)) {
                    qDebug() << "Failed to delete file:" << filePath;
                    return false;
                }
            } else if (fileInfo.isDir()) {
                QDir dir(filePath);
                if (!dir.removeRecursively()) {
                    qDebug() << "Failed to delete directory:" << filePath;
                    return false;
                }
            }
            qDebug() << "Deleted:" << filePath;
        }
    }
    return true;
}

QList<Updater::UpdateInfo> Updater::getUpdatesToApply(const QString &currentVersion,
                                                     const QList<UpdateInfo> &allUpdates) {
    QList<UpdateInfo> updatesToApply;
    QList<UpdateInfo> sortedUpdates = allUpdates;
    std::sort(sortedUpdates.begin(), sortedUpdates.end(),
              [this](const UpdateInfo &a, const UpdateInfo &b) {
                  return compareVersions(b.version, a.version) > 0;
              });

    for (const auto &update : sortedUpdates) {
        if (compareVersions(update.version, currentVersion) > 0) {
            updatesToApply.prepend(update);
        } else {
            break;
        }
    }
    std::reverse(updatesToApply.begin(), updatesToApply.end());
    return updatesToApply;
}

void Updater::handleUpdateListReq(QNetworkReply *reply) {
    QList<UpdateInfo> allUpdates;
    if (reply->error() == QNetworkReply::NoError) {
        QString textData = QString::fromUtf8(reply->readAll());
        QStringList lines = textData.split("\n", Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            QStringList parts = line.split(" ", Qt::SkipEmptyParts);
            if (parts.size() >= 4) {
                allUpdates.append({parts[0], parts[1], parts[2], parts[3]});
            }
        }
        qDebug() << "Fetched update list with" << allUpdates.size() << "entries";

        QString currentVersion = getCurrentVersion();
        pendingUpdates = getUpdatesToApply(currentVersion, allUpdates);

        if (!applyFileDeletions(currentVersion, version, deleteList)) {
            abortUpdate("Failed to apply file deletions");
            return;
        }

        emit numOfUpdates(pendingUpdates.size());
        currentUpdateNum = 1;
        if (!pendingUpdates.isEmpty()) {
            emit installingUpdate(currentUpdateNum);
            getFileSize(pendingUpdates.first().url);
            downloadAndApplyUpdate(pendingUpdates.first());
        } else {
            updateVersionFile(version);
            parseAndSaveXml();
        }
    } else {
        qDebug() << "Failed to fetch update list:" << reply->errorString();
        abortUpdate("Failed to fetch update list");
    }
    reply->deleteLater();
}

void Updater::downloadAndApplyUpdate(const UpdateInfo &update) {
    QUrl url(update.url);
    QString fileName = url.fileName();
    if (fileName.isEmpty()) {
        fileName = "update_" + update.version + ".zip";
    }

    QNetworkRequest req(url);
    update_reply = manager.get(req);
    connect(update_reply, &QNetworkReply::readyRead, this, &Updater::onChunkDownloaded);
    connect(update_reply, &QNetworkReply::downloadProgress, this, &Updater::onDownloadProgress);
    connect(update_reply, &QNetworkReply::finished, this, &Updater::onUpdateDownloaded);

    update_file = new QFile(fileName);
    if (!update_file->open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open update file for writing:" << fileName;
        delete update_file;
        update_file = nullptr;
    }
}

void Updater::onUpdateDownloaded() {
    if (update_reply->error() == QNetworkReply::NoError) {
        qDebug() << "Update downloaded";
        if (update_file) {
            update_file->flush();
            update_file->close();
            QString fileName = update_file->fileName();
            delete update_file;
            update_file = nullptr;
            emit unzipFile(fileName, BASE_PATH, UpdateType::Update);
        }
    } else {
        qDebug() << "Update download failed:" << update_reply->errorString();
        abortUpdate("Update download failed");
    }
    update_reply->deleteLater();
    update_reply = nullptr;

    processNextUpdate(); // Process the next update in the queue
}

void Updater::processNextUpdate() {
    if (pendingUpdates.isEmpty()) {
        qDebug() << "RetroRewind fully updated";
        updateVersionFile(version);
        emit modInstallFinished();
        parseAndSaveXml();
        return;
    }

    pendingUpdates.removeFirst(); // Remove the completed update
    if (!pendingUpdates.isEmpty()) {
        emit installingUpdate(++currentUpdateNum);
        getFileSize(pendingUpdates.first().url);
        downloadAndApplyUpdate(pendingUpdates.first());
    } else {
        updateVersionFile(version);
        parseAndSaveXml();
    }
}

void Updater::getVersion() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleVersionReq);
    QNetworkRequest req{QUrl(VERSION_URL)};
    manager.get(req);
}

QString Updater::getCurrentVersion() {
    QFile file(VERSION_FILE);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString v = in.readAll().trimmed();
        file.close();
        return v;
    }
    return "";
}

void Updater::handleVersionReq(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString textData = QString::fromUtf8(reply->readAll());
        QStringList lines = textData.split("\n", Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            qDebug() << "Parsed line:" << line;
        }

        version = lines.last().split(" ")[0];
        qDebug() << "Latest version:" << version;
    } else {
        qDebug() << "Failed to fetch version:" << reply->errorString();
        abortUpdate("Failed to fetch version");
    }
    reply->deleteLater();
}

int Updater::compareVersions(const QString &v1, const QString &v2) {
    QStringList parts1 = v1.split('.');
    QStringList parts2 = v2.split('.');
    int maxParts = qMax(parts1.size(), parts2.size());

    for (int i = 0; i < maxParts; ++i) {
        int p1 = (i < parts1.size()) ? parts1[i].toInt() : 0;
        int p2 = (i < parts2.size()) ? parts2[i].toInt() : 0;
        if (p1 != p2) return p1 - p2;
    }
    return 0;
}

void Updater::handleUnzip(bool success, UpdateType type) {
    if (success != true) {
        abortUpdate("unzip error, aborting update");
        return;
    }
    if (type == UpdateType::BaseGame) {
        emit fetchingUpdates();
        checkAndUpdate();
    }
}

void Updater::getFileSize(const QString &url) {
    QNetworkRequest headRequest{QUrl(url)};
    QNetworkReply *headReply = manager.head(headRequest);

    connect(headReply, &QNetworkReply::finished, this, [=]() {
        if (headReply->error() == QNetworkReply::NoError) {
            qint64 fileSize = headReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
            qDebug() << "File size:" << fileSize << "bytes";
            emit updateFileSize(fileSize);
        } else {
            qDebug() << "HEAD request failed:" << headReply->errorString();
        }
        headReply->deleteLater();
    });
}

void Updater::updateVersionFile(const QString &newVersion) {
    QFile file(VERSION_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << newVersion;
        file.close();
        parseAndSaveXml();
        qDebug() << "Updated version to:" << newVersion;
    } else {
        qDebug() << "Failed to update version file";
    }
}

void Updater::abortUpdate(const QString &reason) {
    qDebug() << "Update aborted:" << reason;
    pendingUpdates.clear(); // Clear queue to stop further updates
    // TODO: Add UI notification (e.g., QMessageBox)
}

void Updater::onChunkDownloaded() {
    QFile *targetFile = (zip_reply && m_file) ? m_file : update_file;
    QNetworkReply *reply = (zip_reply) ? zip_reply : update_reply;
    if (targetFile && reply) {
        targetFile->write(reply->readAll());
    } else {
        qDebug() << "Error: No valid file or reply for chunk download";
    }
}

void Updater::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    qDebug() << "Download progress:" << bytesReceived << "/" << bytesTotal;
    emit bytesDownloaded(bytesReceived);
}

void Updater::parseAndSaveXml() {
    qDebug() << "Parsing XML";
    QFileInfo fileInfo(XML_PATH);
    QString absoluteXmlPath = fileInfo.absoluteFilePath();

    QDir parentDir = QDir(absoluteXmlPath);
    parentDir.cdUp();
    parentDir.cdUp();
    QString preset_path = parentDir.absolutePath();

    
    parser->parseRiivoXML(QUrl::fromLocalFile(absoluteXmlPath), "RetroRewind");
}

} // namespace RetroRewind