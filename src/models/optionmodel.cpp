#include "optionmodel.h"

OptionModel::OptionModel(QObject *parent) : QAbstractListModel(parent) {}

QModelIndex OptionModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    // Check for a valid row and column
    if (row >= 0 && row < rowCount() && column == 0) {
        return createIndex(row, column);
    } else {
        return QModelIndex(); // Return an invalid index if out of bounds
    }
}

int OptionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return option_list.count();
}

QVariant OptionModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= option_list.count()){
        qWarning() << "Invalid index for Option Model";
        return QVariant();
    }

    auto option = option_list.at(index.row());

    switch(role){
        case NameRole:
            return option->choiceName;

        case ChoicesRole:
            return option->choiceOptions;

        case GamePresetNameRole:
            return option->presetName;

        case ChoiceIndexRole:
            return option->optionIndex;

        default:
            qWarning() << "Invalid role passed to data method of Option Model";
            return QVariant();
    }
}

QHash<int, QByteArray> OptionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "choiceName";
    roles[ChoicesRole] = "choicesList";
    roles[ChoiceIndexRole] = "choiceIndex";
    roles[GamePresetNameRole] = "presetName";

    return roles;
}

void OptionModel::setOptions(QVector<std::shared_ptr<OptionItem>> newOptionList)
{
    beginResetModel();
    option_list = newOptionList;
    endResetModel();
}

QVector<std::shared_ptr<OptionItem>> OptionModel::getOptionList() const
{
    return option_list;
}
