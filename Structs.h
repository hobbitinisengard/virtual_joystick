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
struct Binding
{
    // device_id can be between <1;16>. MainWindow::current_device_index refers to a devices vector,
    //so its corresponding values are device_id-1
    uint8_t device_id;
    SignalType type;
    // mapped keyboard key
    int key;
    uint16_t macro;
    Direction direction;
    Binding(uint8_t ID, SignalType TYPE, int KEY, uint16_t MACRO, Direction DIR = Direction::POSITIVE)
        : device_id(ID), type(TYPE), key(KEY), macro(MACRO), direction(DIR)
    {};
    ~Binding()
    {
        qDebug() << "destr";
    }
};
struct BindingStruct
{
    BindingStruct(QComboBox *COMBO, QListWidget *DATA, SignalType signaltype)
        :comboBox(COMBO), data(DATA), type(signaltype)
    {
    };
    QComboBox *comboBox;
    QListWidget *data;
    SignalType type;
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
#endif // STRUCTS_H
