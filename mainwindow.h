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
#include "windows.h"

#include <scanbox.h>
#include <vjdevice.h>
#include <Structs.h>
#include <windows.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void KeyPressAction(const WPARAM wParam, const int vkCode, const QString &keyname);
    QList<Binding*> bindings;
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
    // AXIS | MACRO | VALUE
    //X HID_USAGE_X 0x30 (-x, 0, +x)
    //Y HID_USAGE_Y 0x31
    //Z HID_USAGE_Z 0x32
    //Rx HID_USAGE_RX 0x33 (0, +x)
    //Ry HID_USAGE_RY 0x34
    //Rz HID_USAGE_RZ 0x35
    //Slider0 HID_USAGE_SL0 0x36
    //Slider1 HID_USAGE_SL1 0x37
    //Wheel HID_USAGE_WHL 0x38
    const std::vector<AxisData*> axisdata {
        new AxisData(HID_USAGE_X, "+X"),
        new AxisData(HID_USAGE_X, "-X", Direction::NEGATIVE),
        new AxisData(HID_USAGE_Y, "+Y"),
        new AxisData(HID_USAGE_Y, "-Y", Direction::NEGATIVE),
        new AxisData(HID_USAGE_Z, "+Z"),
        new AxisData(HID_USAGE_Z, "-Z", Direction::NEGATIVE),
        new AxisData(HID_USAGE_RX, "Rx"),
        new AxisData(HID_USAGE_RY, "Ry"),
        new AxisData(HID_USAGE_RZ, "Rz"),
        new AxisData(HID_USAGE_SL0, "Slider 0"),
        new AxisData(HID_USAGE_SL1, "Slider 1")
    };
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
    void Initialize_VJoy(byte vjoy_no);
    void Create_OK_MessageBox(QString text);
    void Quit();
    void Load_configuration();
    void Save_configuration();
    void UpdateKeyState(BYTE *keystate, int keycode);
};
#endif // MAINWINDOW_H
