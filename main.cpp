#include <libusb-1.0/libusb.h>
#include "owoncontrol.h"
#include "warning.h"
#include "ui_owoncontrol.h"
#include "usb_interface.h"
#include <QApplication>
#include <QDesktopWidget>

owoncontrol * window_ptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    owoncontrol w;
    warning w2;

    /* Set the main window central */
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()- w.width()) / 2;
    int y = (screenGeometry.height()- w.height()) / 2;
    w.move(x, y);

    /* Set the dialog box central */
    x = (screenGeometry.width()- w2.width()) / 2;
    y = (screenGeometry.height()- w2.height()) / 2;
    w2.move(x, y);

    /* Show the warning dialog */
    w2.show();

    /* Show the UI */
    w.show();

    /* Get all of the default values from the UI elements */
    w.init_vars();

    /* Assign a pointer to the main UI */
    window_ptr = &w;

    return a.exec();
}
