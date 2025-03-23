#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <qqmlintegration.h>

#include "presetparser.h"

namespace RetroRewind {

class Updater : public QObject {
    Q_OBJECT
    QML_ANONYMOUS

public:
    explicit Updater(PresetParser *presetParser, QObject *parent = nullptr);
    ~Updater();

    void getVersion();          // Fetch the latest version from the server
    void downloadBaseMod();     // Download the base mod ZIP
    void handleBaseGame();      // Prepare the base game directory
    void extractFolders();      // Extract specific folders from the ZIP

public slots:
    void checkAndUpdate();      // Check if update is needed and apply it

signals:
    void updateFileSize(qint64 size);
    void bytesDownloaded(qint64 size);

private slots:
    void handleVersionReq(QNetworkReply *reply);       // Handle version response
    void onChunkDownloaded();                          // Handle download chunks
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal); // Track download progress
    void onBaseModDownloaded();                        // Handle base mod download completion
    void handleDeleteListReq(QNetworkReply *reply);    // Handle deletion list response
    void handleUpdateListReq(QNetworkReply *reply);    // Handle update list response
    void onUpdateChunkDownloaded();                    // Handle update download chunks
    void onUpdateDownloadProgress(qint64 bytesReceived, qint64 bytesTotal); // Track update progress
    void onUpdateDownloaded();                         // Handle update download completion

private:
    QNetworkAccessManager manager;
    QNetworkReply *zip_reply = nullptr;
    QNetworkReply *update_reply = nullptr;
    QFile *m_file = nullptr;
    QFile *update_file = nullptr;
    QString version;  // Current/latest version fetched from server
    QThread *unzipThread;

    PresetParser *parser;

    const QString BASE_MOD_URL = "http://update.zplwii.xyz:8000/RetroRewind/zip/RetroRewind.zip";
    const QString VERSION_URL = "http://update.zplwii.xyz:8000/RetroRewind/RetroRewindVersion.txt";
    const QString DELETE_URL = "http://update.zplwii.xyz:8000/RetroRewind/RetroRewindDelete.txt"; // Adjust if needed
    const QString BASE_PATH = "presets/retro-rewind/dolphin/RetroRewind6/";
    const QString RIIVOLUTION_PATH = BASE_PATH + "riivolution/";
    const QString XML_PATH = RIIVOLUTION_PATH + "RetroRewind6.xml";
    const QString VERSION_FILE = BASE_PATH + "RetroRewind6/version.txt";

    // Helper structs
    struct UpdateInfo {
        QString version;
        QString url;
        QString path;
        QString description;
    };

    struct DeleteInfo {
        QString version;
        QString path;
    };

    QList<DeleteInfo> deleteList;
    // Constants

    // Helper methods
    void getFileSize();
    QString getCurrentVersion();  // Read current version from local file
    int compareVersions(const QString &v1, const QString &v2);  // Compare version strings
    void fetchDeleteList();       // Fetch list of files to delete
    void fetchUpdateList();       // Fetch list of available updates
    bool applyFileDeletions(const QString &currentVersion, const QString &targetVersion,
                           const QList<DeleteInfo> &deleteList);  // Apply deletions
    QList<UpdateInfo> getUpdatesToApply(const QString &currentVersion,
                                        const QList<UpdateInfo> &allUpdates);  // Filter updates
    void downloadAndApplyUpdate(const UpdateInfo &update);  // Download and apply a single update
    void updateVersionFile(const QString &newVersion);      // Update local version file
    void abortUpdate(const QString &reason);                // Handle update failure
    void parseAndSaveXml();
};

}