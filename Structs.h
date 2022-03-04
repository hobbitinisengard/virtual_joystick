#ifndef STRUCTS_H
#define STRUCTS_H
#include <QString>
#include <malloc.h>
#include <vjdevice.h>
#include <QListWidget>
#include <QComboBox>
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
    uint8_t device_id;
    SignalType type;
    // mapped keyboard key
    int key;
    uint8_t macro;
    Direction direction;
    Binding(uint8_t ID, SignalType TYPE, int KEY, uint8_t MACRO, Direction DIR = Direction::POSITIVE)
        : device_id(ID), type(TYPE), key(KEY), macro(MACRO), direction(DIR)
    {};
};
class BindingStruct
{
public:
    BindingStruct(QComboBox *COMBO, QListWidget *DATA, SignalType signaltype)
        :comboBox(COMBO), data(DATA), type(signaltype)
    {};
    QComboBox *comboBox;
    QListWidget *data;
    SignalType type;
};
#endif // STRUCTS_H
