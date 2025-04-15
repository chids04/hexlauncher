#include "updater.h"
#include "extern/qmicroz/src/qmicroz.h"
#include <QDir>
#include <QDebug>

namespace RetroRewind {

Updater::Updater(PresetParser *presetParser, QObject *parent) : parser(presetParser), QObject(parent) {

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
}

void Updater::parseAndSaveXml() {
    qDebug() << "parsing xml";
    QFileInfo fileInfo(XML_PATH);
    QString absoluteXmlPath = fileInfo.absoluteFilePath();

    QDir parentDir = QDir(absoluteXmlPath);
    parentDir.cdUp(); // Go up one level
    parentDir.cdUp(); // Go up another level
    QString preset_path = parentDir.absolutePath();
    
    parser->parseRiivoXML(QUrl::fromLocalFile(absoluteXmlPath), "Retro Rewind");
    //parser->savePreset("Retro Rewind", QUrl::fromLocalFile(preset_path));
}

void Updater::getVersion() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleVersionReq);
    QNetworkRequest req((QUrl(VERSION_URL)));
    manager.get(req);
}

void Updater::downloadBaseMod() {
    QNetworkRequest req((QUrl(BASE_MOD_URL)));
    zip_reply = manager.get(req);
    connect(zip_reply, &QNetworkReply::readyRead, this, &Updater::onChunkDownloaded);
    connect(zip_reply, &QNetworkReply::downloadProgress, this, &Updater::onDownloadProgress);
    connect(zip_reply, &QNetworkReply::finished, this, &Updater::onBaseModDownloaded);

    m_file = new QFile("RetroRewind.zip");
    if (!m_file->open(QIODevice::WriteOnly)) {
        delete m_file;
        m_file = nullptr;
        qDebug() << "Failed to open zip for writing";
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
        // Clean directory ready for new game
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
    QMicroz qmz("RetroRewind.zip");
    ZipContents zip_contents;

    if (qmz) {
        qmz.setOutputFolder(BASE_PATH);
        zip_contents = qmz.contents();
    } else {
        qDebug() << "Failed to open zip";
        return;
    }

    ZipContents::const_iterator it;
    for (it = zip_contents.constBegin(); it != zip_contents.constEnd(); ++it) {
        const QString file_name = it.value();
        qDebug() << "Extracting:" << file_name;
        if (file_name.contains("/")) {
            QString dname = file_name.split("/")[0];
            if (dname == "RetroRewind6" || dname == "riivolution") {
                qmz.extractIndex(it.key());
            }
        }
    }
    QFile::remove("RetroRewind.zip"); // Clean up the downloaded ZIP
    checkAndUpdate(); // Start the update check after base extraction
}

void Updater::handleVersionReq(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QString textData = QString::fromUtf8(responseData);
        QStringList lines = textData.split("\n", Qt::SkipEmptyParts);

        for (const QString &line : lines) {
            qDebug() << "Parsed line:" << line;
        }

        version = lines.last().split(" ")[0]; // Latest version
        qDebug() << "Latest version:" << version;
    } else {
        qDebug() << "Failed to fetch version:" << reply->errorString();
    }
    reply->deleteLater();
}

void Updater::onChunkDownloaded() {
    if (m_file) {
        m_file->write(zip_reply->readAll());
    }

}

void Updater::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    qDebug() << "Download progress:" << bytesReceived << "/" << bytesTotal;

    emit bytesDownloaded(bytesReceived);
}

void Updater::onBaseModDownloaded() {
    if (zip_reply->error() == QNetworkReply::NoError) {
        qDebug() << "Base mod downloaded";
        m_file->flush();
        m_file->close();
        delete m_file;
        m_file = nullptr;
        handleBaseGame();
    } else {
        qDebug() << "Download failed:" << zip_reply->errorString();
    }
    zip_reply->deleteLater();
    zip_reply = nullptr;
}

void Updater::checkAndUpdate() {
    QString currentVersion = getCurrentVersion();
    qDebug() << "Current version:" << currentVersion;

    if (currentVersion.isEmpty() || compareVersions(currentVersion, "3.2.6") < 0) {
        qDebug() << "Base mod or old version detected, reinstalling";
        getFileSize();
        downloadBaseMod();
    } else if (compareVersions(currentVersion, version) < 0) {
        qDebug() << "Update required from" << currentVersion << "to" << version;
        fetchDeleteList(); // Start update process
    } else {
        parseAndSaveXml();
        qDebug() << "RetroRewind is up to date!";
    }
}

QString Updater::getCurrentVersion() {
    QFile file(VERSION_FILE);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString v = in.readAll().trimmed();
        file.close();
        return v;
    }
    return ""; // No version file means not installed
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

void Updater::fetchDeleteList() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleDeleteListReq);
    QNetworkRequest req((QUrl(DELETE_URL)));
    manager.get(req);
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
    }
    reply->deleteLater();
    fetchUpdateList(); // Proceed to fetch updates after deletions
}

void Updater::fetchUpdateList() {
    connect(&manager, &QNetworkAccessManager::finished, this, &Updater::handleUpdateListReq);
    QNetworkRequest req((QUrl(VERSION_URL))); // Reuse VERSION_URL for update list
    manager.get(req);
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
        QList<UpdateInfo> updatesToApply = getUpdatesToApply(currentVersion, allUpdates);

        // Apply deletions first
        if (!applyFileDeletions(currentVersion, version, deleteList)) {
            abortUpdate("Failed to apply file deletions");
            return;
        }

        // Apply updates
        for (const UpdateInfo &update : updatesToApply) {
            downloadAndApplyUpdate(update);
        }
    } else {
        qDebug() << "Failed to fetch update list:" << reply->errorString();
    }
    reply->deleteLater();
}

