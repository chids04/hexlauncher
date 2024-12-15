#ifndef PRESETPARSER_H
#define PRESETPARSER_H

#include <QtQml>
#include <QUrl>
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

#include "optionitem.h"

class PresetParser : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(QString mkwiiPath READ mkwiiPath WRITE setMkwiiPath NOTIFY mkwiiPathChanged)
    Q_PROPERTY(QString dolphPath READ dolphPath WRITE setDolphPath NOTIFY dolphPathChanged)

public:
    explicit PresetParser(QObject *parent = nullptr);
    QString mkwiiPath() const;
    void setMkwiiPath(const QString &newMkwiiPath);

    QString dolphPath() const;
    void setDolphPath(const QString &newDolphPath);

signals:
    void getOptions();
    void sendError(QString error_msg);



    void mkwiiPathChanged();

    void dolphPathChanged();

public slots:
    void writeToJson(QString game_path, QString save_location, QString preset_name);
    void parseRiivoXML(QUrl riivo_base_path, QString display_name);
    void addOption(QString option, int index);
    void collectOptions();
    void printHello();
    void savePreset(QString display_name, QUrl save_path);
    void setGamePath(QUrl mkwii_path);
    void setExecutablePath(QUrl dolphin_executable);
    void runGame(QString json_path);

private:
    QHash<QString, int> selected_options;
    QHash<QString, OptionItem> new_options;

    QString m_mkwiiPath;
    QString m_dolphPath;
};

#endif // PRESETPARSER_H
