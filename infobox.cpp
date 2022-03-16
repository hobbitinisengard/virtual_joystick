#include "infobox.h"

InfoBox::InfoBox(const QString &text)
    : QMessageBox()
{
    QMessageBox msgBox;
    this->setTextFormat(Qt::RichText);
    this->setWindowTitle("vJoy keyboard feeder");
    this->setText(text);
    this->exec();
}
