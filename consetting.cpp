#include "consetting.h"
#include "ui_consetting.h"
#include "connection.h"

consetting::consetting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::consetting)
{
    ui->setupUi(this);
}

consetting::~consetting()
{
    delete ui;
}

int consetting::get_con_mode()
{
    int ret_val;

    if( ui->usb_radio->isChecked() == true )
        ret_val = USB_CONNECTION;
    else
        ret_val = LAN_CONNECTION;

    return(ret_val);
}

QString consetting::get_ip_address()
{
    return( ui->ip_lineedit->text() );
}

int consetting::get_port()
{
     return( ui->port_spinBox->value() );
}

void consetting::on_usb_radio_clicked()
{
    ui->lan_groupbox->setEnabled(false);
}

void consetting::on_lan_radio_clicked()
{
    ui->lan_groupbox->setEnabled(true);
}

void consetting::on_conform_close_clicked()
{
    close();
}
