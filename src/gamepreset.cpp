#include "gamepreset.h"

#include <QSettings>
GamePreset::GamePreset(QString &display_name, QString &sd_root, OptionModel *option_model, QString &section_name, QString &xml_path, int index, QString json_path, QObject *parent)
    : QObject(parent), display_name(display_name), sd_root(sd_root), option_model(option_model), section_name(section_name), xml_path(xml_path), json_path(json_path), gameIndex(index){}

QString GamePreset::getRoot() const
{
    return sd_root;
}

QString GamePreset::getDisplayName() const
{
    return display_name;
}

QString GamePreset::getSectionName() const
{
    return section_name;
}

QString GamePreset::getXmlPath() const
{
    return xml_path;
}

QString GamePreset::getJsonPath() const
{
    return json_path;
}


int GamePreset::getGameIndex() const {
    return gameIndex;
}

void GamePreset::setJsonPath(QString &newJsonPath)
{
    json_path = newJsonPath;
}

void GamePreset::savePreset()
{
    QSettings settings;

    settings.beginGroup("presets");
    settings.beginGroup(display_name);

    settings.setValue("displayName", display_name);
    settings.setValue("sectionName", section_name);
    settings.setValue("sdRoot", sd_root);
    settings.setValue("xmlPath", xml_path);
    settings.setValue("jsonPath", json_path);
    settings.setValue("gameIndex", gameIndex);

    //getting optionmodel
    settings.beginWriteArray("options");
    const auto option_list = option_model->getOptionList();
    for(int i=0; i<option_list.size(); ++i){
        settings.setArrayIndex(i);
        settings.setValue("optionItem", QVariant::fromValue(*option_list[i]));
    }
    settings.endArray();
    settings.endGroup();
    settings.endGroup();

}

void GamePreset::setGameIndex(int index) {
    gameIndex = index;
}

OptionModel *GamePreset::getOptionModel() const
{
    return option_model;
}
