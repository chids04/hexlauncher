#include "presetparser.h"
#include "gamepreset.h"
#include "optionmodel.h"
#include "globalmodels.h"
#include "optionitem.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QtDebug>
#include <QXmlStreamReader>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QApplication>
#include <QProcess>
#include <QSettings>

PresetParser::PresetParser(QObject *parent) : QObject(parent) {
    QSettings settings;

    setMkwiiPath(settings.value("global/mkwiiPath", "").toString());
    setDolphPath(settings.value("global/dolphPath", "").toString());
}

void PresetParser::writeToJson(QString game_path, QString save_location, QString preset_name)
{
    QVector<GamePreset*> game_presets = GlobalModels::s_singletonInstance->gamePresetModel()->getGamePresets();
    int presetIndex = -1;

    for (int i = 0; i < game_presets.size(); ++i) {
        if (game_presets[i]->getDisplayName() == preset_name) {
            presetIndex = i;
            break;
        }
    }

    if (presetIndex == -1) {
        qWarning() << "Preset not found:" << preset_name;
        return;
    }

    GamePreset* game_preset = game_presets[presetIndex];
    QJsonObject root_object;
    QString escaped_base_path = game_path;

    static const QRegularExpression separatorRegex(R"([\\/])");

    QStringList path_components = escaped_base_path.split(separatorRegex);

    qDebug() << path_components;

    escaped_base_path = path_components.join(R"(\/)");

    qDebug() << escaped_base_path;
    //escaped_base_path.replace("/", "\\/");

    root_object["base-file"] = escaped_base_path;
    root_object["display-name"] = preset_name;

    int num_choices = game_preset->getOptionModel()->rowCount();
    QVector<std::shared_ptr<OptionItem>> options = game_preset->getOptionModel()->getOptionList();
    QJsonObject riivo_object;
    QJsonObject patches_object;
    QJsonArray patches_array;
    QJsonArray options_array;

    for(int i=0; i<num_choices; ++i){
        //need to get the selected value
        QString option_name = options[i]->choiceName;
        QString section_name = options[i]->sectionName;

        int choice = options[i]->currentOption;
        qDebug() << choice;

        options_array.append(QJsonObject{
            {"choice", choice},
            {"option-name", option_name},
            {"section-name", section_name}
        });
    }

    QString escaped_sd_root = game_preset->getRoot();
    QStringList path_components_sd = escaped_sd_root.split(separatorRegex);
    escaped_sd_root = path_components_sd.join(R"(\/)");

    QString escaped_xml = game_preset->getXmlPath();
    QStringList path_components_xml = escaped_xml.split(separatorRegex);
    escaped_xml = path_components_xml.join(R"(\/)");

    patches_object["options"] = options_array;
    patches_object["root"] = escaped_sd_root;
    patches_object["xml"] = escaped_xml;

    patches_array.append(patches_object);

    riivo_object["patches"] = patches_array;
    root_object["riivolution"] = riivo_object;

    root_object["type"] = "dolphin-game-mod-descriptor";
    root_object["version"] = 1;

    QJsonDocument json_doc(root_object);

    QString preset_path = QDir(save_location).absoluteFilePath(preset_name + ".json");
    QFile file(preset_path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open json file for writing:" << file.errorString();
        return;
    }
    file.write(json_doc.toJson(QJsonDocument::Indented));
    file.close();

    GlobalModels::s_singletonInstance->gamePresetModel()->updateJsonPath(presetIndex, preset_path);
    game_preset->savePreset();

}

