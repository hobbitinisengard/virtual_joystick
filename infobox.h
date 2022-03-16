#ifndef INFOBOX_H
#define INFOBOX_H
#include <QMessageBox>

class InfoBox : QMessageBox
{
public:
    InfoBox(const QString &text);
};

#endif // INFOBOX_H
