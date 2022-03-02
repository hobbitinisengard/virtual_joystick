#include "scanbox.h"

void ScanBox::keyPressEvent(QKeyEvent *event)
{
    if(event->key() != Qt::Key_Escape)
    {
        // add binding to listwidget
        QListWidgetItem *i = new QListWidgetItem();
        QString Button_str = binding_struct->comboBox->currentText();
        i->setText(Button_str + " --- " + event->text());
        binding_struct->data->addItem(i);
    }
    this->done(0);
}


