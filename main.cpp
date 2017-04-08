#include <libusb-1.0/libusb.h>
#include "owoncontrol.h"
#include "warning.h"
#include "consetting.h"
#include "ui_owoncontrol.h"
#include "usb_interface.h"
#include <QApplication>
#include <QDesktopWidget>

owoncontrol * window_ptr;
consetting * cest_frm_ptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    owoncontrol w;
    warning w2;
    consetting cset_form;

    /* Assign a pointer to the main UI */
    window_ptr = &w;

    /* Assign a ptr to the settings form */
    cest_frm_ptr = &cset_form;

    /* Set the main window central */
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()- w.width()) / 2;
    int y = (screenGeometry.height()- w.height()) / 2;
    w.move(x, y);

    /* Set the dialog box central */
    x = (screenGeometry.width()- w2.width()) / 2;
    y = (screenGeometry.height()- w2.height()) / 2;
    w2.move(x, y);

    /* Set the connection form central */
    x = (screenGeometry.width()- cset_form.width()) / 2;
    y = (screenGeometry.height()- cset_form.height()) / 2;
    cset_form.move(x, y);

    /* Show the warning dialog */
    w2.show();

    /* Show the UI */
    w.show();

    /* Get all of the default values from the UI elements */
    w.init_vars();

    return a.exec();
}
