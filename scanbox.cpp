#include "scanbox.h"
// this function runs during key registering
void ScanBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_Escape)
    {
        // register keyboard key to virtual axis/button
        // add entry to ui list
        QString Button_str = binding_struct->comboBox->currentText();
        QListWidgetItem *i = new QListWidgetItem();
        i->setText(Button_str + " --- " + event->text());
        i->setData(100, device->id);
        binding_struct->data->addItem(i);
        // register
        Binding *binding = new Binding(device->id, binding_struct->type, event->key());
        bindings->push_back(binding);
    }
    this->done(0);
}


