#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    button_bindings = new BindingStruct(ui->AllButtons_ComboBox, ui->ListWidget_ButtonBindings);
    axes_bindings = new BindingStruct(ui->AllButtonsAndAxesComboBox, ui->listWidget_AxesBinding);

    Assert_vjoy_exists(1);

    // Get the driver attributes (Vendor ID, Product ID, Version Number)
    if (!vJoyEnabled())
    {
        Create_OK_MessageBox("Function vJoyEnabled failed; Make sure that vJoy " + VJOY_VERSION + "is installed and enabled");
        QCoreApplication::quit();
    }
    else
    {
       // wprintf(L"Vendor: %s\nProduct :%s\nVersion Number:%s\n", static_cast<TCHAR *> (GetvJoyManufacturerString()), static_cast<TCHAR *>(GetvJoyProductString()), static_cast<TCHAR *>(GetvJoySerialNumberString()));
    };

    // Get the status of the vJoy device before trying to acquire it
    vJDevice *device = nullptr;
    try
    {
        device = new vJDevice();
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

    devices.push_back(device);
    Fill_bindings();
}
void MainWindow::Quit()
{
    for(const auto device : devices)
        RelinquishVJD(device->device_id);
    QApplication::quit();
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease) {
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         //qDebug("Ate key press %d", keyEvent->key());

        // for po wektorze przypisanych klawiszy




         // The structure that holds the full position data
         //JOYSTICK_POSITION_V2 iReport;
//         USHORT X = 0;
//         USHORT Y = 0;
//         USHORT Z = 0;
//         LONG   Btns = 0;
//         PVOID pPositionMessage;
//         UINT	IoCode = LOAD_POSITIONS;
//         UINT	IoSize = sizeof(JOYSTICK_POSITION);
//         while (1)
//         {
//             // Set target vJoy device
//             byte id = (byte)device_id;
//             iReport.bDevice = id;

//             // Set position data of 3 first axes
//             if (Z>35000) Z=0;
//             Z += 200;
//             iReport.wAxisZ = Z;
//             iReport.wAxisX = 32000-Z;
//             iReport.wAxisY = Z/2+7000;

//             // Set position data of first 8 buttons
//             Btns = 1<<(Z/4000);
//             iReport.lButtons = Btns;
//         }
         // end
         return true;
     } else {
         // standard event processing
         return QObject::eventFilter(obj, event);
     }
 }
void MainWindow::Create_OK_MessageBox(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Info");
    msgBox.setText(text);
    msgBox.exec();
}
void MainWindow::Assert_vjoy_exists(byte vjoy_no)
{
    while(1)
    {
        if(!vJoyEnabled() || GetVJDStatus(1) == VJD_STAT_MISS || GetVJDStatus(1) == VJD_STAT_UNKN){
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
}
void MainWindow::Fill_bindings()
{
    {
    // populate button bindings
    QStringListModel *button_model = new QStringListModel(this);
    QStringList button_list;
    for(int i=0; i<32; ++i)
        button_list << "Button " + QString::number(i+1);
    button_model->setStringList(button_list);
    button_bindings->comboBox->setModel(button_model);
    //button_bindings.data->
    }

    // populate data from .ini or memory
    // code

    // populate axes bindings
    QStringListModel *axes_model = new QStringListModel(this);
    QStringList buttonaxes_list;
    for(int i=0; i<32; ++i)
        buttonaxes_list << "Button " + QString::number(i+1);
    axes_model->setStringList(buttonaxes_list);
    axes_bindings->comboBox->setModel(axes_model);
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
    ScanBox *Scanning_monolog_box = new ScanBox(binding_struct);
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

