#include "mainwindow.h"
#include "scanbox.h"
// this function runs during key registering
void ScanBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_Escape)
    {
        if(!MainWindow::allow_multiple_keybindings)
        {
            for(auto& bin : bindings)
            {
                if((event->key() == bin->key) && (bin->device_id == device->id))
                    return;
            }
        }

        // register keyboard key to virtual axis/button
        // add entry to ui list
        qDebug() << "Registered code=" << event->key();
        QString Button_str = binding_struct.comboBox->currentText();
        QListWidgetItem *i = new QListWidgetItem();
        i->setTextAlignment(Qt::AlignCenter);
        i->setText(Button_str + " --- " + QKeySequence(event->key()).toString());
        i->setData(100, event->key());
        binding_struct.data->addItem(i);

        uint8_t selected_combobox_id = binding_struct.comboBox->currentIndex();
        binding_struct.comboBox->setCurrentIndex((selected_combobox_id+1) % binding_struct.comboBox->count());
        uint16_t macro = 0;
        Binding *binding;
        switch(binding_struct.type)
        {
        case SignalType::BUTTON:
            macro = selected_combobox_id+1; // buttons's indexes start from 1
            binding = new Binding(device->id, binding_struct.type, event->key(), macro);
            break;
        case SignalType::AXIS:
            macro = axisdata[selected_combobox_id]->macro;
            binding = new Binding(device->id, binding_struct.type, event->key(),
                                  macro, axisdata[selected_combobox_id]->direction);
            break;
        case SignalType::DISCRETE_POV:
            // encode button number in bits 0-8
            macro = selected_combobox_id % 4;
            // encode pov number in bits 9-16
            macro |= (((selected_combobox_id) / 4) + 1) << 8;
            binding = new Binding(device->id, binding_struct.type, event->key(), macro);
            break;
        case SignalType::CONTINUOUS_POV:
            binding = new Binding(device->id, binding_struct.type, event->key(), selected_combobox_id);
            break;
        }
        bindings.append(binding);
        qDebug() << bindings.count();
    }
    this->done(0);
}


