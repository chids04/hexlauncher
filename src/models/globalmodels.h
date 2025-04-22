#ifndef GLOBALMODELS_H
#define GLOBALMODELS_H

#include "presetparser.h"
#include "gamepresetmodel.h"
#include "optionmodel.h"
#include "updater.h"
#include "gamesmodel.h"

#include <QObject>
#include <QQmlEngine>
#include <qtmetamacros.h>
#include <QUrl>

class GlobalModels : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

    Q_PROPERTY(PresetParser *presetParser READ presetParser WRITE setPresetParser NOTIFY presetParserChanged)
    Q_PROPERTY(GamePresetModel *gamePresetModel READ gamePresetModel WRITE setGamePresetModel NOTIFY gamePresetModelChanged)
    Q_PROPERTY(OptionModel *optionModel READ optionModel WRITE setOptionModel NOTIFY optionModelChanged)
    Q_PROPERTY(RetroRewind::Updater *updater READ updater WRITE setUpdater NOTIFY updaterChanged)
    Q_PROPERTY(GamesModel *gamesModel READ gamesModel WRITE setGamesModel NOTIFY gamesModelChanged)


public:
    inline static GlobalModels *s_singletonInstance = nullptr;

    GamePresetModel *gamePresetModel() const;
    void setGamePresetModel(GamePresetModel *newGamePresetModel);

    PresetParser *presetParser() const;
    void setPresetParser(PresetParser *newPresetParser);

    OptionModel *optionModel() const;
    void setOptionModel(OptionModel *newOptionModel);

    RetroRewind::Updater *updater() const;
    void setUpdater(RetroRewind::Updater *newUpdater);

    GamesModel *gamesModel() const;
    void setGamesModel(GamesModel *newGamesModel);

public slots:
    void hello();
    void onUpdaterFileSize(qint64 size);
    void onBytesDownloaded(qint64 size);

    QString getLocalPath(const QUrl &path);
    void downloadRR();
    void handleShutdown();

signals:
    void gamePresetModelChanged();
    void presetParserChanged();
    void optionModelChanged();
    void updaterChanged();
    void gamesModelChanged();
    void modFileSize(double size);
    void downloadProgress(double size);
    void showDlProgress();

private:
    GamePresetModel *m_gamePresetModel = nullptr;
    PresetParser *m_presetParser = nullptr;
    OptionModel *m_optionModel = nullptr;
    RetroRewind::Updater *m_updater = nullptr;
    GamesModel *m_gamesModel = nullptr;
};

#endif // GLOBALMODELS_H
