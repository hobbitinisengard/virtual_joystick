#ifndef SCANBOX_H
#define SCANBOX_H
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QMessageBox>
#include <bindingstruct.h>
#include <vjdevice.h>
#include <binding.h>
class ScanBox : public QMessageBox
{
public:
    ScanBox(BindingStruct *CONTAINER, vJDevice *DEVICE, QList<Binding*> *BINDINGS)
        : binding_struct (CONTAINER), device(DEVICE), bindings(BINDINGS)
    {
        this->setWindowTitle("Scanning...");
        this->setText("Press desired button or move axis ...");
        this->setButtonText(QMessageBox::Ok, tr("Cancel"));
    };
    BindingStruct *binding_struct;
    void keyPressEvent(QKeyEvent *event);
    vJDevice* device;
    QList<Binding*> *bindings;
};

#endif // SCANBOX_H