bool Updater::applyFileDeletions(const QString &currentVersion, const QString &targetVersion,
    const QList<DeleteInfo> &deleteList) {

    QList<DeleteInfo> deletionsToApply;
    // Sort deleteList in descending order by version
    QList<DeleteInfo> sortedDeleteList = deleteList;
    std::sort(sortedDeleteList.begin(), sortedDeleteList.end(),
    [this](const DeleteInfo &a, const DeleteInfo &b) {
        return compareVersions(b.version, a.version) > 0;
    });

    // Filter deletions between currentVersion and targetVersion
    for (const auto &deletion : sortedDeleteList) {
        if (compareVersions(deletion.version, currentVersion) > 0 && compareVersions(deletion.version, targetVersion) <= 0) {
            deletionsToApply.append(deletion);
        }
    }

    // Sort in ascending order for application
    std::sort(deletionsToApply.begin(), deletionsToApply.end(),
        [this](const DeleteInfo &a, const DeleteInfo &b) {
        return compareVersions(a.version, b.version) < 0;
    });

    // Apply deletions
    for (const auto &file : deletionsToApply) {
        QString filePath = QDir(RIIVOLUTION_PATH).filePath(file.path.mid(1)); // Remove leading '/'
        QFileInfo fileInfo(filePath);
        QString resolvedPath = fileInfo.absoluteFilePath();

        // Security check to prevent directory traversal
        if (!resolvedPath.startsWith(RIIVOLUTION_PATH, Qt::CaseInsensitive) || filePath.contains("..")) {
            qDebug() << "Invalid file path detected:" << resolvedPath;
            return false;
        }


        qDebug() << "deleting file" << file.path << "for version" << file.version;

        if (fileInfo.exists()) {
            if (fileInfo.isFile()) {
                if (!QFile::remove(filePath)) {
                    qDebug() << "Failed to delete file:" << filePath;
                    return false;
                }
            } 
            else if (fileInfo.isDir()) {
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
    // Copy the const list to a mutable one for sorting
    QList<UpdateInfo> sortedUpdates = allUpdates;
    std::sort(sortedUpdates.begin(), sortedUpdates.end(),
              [this](const UpdateInfo &a, const UpdateInfo &b) {
                  return compareVersions(b.version, a.version) > 0; // Descending order
              });

    for (const auto &update : sortedUpdates) {
        if (compareVersions(update.version, currentVersion) > 0) {
            updatesToApply.prepend(update); // Add in reverse order
        } else {
            break; // Stop once we hit versions <= currentVersion
        }
    }
    std::reverse(updatesToApply.begin(), updatesToApply.end()); // Ascending order
    return updatesToApply;
}

void Updater::downloadAndApplyUpdate(const UpdateInfo &update) {
    QNetworkRequest req(QUrl(update.url));
    update_reply = manager.get(req);
    connect(update_reply, &QNetworkReply::readyRead, this, &Updater::onUpdateChunkDownloaded);
    connect(update_reply, &QNetworkReply::downloadProgress, this, &Updater::onUpdateDownloadProgress);
    connect(update_reply, &QNetworkReply::finished, this, &Updater::onUpdateDownloaded);

    update_file = new QFile(update.path.split("/").last()); // Use the filename from path
    if (!update_file->open(QIODevice::WriteOnly)) {
        delete update_file;
        update_file = nullptr;
        qDebug() << "Failed to open update file for writing:" << update.path;
    }
}

void Updater::onUpdateChunkDownloaded() {
    if (update_file) {
        update_file->write(update_reply->readAll());
    }
}

void Updater::onUpdateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    qDebug() << "Update progress:" << bytesReceived << "/" << bytesTotal;
}

void Updater::onUpdateDownloaded() {
    if (update_reply->error() == QNetworkReply::NoError) {
        qDebug() << "Update downloaded";
        update_file->flush();
        update_file->close();

        QMicroz qmz(update_file->fileName());
        if (qmz) {
            qmz.setOutputFolder(BASE_PATH);
            ZipContents contents = qmz.contents();
            for (auto it = contents.constBegin(); it != contents.constEnd(); ++it) {
                qmz.extractIndex(it.key());
            }
            QFile::remove(update_file->fileName());
        } else {
            qDebug() << "Failed to open update ZIP";
        }

        delete update_file;
        update_file = nullptr;
        updateVersionFile(version); // Update version after successful application
    } else {
        qDebug() << "Update download failed:" << update_reply->errorString();
    }
    update_reply->deleteLater();
    update_reply = nullptr;
}

//private methods
void Updater::getFileSize() {
    QUrl downloadUrl(BASE_MOD_URL);
    
    QNetworkRequest headRequest(downloadUrl);
    QNetworkReply* headReply = manager.head(headRequest);
    
    // Connect to the finished signal for the HEAD request
    connect(headReply, &QNetworkReply::finished, this, [this, downloadUrl, headReply]() {
        if (headReply->error() == QNetworkReply::NoError) {
            // get the content length from the header
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
    // Add UI notification here if needed (e.g., QMessageBox)
}

} // namespace RetroRewind