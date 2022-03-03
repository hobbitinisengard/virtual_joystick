#ifndef BINDINGSTRUCT_H
#define BINDINGSTRUCT_H
#include <QListWidget>
#include <QComboBox>
#include <vjdevice.h>
class BindingStruct
{
public:
    BindingStruct(QComboBox *COMBO, QListWidget *DATA, SignalType signaltype)
        :comboBox(COMBO), data(DATA), type(signaltype)
    {};
    QComboBox *comboBox;
    QListWidget *data;
    SignalType type;
};

#endif // BINDINGSTRUCT_H
