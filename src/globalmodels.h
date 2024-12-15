#ifndef GLOBALMODELS_H
#define GLOBALMODELS_H

#include "presetparser.h"
#include "gamepresetmodel.h"
#include "optionmodel.h"

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


public:
    inline static GlobalModels *s_singletonInstance = nullptr;

    GamePresetModel *gamePresetModel() const;
    void setGamePresetModel(GamePresetModel *newGamePresetModel);

    PresetParser *presetParser() const;
    void setPresetParser(PresetParser *newPresetParser);

    OptionModel *optionModel() const;
    void setOptionModel(OptionModel *newOptionModel);

public slots:
    void hello();

signals:
    void gamePresetModelChanged();

    void presetParserChanged();

    void optionModelChanged();

private:
    GamePresetModel *m_gamePresetModel = nullptr;
    PresetParser *m_presetParser = nullptr;

    OptionModel *m_optionModel = nullptr;
};

#endif // GLOBALMODELS_H
