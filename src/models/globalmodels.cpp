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
    double sizeInMB = static_cast<double>(size) / (1024 * 1024);
    emit modFileSize(sizeInMB);
}

void GlobalModels::onBytesDownloaded(qint64 size) {
    double sizeInMB = static_cast<double>(size) / (1024 * 1024);
    emit downloadProgress(sizeInMB);
}

QString GlobalModels::getLocalPath(const QUrl &path) {
    return path.toLocalFile();
}

void GlobalModels::downloadRR() {
    emit showDlProgress();
    m_updater->checkAndUpdate();
}

void GlobalModels::handleShutdown() {
    m_gamesModel->saveFilesToSettings();
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
    connect(m_updater, &RetroRewind::Updater::bytesDownloaded, this, &GlobalModels::onBytesDownloaded);
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

