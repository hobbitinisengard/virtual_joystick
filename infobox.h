#ifndef INFOBOX_H
#define INFOBOX_H
#include <QMessageBox>

class Infobox : QMessageBox
{
public:
    Infobox(const QString &text);
};

#endif // INFOBOX_H
