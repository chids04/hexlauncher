#include "globalmodels.h"


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
