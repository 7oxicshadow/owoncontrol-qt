#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

#define STARTBMP_SIZE            (8)
#define COUPLING_DATA_SIZE       (12)
#define PROBE_SCALE_DATA_SIZE    (12)
#define VOLT_SCALE_DATA_SIZE     (12)
#define MEM_RANGE_DATA_SIZE      (10)
#define TB_DATA_SIZE             (11)
#define TRACEPOS_DATA_SIZE       (15)
#define HOR_TRIG_DATA_SIZE       (14)
#define ACQU_MODE_DATA_SIZE      (10)
#define ACQU_AVG_MODE_DATA_SIZE  (11)
#define EDGE_TRIG_DATA_SIZE      (52)
#define FORCE_TRIG_DATA_SIZE     (9)
#define SET_50PCT_TRIG_DATA_SIZE (9)
#define SET_0_TRIG_DATA_SIZE     (9)
#define AUTOSET_DATA_SIZE        (9)
#define SELF_CAL_DATA_SIZE       (9)
#define FACTORY_RESET_DATA_SIZE  (9)
#define VID_TRIG_DATA_SIZE       (33)
#define VID_TRIG_LINE_DATA_SIZE  (37)

typedef struct
{
    unsigned int length;
    unsigned int unknown;
    unsigned int flag; // 0 for waveform, 1 for bitmap, 128 if multipart
}owon_start_response_st;

extern char autoset_data[AUTOSET_DATA_SIZE];
extern char self_cal_data[SELF_CAL_DATA_SIZE];
extern char factory_reset_data[FACTORY_RESET_DATA_SIZE];
extern char coupling_data[COUPLING_DATA_SIZE];
extern char probe_scale_data[PROBE_SCALE_DATA_SIZE];
extern char volt_range_data[VOLT_SCALE_DATA_SIZE];
extern char memory_range_data[MEM_RANGE_DATA_SIZE];
extern char timebase_data[TB_DATA_SIZE];
extern char tracepos_data[TRACEPOS_DATA_SIZE];
extern char hor_trigger_data[HOR_TRIG_DATA_SIZE];
extern char acqu_mode_data[ACQU_MODE_DATA_SIZE];
extern char acqu_avg_mode_data[ACQU_AVG_MODE_DATA_SIZE];
extern char edge_trigger_data[EDGE_TRIG_DATA_SIZE];
extern char force_trig_data[FORCE_TRIG_DATA_SIZE];
extern char set_50pct_trig_data[SET_50PCT_TRIG_DATA_SIZE];
extern char set_0_trig_data[SET_0_TRIG_DATA_SIZE];
extern char video_trigger_data[VID_TRIG_DATA_SIZE];
extern char video_trig_line_no_data[VID_TRIG_LINE_DATA_SIZE];

extern libusb_device_handle *dev_handle;
extern char * bmp_buffer;

int connect_usb(void);
int disconnect_usb(void);
void send_usb_data(char* data, int length);
int get_bmp(void);

#endif
