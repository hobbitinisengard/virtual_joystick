#ifndef SCANBOX_H
#define SCANBOX_H
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QListWidget>
#include <QMessageBox>
#include <vjdevice.h>
#include <Structs.h>
class ScanBox : public QMessageBox
{
public:
    ScanBox(BindingStruct *CONTAINER, vJDevice *DEVICE, QList<Binding*> &BINDINGS, const std::vector<AxisData*> &AXISDATA)
        : binding_struct (CONTAINER), device(DEVICE), bindings(BINDINGS), axisdata(AXISDATA)
    {
        this->setWindowTitle("Scanning...");
        this->setText("Press desired button or move axis ...");
        this->setButtonText(QMessageBox::Ok, tr("Cancel"));
    };
    BindingStruct *binding_struct;
    void keyPressEvent(QKeyEvent *event);
    vJDevice* device;
    QList<Binding*> bindings;
    const std::vector<AxisData*> axisdata;
};

#endif // SCANBOX_H
