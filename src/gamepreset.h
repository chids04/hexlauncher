#ifndef GAMEPRESET_H
#define GAMEPRESET_H

#include "optionmodel.h"

#include <QString>
#include <QObject>
#include <QMetaType>
#include <QtQml/qqmlregistration.h>

class GamePreset : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

public:
    explicit GamePreset(QString &display_name, QString &sd_root, OptionModel *optionModel, QString &section_name, QString &xml_path, int index = -1, QString json_path = "", QObject *parent = nullptr);
    QString getRoot() const;
    QString getDisplayName() const;
    QString getSectionName() const;
    QString getXmlPath() const;
    QString getJsonPath() const;
    int getGameIndex() const;
    OptionModel* getOptionModel() const;
    void setJsonPath(QString &newJsonPath);
    void savePreset();
    void setGameIndex(int index);
private:
    QString sd_root;
    QString display_name;
    OptionModel *option_model;
    QString section_name;
    QString xml_path;
    QString json_path;
    int gameIndex = -1;



};

Q_DECLARE_METATYPE(GamePreset)
#endif // GAMEPRESET_H
