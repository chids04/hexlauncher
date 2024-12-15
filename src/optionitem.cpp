#include "optionitem.h"

QDataStream &operator<<(QDataStream &out, const OptionItem &optionItem){
    out << optionItem.presetName << optionItem.choiceName << optionItem.sectionName
    << optionItem.choiceOptions << optionItem.currentOption << optionItem.optionIndex;

    return out;
}

QDataStream &operator>>(QDataStream &in, OptionItem &optionItem){
    in >> optionItem.presetName >> optionItem.choiceName >> optionItem.sectionName
        >> optionItem.choiceOptions >> optionItem.currentOption >> optionItem.optionIndex;

    return in;
}
