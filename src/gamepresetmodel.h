#ifndef GAMEPRESETMODEL_H
#define GAMEPRESETMODEL_H

#include "gamepreset.h"
#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

class GamePresetModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ANONYMOUS

public:
    GamePresetModel(QObject *parent=nullptr);

    enum GamePresetRoles {
        DisplayNameRole = Qt::UserRole + 1,
        SDRole,
        OptionModelRole,
        SectionNameRole,
        JsonPathRole
    };

    void addGamePreset(GamePreset *game_preset);
    QModelIndex index(int row, int column=0, const QModelIndex &parent=QModelIndex()) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVector<GamePreset*> getGamePresets() const;
    void updateJsonPath(int index, QString &json_path);
    

public slots:
    void setOptionChoice(int choice_idx, QString preset_name, int option_idx);
    bool isDuplicate(QString name);
    void deletePreset(QString preset_name);

private:
    QVector<GamePreset*> game_presets;

};

#endif // GAMEPRESETMODEL_H
