#include "gamesmodel.h"

#include <QSettings>


GamesModel::GamesModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadFilesFromSettings();
}

int GamesModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return m_files.size();
}

QVariant GamesModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_files.size())
        return QVariant();

    const FileEntry &entry = m_files.at(index.row());
    switch (role) {
    case FileNameRole:
        return entry.fileName;
    case FilePathRole:
        return entry.filePath;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> GamesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileNameRole] = "fileName";
    roles[FilePathRole] = "filePath";
    return roles;
}

void GamesModel::saveFilesToSettings() {
    QSettings settings;
    settings.beginGroup("GamesModel");
    settings.beginWriteArray("files");
    for (int i = 0; i < m_files.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("fileName", m_files[i].fileName);
        settings.setValue("filePath", m_files[i].filePath);
    }
    settings.endArray();
    settings.endGroup();
}

void GamesModel::loadFilesFromSettings() {
    QSettings settings;
    settings.beginGroup("GamesModel");
    int size = settings.beginReadArray("files");
    m_files.clear();
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        FileEntry entry;
        entry.fileName = settings.value("fileName").toString();
        entry.filePath = settings.value("filePath").toString();
        m_files.append(entry);
    }
    settings.endArray();
    settings.endGroup();
    // Notify views that the model has changed
    beginResetModel();
    endResetModel();
}

void GamesModel::addFile(const QString &fileName, const QString &filePath) {
    beginInsertRows(QModelIndex(), m_files.size(), m_files.size());
    m_files.append({fileName, filePath});
    endInsertRows();
}

void GamesModel::updateFile(int index, const QString &fileName, const QString &filePath) {
    if(index >= m_files.size() || index < 0){
        return;
    }

    FileEntry entry = {};
    entry.fileName = fileName;
    entry.filePath = filePath;



    beginResetModel();
    m_files[index] = entry;
    endResetModel();
}

QString GamesModel::getFilePath(int index) {
    if(index < 0 || index >= m_files.size()){
        return "";
    }

    return m_files[index].filePath;

}

void GamesModel::clear() {
    beginResetModel();
    m_files.clear();
    endResetModel();
}