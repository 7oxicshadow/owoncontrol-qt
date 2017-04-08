#ifdef __linux__
    #include <sys/socket.h>
#else
    #include <winsock2.h>
#endif

#include <libusb-1.0/libusb.h>
#include <arpa/inet.h>
#include <QGraphicsScene>
#include "usb_interface.h"
#include "lan_interface.h"
#include "owoncontrol.h"
#include "consetting.h"
#include "main.h"
#include <unistd.h>

/********************/
/* Global Variables */
/********************/
int socket_desc;
int con_status;
struct sockaddr_in server;

/********************/
/* Code Starts Here */
/********************/
int connect_lan(void)
{
    QByteArray byteArray;
    const char * text;
    QString success_str;


    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0); //IPv4 / TCP / IP Protocol
    if (socket_desc == -1)
    {
        window_ptr->append_log("Could not create socket");
        return(1);
    }

    /* convert QString to const char */
    byteArray = cest_frm_ptr->get_ip_address().toUtf8();
    text = byteArray.constData();

    server.sin_family = AF_INET;

    server.sin_addr.s_addr = inet_addr(text);
    server.sin_port = htons( cest_frm_ptr->get_port() );

    //server.sin_addr.s_addr = inet_addr("192.168.1.72");
    //server.sin_port = htons( 3000 );

    //Connect to remote server
    con_status = connect(socket_desc , (struct sockaddr *)&server , sizeof(server));
    if ( con_status == -1 )
    {
       window_ptr->append_log("LAN Connection Error");
       return (1);
    }

    success_str = "LAN Connected to " + cest_frm_ptr->get_ip_address();
    window_ptr->append_log(success_str);

    return(0);

}

int disconnect_lan(void)
{
    close(socket_desc);
    con_status = -1;
    window_ptr->append_log ("Disconnect LAN successful");
    return(0);
}

int send_lan_data(char* data, int length)
{
    int tx_count = 0U;

    if(con_status != -1)
    {
        //Send some data
        tx_count = send(socket_desc ,data , length , 0);

        if( tx_count < 0)
        {
            window_ptr->append_log("Send failed");
            return (1);
        }
        else
        {
            if(tx_count != length)
            {
                window_ptr->append_log("Byte Count Mismatch");
                return (1);
            }
        }

    }
    else
    {
        window_ptr->append_log ("Failed to send command, Is the LAN connetion active?");
        return(1);
    }

    return(0);
}

int get_lan_data(char* data, int length, int * transferred)
{
    if(con_status != -1)
    {
        *transferred = recv(socket_desc, data, length, 0);

        if( *transferred < 0)
        {
            window_ptr->append_log ("Read Failed");
            return (1);
        }
    }
    else
    {
        window_ptr->append_log ("Failed to read command, Is the LAN connetion active?");
        return (1);
    }

    return (0);
}

int get_lan_bmp(void)
{
    int allocated = 0;
    int transferred = 0;
    int downloaded = 0;

    union
    {
        char data[12];
        owon_start_response_st response;
    }tempdata_u;

    /* Start the USB transfer */
    send_lan_data(start_bmp, sizeof(start_bmp));

    /* Get the response */
    if( get_lan_data(&tempdata_u.data[0], sizeof(tempdata_u), &transferred) )
    {
        return(1);
    }

    /* Clear previous buffer if exists */
    if(NULL != bmp_buffer)
    {
        free(bmp_buffer);
        bmp_buffer = nullptr;
    }

    /* Allocate memory to receive bmp */
    bmp_buffer = (char*) malloc(tempdata_u.response.length);
    allocated = tempdata_u.response.length;

    //printf("Allocated: %d\n",allocated);

    if(NULL == bmp_buffer)
    {
        window_ptr->append_log ("Failed to allocate Buffer!");
        return -1;
    }

    /* Get the data from the oscilloscope */
    do
    {
        if( get_lan_data(bmp_buffer + downloaded, allocated, &transferred) )
        {
            return (1);
        }

        downloaded += transferred;

        //printf("Running download: %d\n",downloaded);

    } while (allocated > downloaded);

    //printf("Downloaded: %d\n",downloaded);

    return 0;

}
