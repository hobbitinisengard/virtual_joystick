#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    installEventFilter(this);

    button_bindings = new BindingStruct(ui->AllButtons_ComboBox,
                                        ui->ListWidget_ButtonBindings, SignalType::BUTTON);
    axes_bindings = new BindingStruct(ui->AllButtonsAndAxesComboBox,
                                      ui->listWidget_AxesBinding, SignalType::AXIS);

    Initialize_VJoy(0);
    try
    {
        current_device = new vJDevice();
    }
    catch(VjdStat Error)
    {
        switch (Error)
        {
        case VJD_STAT_BUSY:
            Create_OK_MessageBox("This device is already owned by another feeder");
            QCoreApplication::quit(); //return -3
        case VJD_STAT_MISS:
            Create_OK_MessageBox("This vJoy device is not installed or disabled. Feeder stopped.");
            QCoreApplication::quit(); //return -4;
        default:
            Create_OK_MessageBox("vJoy device general error. Cannot continue.");
            QCoreApplication::quit(); //return -1;
        };
    }

    devices[0] = current_device;
    Initialize_comboboxes(current_device);
}
void MainWindow::Quit()
{
    for(const auto device : devices)
        RelinquishVJD(device->id);
    QApplication::quit();
}
void MainWindow::Initialize_comboboxes(vJDevice *device)
{
    {
    // populate button combobox
    QStringListModel *button_model = new QStringListModel(this);
    QStringList button_list;
    int buttons_number = GetVJDButtonNumber(device->id);
    for(int i=0; i<buttons_number; ++i){
        button_list << "Button " + QString::number(i+1);
    }
    button_model->setStringList(button_list);
    button_bindings->comboBox->setModel(button_model);
    }

    // populate data from .ini or memory
    // code

    // populate axis combobox
    QStringListModel *axes_model = new QStringListModel(this);
    QStringList buttonaxes_list;
    for(size_t i=0; i<axisdata.size(); ++i){
        if(!GetVJDAxisExist(device->id, axisdata[i]->macro))
        {
            qCritical("MAX axis i ");
            break;
        }
        buttonaxes_list << axisdata[i]->name;
    }

    axes_model->setStringList(buttonaxes_list);
    axes_bindings->comboBox->setModel(axes_model);

    // populate data from .ini or memory
    // code
}
void MainWindow::Load_configuration()
{

}
void MainWindow::Save_configuration()
{

}
void MainWindow::KeyPressAction(const WPARAM wParam, const int keyCode, const QString &keyname)
{
    //qDebug() << wParam << " " << keyCode << " " << keyname;
    for(auto& binding : bindings)
    {
        //event->key() != Qt::Key_Escape
        if(binding->key == keyCode)
        {
            devices[binding->device_id-1]->Send_data(binding, WM_KEYDOWN == wParam);
        }
    }
    //qInfo("Ate key press %d", keyEvent->key());
}
void MainWindow::Create_OK_MessageBox(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText(text);
    msgBox.exec();
}
void MainWindow::Initialize_VJoy(byte vjoy_no)
{
    WORD VerDll, VerDrv;
    if (!DriverMatch(&VerDll, &VerDrv)){
        qCritical("Failed\r\nvJoy Driver (version %04x) does not match\
            vJoyInterface DLL (version %04x)\n", VerDrv ,VerDll);
        throw "DLL and driver versions do not match";
    }

    while(1)
    {
        if(!vJoyEnabled() || GetVJDStatus(1) == VJD_STAT_MISS || GetVJDStatus(1) == VJD_STAT_UNKN)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("");
            msgBox.setText("Device vjoy" + QString::number(vjoy_no) + " is not active. Enable it in 'Configure VJoy' program.");
            msgBox.addButton("Retry", QMessageBox::ActionRole);
            msgBox.addButton("Close program", QMessageBox::RejectRole);
            if(msgBox.exec() == QMessageBox::RejectRole){
                // quit program
                QCoreApplication::quit();
            }
        }
        else{
            break;
        }
    }
    ResetAll();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_ScanBinding_clicked()
{
    Scan(button_bindings);
}
void MainWindow::on_Button_ScanButtonOrAxis_clicked()
{
    Scan(axes_bindings);
}

void MainWindow::Scan(BindingStruct *binding_struct)
{
    ScanBox *Scanning_monolog_box = new ScanBox(binding_struct, current_device, bindings, axisdata);
    Scanning_monolog_box->exec();
    delete Scanning_monolog_box;
}

void MainWindow::on_vjoy_tabs_currentChanged(int index)
{
    if(index == 0)
        return;
}


void MainWindow::on_ListWidget_ButtonBindings_itemDoubleClicked(QListWidgetItem *item)
{

}


void MainWindow::on_listWidget_AxesBinding_itemDoubleClicked(QListWidgetItem *item)
{

}

