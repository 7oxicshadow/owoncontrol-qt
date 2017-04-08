#include <libusb-1.0/libusb.h>
#include <QGraphicsScene>

#include "usb_interface.h"
#include "owoncontrol.h"
#include "consetting.h"
#include "main.h"

#define USB_LOCK_VENDOR 0x5345			  // Dev : (5345) Owon Technologies
#define USB_LOCK_PRODUCT 0x1234			  // (1234) PDS Digital Oscilloscope
#define BULK_WRITE_ENDPOINT 0x03
#define BULK_READ_ENDPOINT 0x81

/*********************/
/* USB Command Lists */
/*********************/
char start_bmp[STARTBMP_SIZE] = {0x53,0x54,0x41,0x52,0x54,0x42,0x4d,0x50}; // "STARTBMP"
char autoset_data[AUTOSET_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x41,0x55,0x54,0x23};
char self_cal_data[SELF_CAL_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x43,0x52,0x53,0x23};
char factory_reset_data[FACTORY_RESET_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x44,0x46,0x54,0x23};
char force_trig_data[FORCE_TRIG_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x46,0x4f,0x52,0x23};
char set_50pct_trig_data[SET_50PCT_TRIG_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x46,0x35,0x30,0x23};
char set_0_trig_data[SET_0_TRIG_DATA_SIZE] = {0x3a,0x53,0x44,0x53,0x4c,0x54,0x4c,0x30,0x23};

char coupling_data[COUPLING_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x06,0x4d,0x43,0x48,0x00,0x63,0x00};
/* Byte[9] ---------- 0x00 = CH1 | 0x01 = CH2 */
/* Byte[11] --------- 0x00 = DC | 0x01 = AC | 0x02 = GND */

char probe_scale_data[PROBE_SCALE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x06,0x4d,0x43,0x48,0x00,0x70,0x00};
/* Byte[9] ---------- 0x00 = CH1 | 0x01 = CH2 */
/* Byte[11] --------- 0x00 = x1 | 0x01 = x10 | 0x02 = x100 | 0x03 = 0x1000 */

char volt_range_data[VOLT_SCALE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x06,0x4d,0x43,0x48,0x00,0x76,0x00};
/* Byte[9] ---------- 0x00 = CH1 | 0x01 = CH2 */
/* Byte[11] --------- 0x00 = 2mV | 0x01 = 5mV | 0x02 = 10mV | 0x03 = 20mV
                      0x04 = 50mV | 0x05 = 100mv | 0x06 = 200mV | 0x07 = 500mv
                      0x08 = 1v | 0x09 = 2v | 0x0A = 5v | 0x0B = 10v */

char memory_range_data[MEM_RANGE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x04,0x4d,0x44,0x50,0x00};
/* Byte[9] ---------- 0x00 = 1k | 0x01 = 10k | 0x02 = 100k | 0x03 = 1M | 0x04 = 10M */

char timebase_data[TB_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x05,0x4d,0x48,0x52,0x62,0x00};
/* Byte[10] -------- 0x00 = 2nS | 0x01 = 5nS | 0x02 = 10nS | 0x03 = 20nS | 0x04 = 50nS
                     0x05 = 100nS | 0x06 = 200nS | 0x07 = 500nS | 0x08 = 1uS | 0x09 = 2uS
                     0x0A = 5uS | 0x0B = 10uS | 0x0C = 20uS | 0x0D = 50uS | 0x0E = 100uS
                     0x0F = 200uS | 0x10 = 500uS | 0x11 = 1mS | 0x12 = 2mS | 0x13 = 5mS
                     0x14 = 10mS | 0x15 = 20mS | 0x16 = 50mS | 0x17 = 100mS | 0x18 = 200mS
                     0x19 = 500mS | 0x1A = 1S | 0x1B = 2S | 0x1C = 5S | 0x1D = 10S | 0x1E = 20S
                     0x1F = 50S | 0x20 = 100S */

char tracepos_data[TRACEPOS_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x09,0x4d,0x43,0x48,0x00,0x7a,0x00,0x00,0x00,0x00};
/* Byte[11] & Byte[12] & Byte[13] & Byte[14] = 32bit signed value. Byte[14] = LSB (Note: Value is div by 25 ) */

