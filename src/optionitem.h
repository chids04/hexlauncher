#ifndef OPTIONITEM_H
#define OPTIONITEM_H

#include <QString>
#include <QStringList>
#include <QMetaType>

struct OptionItem{
    QString presetName;
    QString choiceName;
    QString sectionName;
    QStringList choiceOptions;
    int currentOption;
    int optionIndex;

    void setCurrentOption(int index){
        currentOption = index;
    }

    friend QDataStream &operator<<(QDataStream &out, const OptionItem &optionItem);
    friend QDataStream &operator>>(QDataStream &in, OptionItem &optionItem);
};

Q_DECLARE_METATYPE(OptionItem)
#endif // OPTIONITEM_H
