#ifndef CONSETTING_H
#define CONSETTING_H

#include <QDialog>

namespace Ui {
class consetting;
}

class consetting : public QDialog
{
    Q_OBJECT

public:
    explicit consetting(QWidget *parent = 0);
    ~consetting();
    int get_con_mode();
    QString get_ip_address();
    int get_port();

private slots:
    void on_usb_radio_clicked();

    void on_lan_radio_clicked();

    void on_conform_close_clicked();

private:
    Ui::consetting *ui;
};

#endif // CONSETTING_H