char hor_trigger_data[HOR_TRIG_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x08,0x4d,0x48,0x52,0x76,0x00,0x00,0x00,0x00};
/* Byte[10] & Byte[11] & Byte[12] & Byte[13] = 32bit signed value. Byte[13] = LSB (Note: Value is div by 50 ) */

char acqu_mode_data[ACQU_MODE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x04,0x4d,0x41,0x51,0x00};
/* Byte[9] --------- 0x00 = Sample | 0x01 = Peak */

char acqu_avg_mode_data[ACQU_AVG_MODE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x05,0x4d,0x41,0x51,0x02,0x00};
/* Byte[10] -------- 0x00 = 4Samples | 0x01 = 16samples | 0x02 = Samples64 | 0x03 = 128Samples */

char edge_trigger_data[EDGE_TRIG_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x2e,0x4d,0x54,0x52,0x73,0x00,0x65,0x02,0x00,0x4d,
                                               0x54,0x52,0x73,0x00,0x65,0x03,0x00,0x4d,0x54,0x52,0x73,0x00,0x65,0x04,0x00,
                                               0x00,0x00,0x01,0x4d,0x54,0x52,0x73,0x00,0x65,0x05,0x00,0x4d,0x54,0x52,0x73,
                                               0x00,0x65,0x06,0x00,0x00,0x00,0x00};
/*  byte[9] & byte[17] & byte[25] & byte[36] & byte[44] --------- 0x73 = single | 0x61 = alternate
    byte[10] & byte[18] & byte[26] & byte[37] & byte[45] -------- 0x00 = CH1 | 0x01 = CH2
    byte[40] ------------  0x00 = Rising edge | 0x01 = Falling Edge
    byte[13] ------------  0x00 = dc couple | 0x01 = ac couple | 0x02 = hf couple | 0x03 = lf couple
    byte[21] ------------  0x00 = auto trigger | 0x01 = normal trigger | 0x02 = once trigger
    byte[48] & byte[49] & byte[50] & byte[51] -------- Trigger value 32bit signed. Byte[51] = LSB */

char video_trigger_data[VID_TRIG_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x1b,0x4d,0x54,0x52,0x73,0x00,0x76,0x02,0x00,0x4d,
                                               0x54,0x52,0x73,0x00,0x76,0x03,0x00,0x4d,0x54,0x52,0x73,0x00,0x76,0x04,0x00,
                                               0x00,0x00,0x01};
/*  byte[10] & byte[18] & byte[26] ---------- 0x00 = CH1 | 0x01 CH2
    byte[13] ---------- 0x00 = NTSC | 0x01 = PAL | 0x02 = SECAM
    byte[21] ---------- 0x00 = LINE | 0x01 = FIELD | 0x02 = ODD | 0x03 = EVEN */

char video_trig_line_no_data[VID_TRIG_LINE_DATA_SIZE] = {0x3a,0x4d,0x00,0x00,0x00,0x1f,0x4d,0x54,0x52,0x73,0x01,0x76,0x02,
                                                         0x02,0x4d,0x54,0x52,0x73,0x01,0x76,0x03,0x04,0x00,0x00,0x00,0x01,
                                                         0x4d,0x54,0x52,0x73,0x01,0x76,0x04,0x00,0x00,0x00,0x01};
/*  byte[10] & byte[18] & byte[30] ---------- 0x00 = CH1 | 0x01 CH2
    byte[13] ---------- 0x00 = NTSC | 0x01 = PAL | 0x02 SECAM
    byte[22] & byte[23] & byte[24] & byte[25] = (value between 1 and 625) (note: 525 max ntsc mode) Byte[25] = LSB */

/********************/
/* Global Variables */
/********************/

libusb_device_handle *dev_handle;
libusb_context *lusb_ctx = NULL;

char * bmp_buffer;