void PresetParser::parseRiivoXML(QUrl xml_path, QString display_name)
{
    //no need to check input, will do santiation in qml
    QString	xml_path_str = xml_path.toLocalFile();

    QFileInfo file_info(xml_path_str);
    //need to check to make sure file in riivolution folder
    QDir riivo_dir = file_info.dir();

    if(riivo_dir.dirName() != "riivolution"){
        qWarning() << "xml must be in riivolution dir";
        return;
    }

    riivo_dir.cdUp();
    QString sd_root = riivo_dir.absolutePath();

    if(sd_root == QString()){
        qWarning() << "folder structure: <mod name>/riivolution/<mod name.xml>";
        return;
    }

    QFile file(xml_path_str);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qWarning() << "Failed to open file: " << file.error();
        return;
        //will add exception here or throw signal to display in gui
    }

    QXmlStreamReader xml(&file);
    QVector<std::shared_ptr<OptionItem>> option_list;
    QString current_option;
    QString section_name;
    int idx = 0;
    bool inside_options = false;

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartElement){

            if(xml.name() == "options"){
                inside_options = true;
            }
            else if(xml.name() == "section"){
                section_name = xml.attributes().value("name").toString();

            }
            else if(inside_options && xml.name() == "option"){
                //inside <option> tag
                current_option = xml.attributes().value("name").toString();

                auto option_item = std::make_shared<OptionItem>();
                option_item->presetName = display_name;
                option_item->choiceName = current_option;
                option_item->choiceOptions.append("Disabled");
                option_item->optionIndex = idx;
                option_item->sectionName = section_name;
                option_list.push_back(option_item);

                idx++;
            }
            else if(inside_options && xml.name() == "choice" && !current_option.isEmpty()){
                //inside <choice> tag
                QString choice_name = xml.attributes().value("name").toString();

                for(auto &option: option_list){
                    if(option->choiceName == current_option){
                        option->choiceOptions.append(choice_name);
                        break;
                    }
                }
            }
        }

        else if(token == QXmlStreamReader::EndElement){
            if(xml.name() == "options") {
                break;
            }
        }

    }

    //create gamepreset object here
    OptionModel *option_model = new OptionModel;
    option_model->setOptions(option_list);
    GamePreset *gamePreset = new GamePreset(display_name, sd_root, option_model, section_name, xml_path_str);
    GlobalModels::s_singletonInstance->gamePresetModel()->addGamePreset(gamePreset);
}

void PresetParser::addOption(QString option, int index)
{
    selected_options[option] = index;
}

void PresetParser::collectOptions()
{
    emit getOptions();
}

void PresetParser::printHello()
{
    qDebug() << "hello";
}

void PresetParser::savePreset(QString display_name, QUrl save_path)
{
    if(m_mkwiiPath == ""){
        emit sendError("please select mkwii game path");
        return;
    }

    if(!save_path.isEmpty()){
        writeToJson(m_mkwiiPath, save_path.toLocalFile(), display_name);
    }



}

void PresetParser::setGamePath(QUrl mkwii_path)
{
    setMkwiiPath(mkwii_path.toLocalFile());
}

void PresetParser::setExecutablePath(QUrl dolphin_executable)
{
    setDolphPath(dolphin_executable.toLocalFile());
}

void PresetParser::runGame(QString json_path)
{
    qDebug() << json_path;

    if(m_dolphPath == ""){
        emit sendError("select dolphin executable path");
        return;
    }

    if(json_path == ""){
        emit sendError("save preset before running");
        return;
    }

    QStringList arguments;
    arguments << "-e" << json_path;

    bool success = QProcess::startDetached(m_dolphPath, arguments);

    if(!success){
        emit sendError("failed to start game, check dolphin executable path");
        return;
    }
}


QString PresetParser::mkwiiPath() const
{
    return m_mkwiiPath;
}


void PresetParser::setMkwiiPath(const QString &newMkwiiPath)
{
    if (m_mkwiiPath == newMkwiiPath)
        return;
    m_mkwiiPath = newMkwiiPath;
    emit mkwiiPathChanged();

    QSettings settings;
    settings.beginGroup("global");
    settings.setValue("mkwiiPath", newMkwiiPath);
    settings.endGroup();
}

QString PresetParser::dolphPath() const
{
    return m_dolphPath;
}

void PresetParser::setDolphPath(const QString &newDolphPath)
{
    if (m_dolphPath == newDolphPath)
        return;
    m_dolphPath = newDolphPath;
    emit dolphPathChanged();

    QSettings settings;
    settings.beginGroup("global");
    settings.setValue("dolphPath", newDolphPath);
    settings.endGroup();
}
