#ifndef OPTIONMODEL_H
#define OPTIONMODEL_H

#include "optionitem.h"
#include <QAbstractListModel>


class OptionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    OptionModel(QObject *parent=nullptr);

    enum OptionRoles {
        NameRole = Qt::UserRole + 1,
        ChoicesRole,
        ChoiceIndexRole,
        GamePresetNameRole
    };

    QModelIndex index(int row, int column=0, const QModelIndex &parent=QModelIndex()) const override;
    int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    void setOptions(QVector<std::shared_ptr<OptionItem>> option_list);
    QVector<std::shared_ptr<OptionItem>> getOptionList() const;

private:
    QVector<std::shared_ptr<OptionItem>> option_list;
};

#endif // OPTIONMODEL_H
