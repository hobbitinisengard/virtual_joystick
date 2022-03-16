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
        Binding *binding = binding_struct.Create_binding(device->id, event->key(), macro, selected_combobox_id);
        bindings.append(binding);
        qDebug() << bindings.count();
    }
    this->done(0);
}


