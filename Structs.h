#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>
#include <malloc.h>
#include <vjdevice.h>
#include <QListWidget>
#include <QComboBox>
#include <exception>
enum class Direction {NEGATIVE, POSITIVE};
enum class SignalType{BUTTON, AXIS, DISCRETE_POV, CONTINUOUS_POV };
struct AxisData
{
    int macro;
    QString name;
    Direction direction;
    AxisData(const int MACRO, const QString& NAME, Direction DIRECTION = Direction::POSITIVE)
        :macro(MACRO), name(NAME), direction(DIRECTION)
    {};
};
struct Binding;
struct BindingStruct
{
    BindingStruct(QComboBox *COMBO, QListWidget *DATA)
        :comboBox(COMBO), data(DATA)
    {}
    virtual ~BindingStruct()
    {}
    QComboBox *comboBox;
    QListWidget *data;
    virtual void Initialize_comboboxes(int device_id) = 0;
    virtual void Add_item(Binding &b) = 0;
    virtual Binding* Create_binding(uint8_t id, int key, int macro, uint8_t selected_combobox_id) = 0;
    virtual int Send_data(const Binding &b, bool Pressed_not_released) = 0;
};
struct Binding
{
    // device_id can be between <1;16>.
    uint8_t device_id;
    BindingStruct *bindingstruct;
    // mapped keyboard key
    int key;
    uint16_t macro;
    Direction direction;
    Binding(uint8_t ID, BindingStruct *BINDINGSTRUCT, int KEY, uint16_t MACRO, Direction DIR = Direction::POSITIVE)
        : device_id(ID), bindingstruct(BINDINGSTRUCT), key(KEY), macro(MACRO), direction(DIR)
    {}
};

struct Button_bindingStruct : BindingStruct
{
    Button_bindingStruct(QComboBox *COMBO, QListWidget *DATA)
        :BindingStruct(COMBO, DATA)
    {}
    virtual ~Button_bindingStruct()
    {}
    void Initialize_comboboxes(const int device_id) override
    {
        QStringListModel *button_model = new QStringListModel();
        QStringList button_list;
        int buttons_number = GetVJDButtonNumber(device_id);
        for(int i=0; i<buttons_number; ++i){
            button_list << "Button " + QString::number(i+1);
        }
        button_model->setStringList(button_list);
        this->comboBox->setModel(button_model);
    }
    void Add_item(Binding& bind) override
    {
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        i->setText("Button " + QString::number(bind.macro) + " --- " + QKeySequence(bind.key).toString());
        i->setData(100, bind.key);
        this->data->addItem(i);
    }
    Binding* Create_binding(uint8_t id, int key, int macro, uint8_t selected_combobox_id) override
    {
        macro = selected_combobox_id+1; // buttons's indexes start from 1
        return new Binding(id, this, key, macro);
    }
    int Send_data(const Binding &b, bool Pressed_not_released) override
    {
        return SetBtn(Pressed_not_released, b.device_id, b.macro);
    }
};
struct Axis_bindingStruct : BindingStruct
{
    inline static const std::vector<AxisData*> axisdata {
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
    Axis_bindingStruct(QComboBox *COMBO, QListWidget *DATA)
        :BindingStruct(COMBO, DATA)
    {}
    virtual ~Axis_bindingStruct()
    {}
    void Initialize_comboboxes(const int device_id) override
    {
        QStringListModel *axes_model = new QStringListModel();
        QStringList buttonaxes_list;
        for(size_t i=0; i<axisdata.size(); ++i){
            if(!GetVJDAxisExist(device_id, axisdata[i]->macro))
            {
                qCritical("MAX axis i ");
                break;
            }
            buttonaxes_list << axisdata[i]->name;
        }

        axes_model->setStringList(buttonaxes_list);
        this->comboBox->setModel(axes_model);
    }
    void Add_item(Binding& bind) override
    {
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        // find name of axis on macro
        auto axisdata_it = std::find_if(axisdata.begin(), axisdata.end(),
                                        [&bind](AxisData *a){return a->macro == bind.macro;});

        i->setText((*axisdata_it)->name + " --- " + QKeySequence(bind.key).toString());
        i->setData(100, bind.key);
        this->data->addItem(i);
    }
    Binding* Create_binding(uint8_t id, int key, int macro, uint8_t selected_combobox_id) override
    {
        macro = axisdata[selected_combobox_id]->macro;
        return new Binding(id, this, key, macro, axisdata[selected_combobox_id]->direction);
    }
    int Send_data(const Binding &b, bool Pressed_not_released) override
    {
        if(b.direction == Direction::POSITIVE)
            return SetAxis(Pressed_not_released ? 32767 : 16383, b.device_id, b.macro);
        else
            return SetAxis(Pressed_not_released ? 0 : 16383, b.device_id, b.macro);
    }
};
struct POV_bindingStruct : BindingStruct
{
    POV_bindingStruct(QComboBox *COMBO, QListWidget *DATA)
        :BindingStruct(COMBO, DATA)
    {};
    virtual ~POV_bindingStruct()
    {}
    void Initialize_comboboxes(const int device_id) override
    {
        QStringListModel *pov_model = new QStringListModel();
        QStringList pov_list;
        int povs_number = GetVJDDiscPovNumber(device_id);
        for(int i=0; i<povs_number; ++i){
            for(int j=0; j<4; ++j)
                pov_list << "POV " + QString::number(i+1) + "-" + QString::number(j);
        }
        pov_model->setStringList(pov_list);
        this->comboBox->setModel(pov_model);
    }
    void Add_item(Binding& bind) override
    {
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        int value = (uint8_t)bind.macro; // <0; 3>
        int npov = (uint8_t)(bind.macro >> 8); // <1;4>
        i->setText("POV" + QString::number(npov) + " " + QString::number(value) +
                   " --- " + QKeySequence(bind.key).toString());
        i->setData(100, bind.key);
        this->data->addItem(i);
    }
    Binding* Create_binding(uint8_t id, int key, int macro, uint8_t selected_combobox_id) override
    {
        // encode button number in bits 0-8
        macro = selected_combobox_id % 4;
        // encode pov number in bits 9-16
        macro |= (((selected_combobox_id) / 4) + 1) << 8;
        return new Binding(id, this, key, macro);
    }
    int Send_data(const Binding &b, bool Pressed_not_released) override
    {
        uint8_t value;
        uint8_t npov;
        if(Pressed_not_released){
            value = (uint8_t)b.macro; // <0; 3>
            npov = (uint8_t)(b.macro >> 8); // <1;4>
        }
        return SetDiscPov(value, b.device_id, npov);
    }
};

struct VJD_STAT_BUSY_Exception : public std::exception
{
  virtual const char* what() const throw()
  {
    return "This device is already owned by another feeder";
  }
};
struct VJD_STAT_MISS_Exception : public std::exception
{
  virtual const char* what() const throw()
  {
    return "This vJoy device is not installed or disabled.";
  }
};
struct already_exists_exception : public std::exception
{
  virtual const char* what() const throw()
  {
    return "Argument already exists";
  }
};

#endif // STRUCTS_H
