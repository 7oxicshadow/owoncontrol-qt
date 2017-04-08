#include "connection.h"
#include <libusb-1.0/libusb.h>
#include "lan_interface.h"
#include "usb_interface.h"
#include "owoncontrol.h"
#include "consetting.h"
#include "main.h"

/********************/
/* Global Variables */
/********************/


/********************/
/* Code Starts Here */
/********************/
int scope_connect(void)
{
    int type;
    type = cest_frm_ptr->get_con_mode();

    if(type == USB_CONNECTION)
    {
        if( connect_usb() )
            return(1);
    }
    else if(type == LAN_CONNECTION)
    {
        if( connect_lan() )
            return(1);
    }
    else
    {
        return(1);
    }

    return(0);
}

int scope_disconnect(void)
{
    int type;
    type = cest_frm_ptr->get_con_mode();

    if(type == USB_CONNECTION)
    {
        if( disconnect_usb() )
            return(1);
    }
    else if(type == LAN_CONNECTION)
    {
        if( disconnect_lan() )
            return(1);
    }
    else
    {
        return(1);
    }

    return(0);
}

int scope_send_message(char* data, int length)
{
    int type;
    type = cest_frm_ptr->get_con_mode();

    if(type == USB_CONNECTION)
    {
        if( send_usb_data(data, length) )
            return(1);
    }
    else if(type == LAN_CONNECTION)
    {

        if( send_lan_data(data, length) )
            return(1);
    }
    else
    {
        return(1);
    }

    return(0);
}

int scope_get_bmp(void)
{
    int type;
    type = cest_frm_ptr->get_con_mode();

    if(type == USB_CONNECTION)
    {
        get_bmp_usb();
        return(0);
    }
    else if(type == LAN_CONNECTION)
    {
        get_lan_bmp();
        return(0);
    }
    else
    {
        return(1);
    }

    return(0);
}
