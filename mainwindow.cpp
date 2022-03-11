#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    installEventFilter(this);
    //on_actionAbout_triggered();
    button_bindings = new BindingStruct(ui->AllButtons_ComboBox,
                                        ui->ListWidget_ButtonBindings, SignalType::BUTTON);
    axis_bindings = new BindingStruct(ui->AllButtonsAndAxesComboBox,
                                      ui->listWidget_AxesBinding, SignalType::AXIS);
    pov_bindings = new BindingStruct(ui->AllPovs_ComboBox,
                                     ui->ListWidget_POVBindings, SignalType::DISCRETE_POV);
    Initialize_VJoy(0);

    Initialize_comboboxes(current_device);
}
void MainWindow::Create_new_Device(uint8_t vjoy_id)
{
    assert(vjoy_id >= 1 && vjoy_id <= 16);
    qDebug() << "Created device " << vjoy_id;
    devices.push_back(new vJDevice(vjoy_id));
    current_device = devices.back();
}
void MainWindow::on_actionAbout_triggered()
{
    WORD VerDll, VerDrv;
    DriverMatch(&VerDll, &VerDrv);
    QString info {"<center>Virtual Joystick feeder v" + VERSION + "</center><br>"};
    info += "<center><a href='https://github.com/hobbitinisengard/virtual_joystick'>https://github.com/hobbitinisengard/virtual_joystick</a></center><br>";
    info += "This application uses an updated ";
    info += QString::fromWCharArray((PWSTR)GetvJoyProductString()) + " library originally made by ";
    info += QString::fromWCharArray((PWSTR)GetvJoyManufacturerString())+ " and updated by <a href='https://github.com/njz3/vJoy'>njz3</a>.<br>";
    info += "DLL version: " + QString::number(VerDll) + "<br>";
    info += "Driver version: " + QString::number(VerDrv) + " ("+QString::fromWCharArray((PWSTR)GetvJoySerialNumberString()) + ")";
    Create_OK_MessageBox(info);
}
void MainWindow::Quit()
{
    for(const auto device : devices)
        RelinquishVJD(device->id);
    QApplication::quit();
}
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
    {
        for(auto& binding : bindings)
        {
            auto *keyEvent = static_cast<QKeyEvent *>(event);
            //event->key() != Qt::Key_Escape
            if(binding->key == keyEvent->key())
            {
                vJDevice::Send_data(binding, event->type() == QEvent::KeyPress);
            }
        }
    }
    return QMainWindow::eventFilter(object, event);
}
void MainWindow::Initialize_comboboxes(const vJDevice *device)
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
    axis_bindings->comboBox->setModel(axes_model);

    // populate data from .ini or memory
    // code
    // populate discrete POV combobox
    QStringListModel *pov_model = new QStringListModel(this);
    QStringList pov_list;
    for(size_t i=0; i<4; ++i){
        for(size_t j=0; j<4; ++j)
            pov_list << "POV " + QString::number(i+1) + "-" + QString::number(j);
    }
    pov_model->setStringList(pov_list);
    pov_bindings->comboBox->setModel(pov_model);
    // populate data from .ini or memory
    // code
}
std::vector<std::string> MainWindow::split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    res.push_back (s.substr (pos_start));
    return res;
}
// int curr_index = ui->comboBox_configs->currentIndex();
//    if(curr_index == -1)
//    { // empty combobox
//        filepath = QFileDialog::getSaveFileName(
//                    this,tr("Select vjoy configuration file"), "",
//                    tr("ini files (*.ini);;All Files (*)")).toStdString();
//        if(filepath == "")
//            return;
//         Add_2_ini_combobox(filepath);
//         ++curr_index;
//    }
//    else
//    { // combobox selected
//        filepath = ui->comboBox_configs->itemData(curr_index).value<std::string>();
//    }
void MainWindow::Load_configuration()
{
    std::string filepath;
    filepath = QFileDialog::getOpenFileName(
                        this,tr("Select vjoy configuration file"), "",
                        tr("ini files (*.ini);;All Files (*)")).toStdString();
    if(filepath == "")
        return;

    bindings.clear();
    std::ifstream inf {filepath};
    while (inf.good())
    {
        // read and insert data to cleared bindings QList.

        std::string strInput;
        std::getline(inf, strInput);
        if(strInput == "")
            break;
        auto ln = split(strInput, " ");
        int id = std::stoi(ln[0]);
        SignalType type = (SignalType) std::stoi(ln[1]);
        int key = std::stoi(ln[2]);
        int macro = std::stoi(ln[3]);
        Direction direction = (Direction) std::stoi(ln[4]);
        bindings.append(new Binding(id,type,key,macro,direction));
    }
    UpdateName(filepath);
    Switch_device(current_device->id-1);
}
//Binding(uint8_t ID, SignalType TYPE, int KEY, uint16_t MACRO, Direction DIR = Direction::POSITIVE)
void MainWindow::Save_configuration()
{
    std::string filepath = QFileDialog::getSaveFileName(
                this,tr("Save vjoy configuration file"), "",
                tr("ini files (*.ini);;All Files (*)")).toStdString();
    if(filepath == "")
        return;

    std::ofstream outf {filepath, std::ios::trunc};
    for(auto& bind : bindings)
    {
    outf << (int)bind->device_id << " ";
    outf << (int)bind->type << " ";
    outf << bind->key << " ";
    outf << (int)bind->macro << " ";
    outf << (int)bind->direction << '\n';
    }
    UpdateName(filepath);
    int temp = current_device->id-1;
    Switch_device(temp);
}
void MainWindow::UpdateName(std::string filepath)
{
    QString ini_name = QString::fromStdString(remove_extension(base_name(filepath)));
    ui->lineEdit_Name->setText(ini_name);
}
template<class T>
T MainWindow::base_name(T const & path, T const & delims /*= "/\\"*/)
{
  return path.substr(path.find_last_of(delims) + 1);
}
template<class T>
T MainWindow::remove_extension(T const & filename)
{
  typename T::size_type const p(filename.find_last_of('.'));
  return p > 0 && p != T::npos ? filename.substr(0, p) : filename;
}
void MainWindow::Create_OK_MessageBox(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setWindowTitle("vJoy keyboard feeder");
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
    Scan(*button_bindings);
}
void MainWindow::on_Button_ScanButtonOrAxis_clicked()
{
    Scan(*axis_bindings);
}
void MainWindow::on_Button_ScanPOVs_clicked()
{
    Scan(*pov_bindings);
}

