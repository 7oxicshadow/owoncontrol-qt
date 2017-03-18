#ifndef OWON_COMMANDS_H
#define OWON_COMMANDS_H

/***************************************************
            General typedefs
****************************************************/
typedef enum
{
    CHANNEL_1 = 0,
    CHANNEL_2,
    NUM_CHANNEL_ENTRIES
}channel_e;

typedef enum
{
    TBASE_2NS = 0,
    TBASE_5NS, TABSE_10NS, TABSE_20NS,
    TBASE_50NS, TBASE_100NS, TBASE_200NS,
    TBASE_500NS, TBASE_1US, TBASE_2US,
    TBASE_5US, TBASE_10US, TBASE_20US,
    TBASE_50US, TBASE_100US, TBASE_200US,
    TBASE_500US, TBASE_1MS, TBASE_2MS,
    TBASE_5MS, TBASE_10MS, TBASE_20MS,
    TBASE_50MS, TBASE_100MS, TBASE_200MS,
    TBASE_500MS, TBASE_1S, TBASE_2S,
    TBASE_5S, TBASE_10S, TBASE_20S,
    TBASE_50S, TBASE_100S,
    NUM_TBASE_ENTRIES
}tbase_e;

typedef enum
{
    ACQU_SAMPLE = 0,
    ACQU_PEAK,
    ACQU_AVG,
    NUM_ACQU_ENTRIES
}acqu_e;

typedef enum
{
    SAMPLES_4 = 0,
    SAMPLES_16,
    SAMPLES_64,
    SAMPLES_128,
    NUM_SAMPLE_ENTRIES
}samples_e;

typedef enum
{
    MEM_1K = 0,
    MEM_10K,
    MEM_100K,
    MEM_1M,
    MEM_10M,
    NUM_MEM_ENTRIES
}memory_e;

typedef enum
{
    TRIG_TYPE_SINGLE = 0,
    TRIG_TYPE_ALT,
    TRIG_TYPE_VIDEO,
    NUM_TRIG_ENTRIES
}trig_type_e;

typedef enum
{
    TRIG_EDGE_RISING = 0,
    TRIG_EDGE_FALLING,
    NUM_EDGE_ENTRIES
}trig_edge_e;

typedef enum
{
    TRIG_CPL_DC = 0,
    TRIG_CPL_AC,
    TRIG_CPL_HF,
    TRIG_CPL_LF,
    NUM_TRIG_CPL_ENTRIES
}trig_cpl_e;

typedef enum
{
    TRIG_MODE_AUTO = 0,
    TRIG_MODE_NORMAL,
    TRIG_MODE_ONCE,
    NUM_TRIG_MODE_ENTRIES
}trig_mode_e;

typedef enum
{
    MODU_NTSC = 0,
    MODU_PAL,
    MODU_SECAM,
    NUM_MODU_ENTRIES

}video_modu_e;

typedef enum
{
    SYNC_LINE = 0,
    SYNC_FIELD,
    SYNC_ODD,
    SYNC_EVEN,
    SYNC_LINE_NO,
    NUM_SYNC_ENTRIES
}video_sync_e;

typedef enum
{
    TRIG_EDGE,
    TRIG_VIDEO,
    NUM_MASTER_TRIG_ENTRIES
}master_trig_mode;

/***************************************************
            Common Typedefs
****************************************************/
typedef enum
{
    DC_CPL = 0,
    AC_CPL,
    GND_CPL,
    NUM_CPL_ENTRIES
}coupling_e;

typedef enum
{
    PRB_X1 = 0,
    PRB_X10,
    PRB_X100,
    PRB_X1000,
    NUM_PRB_ENTRIES
}prb_scale_e;

typedef enum
{
    VOLT_2MV = 0,
    VOLT_5MV,
    VOLT_10MV,
    VOLT_20MV,
    VOLT_50MV,
    VOLT_100MV,
    VOLT_200MV,
    VOLT_500MV,
    VOLT_1V,
    VOLT_2V,
    VOLT_5V,
    VOLT_10V,
    NUM_VOLT_ENTRIES
}volt_range_e;

/***************************************************
            Struct Typedefs
****************************************************/

typedef struct
{
    int           enabled;
    coupling_e    ch_coupling;
    prb_scale_e   ch_prb_scale;
    volt_range_e  ch_volt_range;
    int           tracepos;
}channel_st;

typedef struct
{
    trig_type_e type;
    channel_e chan;
    master_trig_mode master_mode;
    trig_edge_e edge;
    trig_cpl_e coupling;
    trig_mode_e trig_mode;
    int trig_voltage;
    video_modu_e modulation;
    video_sync_e sync;
    int line;
}trigger_st;

extern channel_st current_ch1_vars;
extern channel_st current_ch2_vars;
extern tbase_e current_timebase_var;
extern memory_e current_memory_var;
extern int horiz_pos;

extern acqu_e acqu_mode;
extern samples_e acqu_samples;

extern trigger_st trig_vars;

void set_coupling(channel_e chan);
void set_probescale(channel_e chan);
void set_voltscale(channel_e chan);
void set_tracepos(channel_e chan);
void set_memrange(void);
void set_timebase(void);
void force_trigger(void);
void set_50pct_trigger(void);
void set_0_trigger(void);
void set_horiz_trigger_pos(void);
void set_acqu_mode(void);
void set_avg_acqu_mode(void);
void set_edge_or_alt_trigger(void);
void check_changes(void);
void send_autoset_command(void);
void send_self_cal_command(void);
void send_factory_reset_command(void);
void select_trigger_type(void);
void set_video_trigger(void);

#endif
