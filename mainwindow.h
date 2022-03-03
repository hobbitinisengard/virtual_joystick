#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QListView>
#include <QVector>

#include "vjoyinterface.h"
#include <malloc.h>
#include <stdlib.h>
#include <cstdio>

#include <scanbox.h>
#include <bindingstruct.h>
#include <vjdevice.h>
#include <binding.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Button_ScanBinding_clicked();

    void on_Button_ScanButtonOrAxis_clicked();

    void on_vjoy_tabs_currentChanged(int index);

    void on_ListWidget_ButtonBindings_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_AxesBinding_itemDoubleClicked(QListWidgetItem *item);

private:
    inline const static QString VJOY_VERSION {"2.18"};
    Ui::MainWindow *ui;
    BindingStruct *button_bindings = nullptr;
    BindingStruct *axes_bindings = nullptr;
    vJDevice *current_device;
    vJDevice *devices[16];
    QList<Binding*> bindings;
//    const QString axes[18] {
//        "Throttle",
//        "Rudder",
//        "Aileron",
//        "AxisX",
//        "AxisY",
//        "AxisZ",
//        "AxisXRot",
//        "AxisYRot",
//        "AxisZRot",
//        "Slider",
//        "Dial",
//        "Wheel",
//        "AxisVX",
//        "AxisVY",
//        "AxisVZ",
//        "AxisVBRX",
//        "AxisVBRY",
//        "AxisVBRZ"
//    };
    void Populate_joystick_buttons_menu(QStringListModel *model);
    void Scan(BindingStruct *binding_struct);
    void Initialize_comboboxes(vJDevice *device);
    bool Vjoy_device_exist(byte vjoy_no);
    void Assert_vjoy_exists(byte vjoy_no);
    void Create_OK_MessageBox(QString text);
    void Quit();
    bool eventFilter(QObject *obj, QEvent *event);

    void Load_configuration();
    void Save_configuration();
};
#endif // MAINWINDOW_H
