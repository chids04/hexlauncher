#ifndef PRESETPARSER_H
#define PRESETPARSER_H

#include <QtQml>
#include <QUrl>
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml/qqmlregistration.h>

#include "gamesmodel.h"
#include "optionitem.h"

class PresetParser : public QObject
{
    Q_OBJECT
    QML_ANONYMOUS

    Q_PROPERTY(QString mkwiiPath READ mkwiiPath WRITE setMkwiiPath NOTIFY mkwiiPathChanged)
    Q_PROPERTY(QString dolphPath READ dolphPath WRITE setDolphPath NOTIFY dolphPathChanged)

public:
    explicit PresetParser(GamesModel *gamesModel, QObject *parent = nullptr);
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
    int savePreset(const QString &display_name, const QUrl &save_path, int gameIndex);
    int updatePreset(const QString &display_name, const QString &file_path, int gameIndex);
    void setGamePath(QUrl mkwii_path);
    void setExecutablePath(QUrl dolphin_executable);
    void runGame(const QString &json_path, const QString &display_name, int gameIndex);

private:
    QHash<QString, int> selected_options;
    QHash<QString, OptionItem> new_options;

    QString m_mkwiiPath;
    QString m_dolphPath;

    GamesModel *gamesModel;
};

#endif // PRESETPARSER_H
