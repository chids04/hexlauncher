#include "gamepresetmodel.h"
#include "optionmodel.h"

#include <QSettings>
#include <QUrl>

GamePresetModel::GamePresetModel(QObject *parent) : QAbstractListModel(parent) {
    QSettings settings;

    // Check if "presets" group exists
    if (!settings.childGroups().contains("presets")) {
        qDebug() << "No presets found in settings.";
        return;
    }

    // Begin reading the "presets" group
    settings.beginGroup("presets");
    const QStringList presetNames = settings.childGroups();

    for (const QString &presetName : presetNames) {
        settings.beginGroup(presetName);

        // Read preset properties
        QString displayName = settings.value("displayName").toString();
        QString sectionName = settings.value("sectionName").toString();
        QString sdRoot = settings.value("sdRoot").toString();
        QString xmlPath = settings.value("xmlPath").toString();
        QString jsonPath = settings.value("jsonPath").toString();
        int gameIndex = settings.value("gameIndex", -1).toInt();

        // Read options
        QVector<std::shared_ptr<OptionItem>> options;
        int size = settings.beginReadArray("options");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            OptionItem option = settings.value("optionItem").value<OptionItem>();
            options.append(std::make_shared<OptionItem>(option));
        }
        settings.endArray();

        // Create the OptionModel and GamePreset
        OptionModel *optionModel = new OptionModel();
        optionModel->setOptions(options);

        GamePreset *gamePreset = new GamePreset(displayName, sdRoot, optionModel, sectionName, xmlPath, gameIndex, jsonPath);

        // Add to the model
        addGamePreset(gamePreset);

        settings.endGroup(); // End current preset group
    }

    settings.endGroup(); // End "presets" group
}

void GamePresetModel::addGamePreset(GamePreset *game_preset)
{
    int row = game_presets.size();
    beginInsertRows(QModelIndex(), row, row);
    game_presets << game_preset;
    endInsertRows();

    qDebug() << "added new preset";

}

QModelIndex GamePresetModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    // Check for a valid row and column
    if (row >= 0 && row < rowCount() && column == 0) {
        return createIndex(row, column);
    } else {
        return QModelIndex(); // Return an invalid index if out of bounds
    }
}

int GamePresetModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return  game_presets.count();
}

QVariant GamePresetModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= game_presets.count()){
        qWarning() << "Invalid index for Option Model";
        return QVariant();
    }

    auto game_preset = game_presets.at(index.row());

    switch(role){
    case DisplayNameRole:
        return game_preset->getDisplayName();
        break;

    case SDRole:
        return game_preset->getRoot();
        break;

    case OptionModelRole:
        return QVariant::fromValue(game_preset->getOptionModel());
        break;

    case SectionNameRole:
        return game_preset->getSectionName();
        break;

    case JsonPathRole:
        return game_preset->getJsonPath();
        break;
    
    case GameIndexRole:
        return game_preset->getGameIndex();
        break;

    default:
        qWarning() << "Invalid role passed to data method of Option Model";
        return QVariant();
        break;
    }
}

QHash<int, QByteArray> GamePresetModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DisplayNameRole] = "display_name";
    roles[SDRole] = "sd_root";
    roles[OptionModelRole] = "option_model";
    roles[JsonPathRole] = "json_path";
    roles[GameIndexRole] = "gameIndex";

    return roles;
}

QVector<GamePreset *> GamePresetModel::getGamePresets() const
{
    return game_presets;
}

void GamePresetModel::updateJsonPath(int index, QString &json_path)
{
    if (index < 0 || index >= game_presets.size()) {
        qWarning() << "Invalid index for updateJsonPath:" << index;
        return;
    }

    GamePreset* game_preset = game_presets[index];
    game_preset->setJsonPath(json_path);

    // Emit dataChanged to notify QML
    QModelIndex modelIndex = this->index(index, 0);
    emit dataChanged(modelIndex, modelIndex, {JsonPathRole});

    qDebug() << "Updated json_path for preset:" << game_preset->getDisplayName() << json_path;
}


void GamePresetModel::setOptionChoice(int choice_idx, QString preset_name, int chosen_choice_idx)
{
    for(auto &preset: game_presets){
        if(preset->getDisplayName() == preset_name){
            //set the chosen choice
            qDebug() << "found preset" << preset_name << "setting option to index" << chosen_choice_idx;
            qDebug() << choice_idx;
            preset->getOptionModel()->getOptionList()[choice_idx]->currentOption = chosen_choice_idx;
            break;
        }
    }
}

bool GamePresetModel::isDuplicate(QString name)
{
    for(auto &preset : game_presets){
        if(preset->getDisplayName() == name){
            return true;
        }
    }

    return false;
}

void GamePresetModel::deletePreset(QString preset_name)
{
    for (int i = 0; i < game_presets.size(); ++i) {
        if (game_presets[i]->getDisplayName() == preset_name) {
            // Remove from QSettings
            QSettings settings;
            settings.beginGroup("presets");
            if (settings.childGroups().contains(preset_name)) {
                settings.remove(preset_name);
            }
            settings.endGroup();

            // Remove from the model
            beginRemoveRows(QModelIndex(), i, i);
            delete game_presets.takeAt(i);  // Free memory and remove from the vector
            endRemoveRows();

            qDebug() << "Preset deleted successfully:" << preset_name;
            return;
        }
    }

    qWarning() << "Preset with name" << preset_name << "not found in the model.";
}

void GamePresetModel::setGamePath(int presetIndex, int gameIndex) {
    if(presetIndex < 0 || presetIndex > game_presets.size()){
        return;
    }

    game_presets[presetIndex]->setGameIndex(gameIndex);

    emit dataChanged(this->index(presetIndex), this->index(presetIndex));
}

void GamePresetModel::setJsonPath(const QString &name, const QUrl &path) {
    int index = -1;
    for (int i = 0; i < game_presets.size(); ++i) {
        if (game_presets[i]->getDisplayName() == name) {
            index = i;
            break;
        }
    }

    // If preset not found, log a warning and return
    if (index == -1) {
        qWarning() << "Preset with name" << name << "not found in the model.";
        return;
    }

    // Convert QUrl to QString and update the JSON path
    QString jsonPath = path.toLocalFile();
    updateJsonPath(index, jsonPath);
}