/********************/
/* Code Starts Here */
/********************/
int connect_usb(void)
{
    libusb_device **devs;
    int ret = 0;
    int devcount = 0;

    /* Initialise the usb interface */
    ret = libusb_init(&lusb_ctx);

    /* Check to see if the lusb_init call failed */
    if ( ret < 0 )
    {
        window_ptr->append_log("USB init failed!");
        return 1;
    }

    libusb_set_debug(lusb_ctx, 3); //set verbosity level to 3, as suggested in the documentation

    /* Get device list */
    devcount = libusb_get_device_list(lusb_ctx, &devs);

    if(devcount < 0)
    {
        /* error getting a device list */
        window_ptr->append_log("Get Device Error");
    }

    //w.append_log("Num Devices Found :  %d", devcount);

    dev_handle = libusb_open_device_with_vid_pid(lusb_ctx, USB_LOCK_VENDOR, USB_LOCK_PRODUCT);
    if(dev_handle == NULL)
    {
        window_ptr->append_log("Cannot open OWON device");
        return 1;
    }

    /* Free the device list */
    libusb_free_device_list(devs, 1);

    if(libusb_kernel_driver_active(dev_handle, 0) == 1)
    {
        window_ptr->append_log("Kernel Driver Active - Requesting Detach");

        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
        {
            window_ptr->append_log("Driver Detached");
        }
    }

    ret = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(ret < 0)
    {
        window_ptr->append_log("Failed to claim interface. Exiting");
        return 1;
    }

    window_ptr->append_log("Claimed USB Interface");

    return(0);

}

int disconnect_usb(void)
{
    int ret;

    if(dev_handle == NULL)
    {
        window_ptr->append_log("No Interface Claimed");
        return(1);
    }

    ret = libusb_release_interface(dev_handle, 0); //release the claimed interface
    if(ret != 0)
    {
        window_ptr->append_log("Cannot Release Interface");
        return 1;
    }
    window_ptr->append_log("Released Interface");

    libusb_close(dev_handle);

    /* Close the connection */
    libusb_exit(lusb_ctx);

    if(NULL != bmp_buffer)
    {
        free(bmp_buffer);
        bmp_buffer = nullptr;
    }

    return(0);
}

int send_usb_data(char* data, int length)
{
    int local_ret;
    int local_written;

    if(dev_handle != 0)
    {
        local_ret = libusb_bulk_transfer(dev_handle, BULK_WRITE_ENDPOINT, (unsigned char*)data, length, &local_written, 0);

        if( local_ret != 0 )
        {
            window_ptr->append_log ("Write Failed");
            return (1);
        }
    }
    else
    {
        window_ptr->append_log ("Failed to send command, Is the USB connetion active?");
        return(1);
    }

    return(0);
}

int get_usb_data(char* data, int length, int * transferred)
{
    int local_ret = 0;

    if(dev_handle != 0)
    {
        local_ret = libusb_bulk_transfer(dev_handle, BULK_READ_ENDPOINT, (unsigned char*)data, length, transferred, 0);
        if( local_ret != 0 ) {window_ptr->append_log ("Read Failed");}
    }
    else
    {
        window_ptr->append_log ("Failed to read command, Is the USB connetion active?");
    }

    return local_ret;
}

int get_bmp_usb(void)
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
    send_usb_data(start_bmp, sizeof(start_bmp));

    /* Get the response */
    get_usb_data(&tempdata_u.data[0], sizeof(tempdata_u), &transferred);

    /* Clear previous buffer if exists */
    if(NULL != bmp_buffer)
    {
        free(bmp_buffer);
        bmp_buffer = nullptr;
    }

    /* Allocate memory to receive bmp */
    bmp_buffer = (char*) malloc(tempdata_u.response.length);
    allocated = tempdata_u.response.length;

    if(NULL == bmp_buffer)
    {
        window_ptr->append_log ("Failed to allocate Buffer!");
        return -1;
    }

    /* Get the data from the oscilloscope */
    do
    {
        get_usb_data(bmp_buffer + downloaded, 131072, &transferred);
        downloaded += transferred;

    } while (allocated > downloaded);

    //printf("Downloaded: %d\n",downloaded);

    return 0;

}
