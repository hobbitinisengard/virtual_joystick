#ifndef SCANBOX_H
#define SCANBOX_H
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QMessageBox>
#include <bindingstruct.h>
class ScanBox : public QMessageBox
{
public:
    ScanBox(BindingStruct *CONTAINER)
        : binding_struct (CONTAINER)
    {
        this->setWindowTitle("Scanning...");
        this->setText("Press desired button or move axis ...");
        this->setButtonText(QMessageBox::Ok, tr("Cancel"));
    };
    void keyPressEvent(QKeyEvent *event);
    BindingStruct *binding_struct;
};

#endif // SCANBOX_H