void MainWindow::Scan(BindingStruct &binding_struct)
{
    ScanBox Scanning_monolog_box(binding_struct, current_device, bindings, axisdata);
    Scanning_monolog_box.exec();
}
void MainWindow::Switch_device(int index)
{
    qDebug() << "bindings count=" << bindings.count();
    int vjoy_index = index+1;
    if(vjoy_index <= 0 || vjoy_index>=17)
    {
        int a = 5;
    }
    Clear_Listwidgets();
    //check if device with id of index exists
    auto device = std::find_if(devices.begin(), devices.end(), [vjoy_index](vJDevice *d){return d->id == vjoy_index;});
    if(device == devices.end())
    {// if not, create device.
        try
        {
             Create_new_Device(vjoy_index);
        }
        catch(VJD_STAT_BUSY_Exception &e)
        {
            Create_OK_MessageBox("This device is already owned by another feeder");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
        catch(VJD_STAT_MISS_Exception &e)
        {
            Create_OK_MessageBox("This vJoy device is not installed or disabled. Feeder stopped.");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
        catch(...)
        {
            Create_OK_MessageBox("vJoy device error. Enable the virtual device using 'Configure vJoy' program. After that, restart this application.");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
    }
    else
    { // device exists

        qDebug() << "device exists";
        current_device = *device;
    }

    Initialize_comboboxes(current_device);
    // reconstruct listwidget of bindings
    for(auto &bind: bindings)
    {
        if(bind->device_id != current_device->id)
            continue;
        qDebug() << "Bind: " << bind->key;
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        if(bind->type == SignalType::BUTTON)
        {
            i->setText("Button " + QString::number(bind->macro) + " --- " + QKeySequence(bind->key).toString());
            i->setData(100, bind->key);
            button_bindings->data->addItem(i);
        }
        else if (bind->type == SignalType::AXIS)
        {
            // find name of axis on macro
            auto axisdata_it = std::find_if(axisdata.begin(), axisdata.end(),
                                            [&bind](AxisData *a){return a->macro == bind->macro;});

            i->setText((*axisdata_it)->name + " --- " + QKeySequence(bind->key).toString());
            i->setData(100, bind->key);
            axis_bindings->data->addItem(i);
        }
        else if(bind->type == SignalType::DISCRETE_POV)
        {
            int value = (uint8_t)bind->macro; // <0; 3>
            int npov = (uint8_t)(bind->macro >> 8); // <1;4>
            i->setText("POV" + QString::number(npov) + " " + QString::number(value) +
                       " --- " + QKeySequence(bind->key).toString());
            i->setData(100, bind->key);
            pov_bindings->data->addItem(i);
        }
    }
}
void MainWindow::on_vjoy_tabs_currentChanged(int index)
{
    Switch_device(index);
}

void MainWindow::Delete_from_bindings(QListWidgetItem *item)
{
    int keycode = item->data(100).toInt();
    for(int i=0; i<bindings.count(); ++i){
        if(keycode == bindings[i]->key){
            bindings.removeAt(i);
            //if(MainWindow::allow_multiple_keybindings == false)
            //    --i;
            //else //
                return;
        }
    }
}
void MainWindow::on_ListWidget_ButtonBindings_itemDoubleClicked(QListWidgetItem *item)
{
    Delete_from_bindings(item);
    ui->ListWidget_ButtonBindings->removeItemWidget(item);
    delete item;
}

void MainWindow::on_listWidget_AxesBinding_itemDoubleClicked(QListWidgetItem *item)
{
    Delete_from_bindings(item);
    ui->listWidget_AxesBinding->removeItemWidget(item);
    delete item;
}
void MainWindow::on_ListWidget_POVBindings_itemDoubleClicked(QListWidgetItem *item)
{
    Delete_from_bindings(item);
    ui->ListWidget_POVBindings->removeItemWidget(item);
    delete item;
}

void MainWindow::on_Checkbox_AllowMultipleKeybinding_stateChanged(int arg1)
{
    this->allow_multiple_keybindings = arg1;
}


void MainWindow::Clear_Listwidgets()
{
    ui->listWidget_AxesBinding->clear();
    ui->ListWidget_POVBindings->clear();
    ui->ListWidget_ButtonBindings->clear();
}
void MainWindow::on_button_CLEAR_clicked()
{
    bindings.removeIf([this](Binding *a) { return a->device_id == current_device->id; });
    Clear_Listwidgets();
}

void MainWindow::on_button_LOAD_clicked()
{
    Load_configuration();
}


void MainWindow::on_button_SAVE_clicked()
{
    Save_configuration();
}


void MainWindow::on_ListWidget_ButtonBindings_itemClicked(QListWidgetItem *item)
{

}


void MainWindow::on_listWidget_AxesBinding_itemClicked(QListWidgetItem *item)
{

}

void MainWindow::on_ListWidget_POVBindings_itemClicked(QListWidgetItem *item)
{

}



