#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QMessageBox>
#include <QListView>
#include <QVector>
#include <QToolBar>
#include <QFileDialog>
#include "vjoyinterface.h"
#include <malloc.h>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <functional>

#include <scanbox.h>
#include <vjdevice.h>
#include <Structs.h>
#include <windows.h>
#include <VJoyConnector.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    inline static const QString VERSION = "0.1";
    inline static const QString latest_link_to_driver = "https://github.com/njz3/vJoy/releases/tag/v2.2.1.1";
    bool eventFilter(QObject *object, QEvent *event) override;
    QList<Binding*> bindings;
    VJoyConnector connector {};
    std::unique_ptr<Button_bindingStruct> bs_buttons;
    std::unique_ptr<Axis_bindingStruct> bs_axes;
    std::unique_ptr<POV_bindingStruct> bs_povs;
    inline static bool allow_multiple_keybindings = false;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    BindingStruct *get_bindingstruct_of_type(const SignalType type) const;
    int Convert_bindingstruct_to_type(BindingStruct *bs) const;
private slots:
    void on_Button_ScanBinding_clicked();

    void on_Button_ScanButtonOrAxis_clicked();

    void on_vjoy_tabs_currentChanged(int index);

    void on_ListWidget_ButtonBindings_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_AxesBinding_itemDoubleClicked(QListWidgetItem *item);

    void on_Checkbox_AllowMultipleKeybinding_stateChanged(int arg1);

    void on_ListWidget_ButtonBindings_itemClicked(QListWidgetItem *item);

    void on_listWidget_AxesBinding_itemClicked(QListWidgetItem *item);

    void on_Button_ScanPOVs_clicked();

    void on_ListWidget_POVBindings_itemDoubleClicked(QListWidgetItem *item);

    void on_ListWidget_POVBindings_itemClicked(QListWidgetItem *item);

    void on_button_CLEAR_clicked();

    void on_button_LOAD_clicked();

    void on_button_SAVE_clicked();

    void on_actionAbout_triggered();

private:
    inline const static QString VJOY_VERSION {"2.18"};
    static constexpr uint8_t VJ_DEVICES = 16;
    Ui::MainWindow *ui;
//    BindingStruct *button_bindings = nullptr;
//    BindingStruct *axis_bindings = nullptr;
//    BindingStruct *pov_bindings = nullptr;
    std::vector<BindingStruct*> binding_structs;
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
    void Scan(BindingStruct &binding_struct);
    bool Vjoy_device_exist(byte vjoy_no);
    void Initialize_VJoy(byte vjoy_no);
    void Quit();
    void Load_configuration();
    void Save_configuration();
    void UpdateKeyState(BYTE *keystate, int keycode);
    void Delete_from_bindings(QListWidgetItem *item);
    void Clear_Listwidgets();
    void Create_Retry_MessageBox(const QString &text);
    void Switch_device(int index);
    std::vector<std::string> split(std::string s, std::string delimiter);
    template<class T>
    T base_name(const T &path, const T &delims = "/\\");
    template<class T>
    T remove_extension(const T &filename);
    void UpdateName(std::string filepath);
};
#endif // MAINWINDOW_H
