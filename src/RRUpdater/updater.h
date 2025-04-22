#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <qqmlintegration.h>

#include "presetparser.h"
#include "unzipper.h"
#include "common.h"

namespace RetroRewind {

class Updater : public QObject {
    Q_OBJECT
    QML_ANONYMOUS

public:
    explicit Updater(PresetParser *presetParser, QObject *parent = nullptr);
    ~Updater();

    // Public Methods
    void getVersion();
    void downloadBaseMod();
    void handleBaseGame();
    void extractFolders();

public slots:
    void checkAndUpdate();

signals:
    void updateFileSize(qint64 size);
    void bytesDownloaded(qint64 bytesReceived);
    void downloadingBaseMod();
    void unzipFile(const QString &src, const QString &dst, UpdateType type);
    void unzippingBaseGame();
    void fetchingUpdates();
    void installingUpdate(int updateNum);
    void numOfUpdates(int size);
    void modInstallFinished();

private slots:
    void handleVersionReq(QNetworkReply *reply);
    void handleDeleteListReq(QNetworkReply *reply);
    void handleUpdateListReq(QNetworkReply *reply);
    void onChunkDownloaded();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onBaseModDownloaded();
    void onUpdateDownloaded();
    void handleUnzip(bool success, UpdateType type);

private:


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
    QNetworkAccessManager manager;
    QNetworkReply *zip_reply = nullptr;
    QNetworkReply *update_reply = nullptr;
    QFile *m_file = nullptr;
    QFile *update_file = nullptr;
    Unzipper *unzipper = nullptr;
    QString version;
    QThread workerThread;
    PresetParser *parser;
    QList<DeleteInfo> deleteList;
    QList<UpdateInfo> pendingUpdates; // New: Queue for sequential updates
    int currentUpdateNum = 0;        // New: Track current update number

    const QString BASE_MOD_URL = "http://update.zplwii.xyz:8000/RetroRewind/zip/RetroRewind.zip";
    const QString VERSION_URL = "http://update.zplwii.xyz:8000/RetroRewind/RetroRewindVersion.txt";
    const QString DELETE_URL = "http://update.zplwii.xyz:8000/RetroRewind/RetroRewindDelete.txt";
    const QString BASE_PATH = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/presets/Retro Rewind/dolphin/RetroRewind6/";
    const QString RIIVOLUTION_PATH = BASE_PATH + "riivolution/";
    const QString XML_PATH = RIIVOLUTION_PATH + "RetroRewind6.xml";
    const QString VERSION_FILE = BASE_PATH + "RetroRewind6/version.txt";

    void getFileSize(const QString &url);
    QString getCurrentVersion();
    int compareVersions(const QString &v1, const QString &v2);
    void fetchDeleteList();
    void fetchUpdateList();
    bool applyFileDeletions(const QString &currentVersion, const QString &targetVersion,
                            const QList<DeleteInfo> &deleteList);
    QList<UpdateInfo> getUpdatesToApply(const QString &currentVersion,
                                        const QList<UpdateInfo> &allUpdates);
    void downloadAndApplyUpdate(const UpdateInfo &update);
    void updateVersionFile(const QString &newVersion);
    void abortUpdate(const QString &reason);
    void parseAndSaveXml();
    void processNextUpdate(); // New: Process the next update in the queue
};

}
