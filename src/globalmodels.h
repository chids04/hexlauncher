#ifndef GLOBALMODELS_H
#define GLOBALMODELS_H

#include "presetparser.h"
#include "gamepresetmodel.h"
#include "optionmodel.h"
#include "updater.h"

#include <QObject>
#include <QQmlEngine>

class GlobalModels : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

    Q_PROPERTY(PresetParser *presetParser READ presetParser WRITE setPresetParser NOTIFY presetParserChanged)
    Q_PROPERTY(GamePresetModel *gamePresetModel READ gamePresetModel WRITE setGamePresetModel NOTIFY gamePresetModelChanged)
    Q_PROPERTY(OptionModel *optionModel READ optionModel WRITE setOptionModel NOTIFY optionModelChanged)
    Q_PROPERTY(RetroRewind::Updater *updater READ updater WRITE setUpdater NOTIFY updaterChanged)


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

public slots:
    void hello();
    void onUpdaterFileSize(qint64 size);
    void onBytesDownloaded(qint64 size);

signals:
    void gamePresetModelChanged();
    void presetParserChanged();
    void optionModelChanged();
    void updaterChanged();
    void modFileSize(const QString &size);
    void downloadProgress(const QString &size);


private:
    GamePresetModel *m_gamePresetModel = nullptr;
    PresetParser *m_presetParser = nullptr;
    OptionModel *m_optionModel = nullptr;
    RetroRewind::Updater *m_updater = nullptr;
};

#endif // GLOBALMODELS_H
