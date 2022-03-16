#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    installEventFilter(this);
    bs_buttons = new Button_bindingStruct(ui->AllButtons_ComboBox,
                                              ui->ListWidget_ButtonBindings);
    bs_axes = new Axis_bindingStruct(ui->AllButtonsAndAxesComboBox,
                                     ui->listWidget_AxesBinding);
    bs_povs = new POV_bindingStruct(ui->AllPovs_ComboBox,
                                    ui->ListWidget_POVBindings);
    binding_structs.push_back(bs_buttons);
    binding_structs.push_back(bs_axes);
    binding_structs.push_back(bs_povs);
    connector = new VJoyConnector();

    for(auto &binding_struct : binding_structs)
        binding_struct->Initialize_comboboxes(connector->current_device->id);
}
void MainWindow::on_actionAbout_triggered()
{
    WORD VerDll, VerDrv;
    DriverMatch(&VerDll, &VerDrv);
    QString info {"<center>Virtual Joystick feeder v" + VERSION + "</center><br>"};
    info += "<center><a href='https://github.com/hobbitinisengard/virtual_joystick'>Link</a></center><br>";
    info += "This application uses an updated ";
    info += QString::fromWCharArray((PWSTR)GetvJoyProductString()) + " library originally made by ";
    info += QString::fromWCharArray((PWSTR)GetvJoyManufacturerString())+ " and updated by <a href='https://github.com/njz3/vJoy'>njz3</a>.<br>";
    info += "DLL version: " + QString::number(VerDll) + "<br>";
    info += "Driver version: " + QString::number(VerDrv) + " ("+QString::fromWCharArray((PWSTR)GetvJoySerialNumberString()) + ")";
    InfoBox infobox(info);
}
void MainWindow::Quit()
{   
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
                bool success = binding->bindingstruct->Send_data(*binding, event->type() == QEvent::KeyPress);
                if(!success){
                    qWarning("Couldn't send signal to vjoystick");
                }
            }
        }
    }
    return QMainWindow::eventFilter(object, event);
}
std::vector<std::string> MainWindow::split (std::string s, std::string delimiter)
{
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
BindingStruct* MainWindow::Get_bindingstruct_of_type(const SignalType type) const
{
    switch(type){
    case SignalType::BUTTON:
        for(auto &bs : binding_structs){
            if(dynamic_cast<Button_bindingStruct*>(bs) != nullptr){
                return bs;
            }
        }
        break;
    case SignalType::AXIS:
        for(auto &bs : binding_structs){
            if(dynamic_cast<Axis_bindingStruct*>(bs) != nullptr){
                return bs;
            }
        }
        break;
    case SignalType::DISCRETE_POV:
        for(auto &bs : binding_structs){
            if(dynamic_cast<POV_bindingStruct*>(bs) != nullptr){
                return bs;
            }
        }
        break;
    case SignalType::CONTINUOUS_POV:
//        for(auto &bs : binding_structs){
//            if(dynamic_cast<POV_bindingStruct*>(bs) != nullptr){
//                return bs;
//            }
//        }
        break;
    }
    throw std::out_of_range("haven't found correct binding struct");
}
int MainWindow::Convert_bindingstruct_to_type(BindingStruct* bs) const
{
    if(dynamic_cast<Button_bindingStruct*>(bs) != nullptr){
        return (int)SignalType::BUTTON;
    }
    if(dynamic_cast<Axis_bindingStruct*>(bs) != nullptr){
        return (int)SignalType::AXIS;
    }
    if(dynamic_cast<POV_bindingStruct*>(bs) != nullptr){
        return (int)SignalType::DISCRETE_POV;
    }
    throw std::out_of_range("No.");
}

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

        BindingStruct* binding_struct = Get_bindingstruct_of_type(type);
        bindings.append(new Binding(id,binding_struct,key,macro,direction));
    }
    UpdateName(filepath);
    Switch_device(connector->current_device->id-1);
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
    outf << Convert_bindingstruct_to_type(bind->bindingstruct) << " ";
    outf << bind->key << " ";
    outf << (int)bind->macro << " ";
    outf << (int)bind->direction << '\n';
    }
    UpdateName(filepath);
    int temp = connector->current_device->id-1;
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Button_ScanBinding_clicked()
{
    Scan(*bs_buttons);
}
void MainWindow::on_Button_ScanButtonOrAxis_clicked()
{
    Scan(*bs_axes);
}
void MainWindow::on_Button_ScanPOVs_clicked()
{
    Scan(*bs_povs);
}

void MainWindow::Scan(BindingStruct &binding_struct)
{
    ScanBox Scanning_monolog_box(binding_struct, connector->current_device, bindings);
    Scanning_monolog_box.exec();
}
void MainWindow::Switch_device(int index)
{
    qDebug() << "bindings count=" << bindings.count();
    int vjoy_index = index+1;
    Clear_Listwidgets();
    //check if device with id of index exists
    auto device = std::find_if(connector->devices.begin(), connector->devices.end(),
                               [vjoy_index](vJDevice *d){return d->id == vjoy_index;});
    if(device == connector->devices.end())
    {// if not, create device.
        try
        {
             connector->Create_new_Device(vjoy_index);
        }
        catch(VJD_STAT_BUSY_Exception &e)
        {
            InfoBox info("This device is already owned by another feeder");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
        catch(VJD_STAT_MISS_Exception &e)
        {
             InfoBox info("This vJoy device is not installed or disabled. Feeder stopped.");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
        catch(...)
        {
             InfoBox info("vJoy device error. Enable the virtual device using 'Configure vJoy' program. After that, restart this application.");
            ui->vjoy_tabs->setCurrentIndex(0);
            return;
        }
    }
    else
    { // device exists

        qDebug() << "device exists";
        connector->current_device = *device;
    }

    for(auto &binding_struct : binding_structs)
        binding_struct->Initialize_comboboxes(connector->current_device->id);
    // reconstruct listwidget of bindings
    for(const auto &bind: bindings)
    {
        if(bind->device_id != connector->current_device->id)
            continue;
        qDebug() << "Bind: " << bind->key;
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        bind->bindingstruct->Add_item(*bind);
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
    bindings.removeIf([this](Binding *a) { return a->device_id == connector->current_device->id; });
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



