#include "scanbox.h"
// this function runs during key registering
void ScanBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_Escape)
    {
        for(auto& binding : bindings)
        {
            int a = 5;
        }
        // register keyboard key to virtual axis/button
        // add entry to ui list
        qDebug() << "Registered code=" << event->key();
        QString Button_str = binding_struct->comboBox->currentText();
        QListWidgetItem *i = new QListWidgetItem();
        i->setText(Button_str + " --- " + event->text());
        i->setData(100, device->id);
        binding_struct->data->addItem(i);

        int selected_combobox_id = binding_struct->comboBox->currentIndex();
        Binding *binding;
        switch(binding_struct->type)
        {
        case SignalType::BUTTON:
            binding = new Binding(device->id, binding_struct->type, event->key(), selected_combobox_id);
            break;
        case SignalType::AXIS:
            binding = new Binding(device->id, binding_struct->type, event->key(),
                                  axisdata[selected_combobox_id]->macro, axisdata[selected_combobox_id]->direction);
            break;
        case SignalType::DISCRETE_POV:
            binding = new Binding(device->id, binding_struct->type, event->key(), selected_combobox_id);
            break;
        case SignalType::CONTINUOUS_POV:
            binding = new Binding(device->id, binding_struct->type, event->key(), selected_combobox_id);
            break;
        }
        bindings.push_back(binding);
    }
    this->done(0);
}


