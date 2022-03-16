#ifndef SCANBOX_H
#define SCANBOX_H
#include <QMainWindow>
#include <QAbstractButton>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QMessageBox>
#include <vjdevice.h>
#include <Structs.h>
class ScanBox : public QMessageBox
{
public:
    ScanBox(BindingStruct &CONTAINER, vJDevice *DEVICE, QList<Binding*> &BINDINGS)
        : QMessageBox(), binding_struct (CONTAINER), device(DEVICE), bindings(BINDINGS)
    {
        this->setWindowTitle("Scanning...");
        this->setText("Press desired button or move axis ...");
        this->setStandardButtons(QMessageBox::NoButton);
        //this->setButtonText(QMessageBox::Ok, tr("Cancel"));

    };
    BindingStruct &binding_struct;
    void keyPressEvent(QKeyEvent *event);
    vJDevice* device;
    QList<Binding*>& bindings; // <- when passing an object by reference, member has to be a reference as well
};

#endif // SCANBOX_H
