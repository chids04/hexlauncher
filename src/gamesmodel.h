#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>
#include <qqmlintegration.h>
#include <QUrl>

struct FileEntry {
    QString fileName;
    QString filePath;
};

class GamesModel : public QAbstractListModel {
    Q_OBJECT
    QML_ANONYMOUS

public:
    enum Roles {
        FileNameRole = Qt::UserRole + 1,
        FilePathRole
    };

    explicit GamesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addFile(const QString &fileName, const QString &filePath);
    void updateFile(int index, const QString &fileName, const QString &filePath);
    QString getFilePath(int index);
    void clear();

private:
    QVector<FileEntry> m_files;
};
