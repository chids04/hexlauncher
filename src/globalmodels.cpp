#include "globalmodels.h"
#include "updater.h"


// GlobalModels &GlobalModels::instance()
// {
//     static GlobalModels globalModels;
//     return globalModels;
// }

// GlobalModels *GlobalModels::create(QQmlEngine *qmlEngine, QJSEngine *)
// {
//     qDebug() << "in create function";
//     return &GlobalModels::instance();
// }


GamePresetModel *GlobalModels::gamePresetModel() const
{
    return m_gamePresetModel;
}

void GlobalModels::setGamePresetModel(GamePresetModel *newGamePresetModel)
{
    m_gamePresetModel = newGamePresetModel;
}

PresetParser *GlobalModels::presetParser() const
{
    return m_presetParser;
}

void GlobalModels::setPresetParser(PresetParser *newPresetParser)
{
    m_presetParser = newPresetParser;
}

void GlobalModels::hello()
{
    qDebug() << "hello";
}

void GlobalModels::onUpdaterFileSize(qint64 size) {
    qint64 sizeInMB = size / (1024 * 1024);
    QString m_fileSizeMB = QString::number(sizeInMB, 'f', 2) + " MB";

    emit modFileSize(m_fileSizeMB);
}

void GlobalModels::onBytesDownloaded(qint64 size) {
    qint64 sizeInMB = size / (1024 * 1024);
    QString m_fileSizeMB = QString::number(sizeInMB, 'f', 2) + " MB";

    emit downloadProgress(m_fileSizeMB);
}

QString GlobalModels::getLocalPath(const QUrl &path) {
    return path.toLocalFile();
}


OptionModel *GlobalModels::optionModel() const
{
    return m_optionModel;
}

void GlobalModels::setOptionModel(OptionModel *newOptionModel)
{
    if (m_optionModel == newOptionModel)
        return;
    m_optionModel = newOptionModel;
    emit optionModelChanged();
}

RetroRewind::Updater *GlobalModels::updater() const {
    return m_updater;
}

void GlobalModels::setUpdater(RetroRewind::Updater *newUpdater) {
    if(newUpdater == m_updater)    {
        return;
    }
    m_updater = newUpdater;
    connect(m_updater, &RetroRewind::Updater::updateFileSize, this, &GlobalModels::onUpdaterFileSize);
    emit updaterChanged();
}

GamesModel *GlobalModels::gamesModel() const {
    return m_gamesModel;
}

void GlobalModels::setGamesModel(GamesModel *newGamesModel) {
    if(newGamesModel == m_gamesModel){
        return;
    }

    m_gamesModel = newGamesModel;
    emit gamesModelChanged();
}

