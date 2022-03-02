#ifndef BINDINGSTRUCT_H
#define BINDINGSTRUCT_H
#include <QListWidget>
#include <QComboBox>

class BindingStruct
{
public:
    BindingStruct(QComboBox *COMBO, QListWidget *DATA)
        :comboBox(COMBO), data(DATA)
    {
    };
    QComboBox *comboBox;
    QListWidget *data;
};

#endif // BINDINGSTRUCT_H
