#include "infobox.h"

Infobox::Infobox(const QString &text)
    : QMessageBox()
{
    QMessageBox msgBox;
    this->setTextFormat(Qt::RichText);
    this->setWindowTitle("vJoy keyboard feeder");
    this->setText(text);
    this->exec();
}
