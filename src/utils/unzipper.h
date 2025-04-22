#pragma once

#include <QObject>
#include <QFile>
#include "extern/qmicroz/src/qmicroz.h"

#include "common.h"

class Unzipper : public QObject {
    
    Q_OBJECT

signals:
    void unzipStatus(bool success, UpdateType type);
    
public slots:
    void unzip(const QString &src, const QString &dst, UpdateType type){
        QMicroz qmz(src);
        ZipContents zip_contents;

        if (qmz) {
            qmz.setOutputFolder(dst);
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

        QFile::remove(src); // Clean up the downloaded ZIP
        emit unzipStatus(true, type);

    };
};