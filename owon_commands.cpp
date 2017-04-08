#include <stdio.h>
#include <libusb-1.0/libusb.h>
#include "usb_interface.h"
#include "owon_commands.h"
#include "owoncontrol.h"
#include "consetting.h"
#include "main.h"
#include "connection.h"

channel_st current_ch1_vars;
channel_st current_ch2_vars;
tbase_e current_timebase_var;
memory_e current_memory_var;
int horiz_pos;
acqu_e acqu_mode;
samples_e acqu_samples;
trigger_st trig_vars;

void set_coupling(channel_e chan)
{
    int value = 0;

    if (chan < NUM_CHANNEL_ENTRIES)
    {
        if(chan == CHANNEL_1)
        {
            value = current_ch1_vars.ch_coupling;
            window_ptr->append_log("Updating CH1 Coupling");
        }
        else
        {
            value = current_ch2_vars.ch_coupling;
            window_ptr->append_log("Updating CH2 Coupling");
        }

        if(value < NUM_CPL_ENTRIES)
        {
            coupling_data[9] = chan;
            coupling_data[11] = value;
            scope_send_message(coupling_data, sizeof(coupling_data));
        }
        else
            window_ptr->append_log("Value out of range!");
    }
    else
    {
        window_ptr->append_log("Channel out of range!");
    }
}

void set_probescale(channel_e chan)
{
    int value = 0;

    if (chan < NUM_CHANNEL_ENTRIES)
    {
        if(chan == CHANNEL_1)
        {
            value = current_ch1_vars.ch_prb_scale;
            window_ptr->append_log("Updating CH1 Probe Scale");
        }
        else
        {
            value = current_ch2_vars.ch_prb_scale;
            window_ptr->append_log("Updating CH2 Probe Scale");
        }

        if(value < NUM_PRB_ENTRIES)
        {
            probe_scale_data[9] = chan;
            probe_scale_data[11] = value;
            scope_send_message(probe_scale_data, sizeof(probe_scale_data));
        }
        else
        {
            window_ptr->append_log("Value out of range!");
        }
    }
    else
    {
        window_ptr->append_log("Channel out of range!");
    }
}

void set_voltscale(channel_e chan)
{
    int value = 0;

    if (chan < NUM_CHANNEL_ENTRIES)
    {
        if(chan == CHANNEL_1)
        {
            value = current_ch1_vars.ch_volt_range;
            window_ptr->append_log("Updating CH1 Volt Range");
        }
        else
        {
            value = current_ch2_vars.ch_volt_range;
            window_ptr->append_log("Updating CH2 Volt Range");
        }

        if(value < NUM_VOLT_ENTRIES)
        {
            volt_range_data[9] = chan;
            volt_range_data[11] = value;
            scope_send_message(volt_range_data, sizeof(volt_range_data));
        }
        else
        {
            window_ptr->append_log("Value out of range!");
        }
    }
    else
    {
        window_ptr->append_log("Channel out of range!");
    }
}

void set_tracepos(channel_e chan)
{
    int value = 0;

    if (chan < NUM_CHANNEL_ENTRIES)
    {
        if(chan == CHANNEL_1)
        {
            value = current_ch1_vars.tracepos;
            window_ptr->append_log("Updating CH1 Trace Position");
        }
        else
        {
            value = current_ch2_vars.tracepos;
            window_ptr->append_log("Updating CH2 Trace Position");
        }

        if( (value >= -250) && (value <= 250) )
        {
            tracepos_data[9] = chan;
            tracepos_data[11] = (char) ((value >> 24) & 0x000000FF);
            tracepos_data[12] = (char) ((value >> 16) & 0x000000FF);
            tracepos_data[13] = (char) ((value >> 8) & 0x000000FF);
            tracepos_data[14] = (char) ((value) & 0x000000FF);
            scope_send_message(tracepos_data, sizeof(tracepos_data));
        }
        else
        {
            window_ptr->append_log("Value out of range!");
        }
    }
    else
    {
        window_ptr->append_log("Channel out of range!");
    }
}

void set_memrange(void)
{
    int value = 0;

    value = current_memory_var;

    if(value < NUM_MEM_ENTRIES)
    {
        memory_range_data[9] = value;
        window_ptr->append_log("Updating Memory Depth");
        scope_send_message(memory_range_data, sizeof(memory_range_data));
    }
    else
    {
        window_ptr->append_log("Value out of range!");
    }
}

void set_timebase(void)
{
    int value = 0;

    value = current_timebase_var;

    if(value < NUM_TBASE_ENTRIES)
    {
        timebase_data[10] = value;
        window_ptr->append_log("Updating Timebase");
        scope_send_message(timebase_data, sizeof(timebase_data));
    }
    else
    {
        window_ptr->append_log("Value out of range!");
    }
}

void force_trigger(void)
{
    window_ptr->append_log("Send Force Trigger Command");
    scope_send_message(force_trig_data, sizeof(force_trig_data));
}

void set_50pct_trigger(void)
{
    window_ptr->append_log("Send 50pct Trigger Command");
    scope_send_message(set_50pct_trig_data, sizeof(set_50pct_trig_data));
}

void set_0_trigger(void)
{
    window_ptr->append_log("Send 0 Trigger Command");
    scope_send_message(set_0_trig_data, sizeof(set_0_trig_data));
}

void set_horiz_trigger_pos(void)
{
    int value = 0;

    value = horiz_pos;

    if( (value >= -10000) && (value <= 10000) )
    {
        hor_trigger_data[10] = (char) ((value >> 24) & 0x000000FF);
        hor_trigger_data[11] = (char) ((value >> 16) & 0x000000FF);
        hor_trigger_data[12] = (char) ((value >> 8) & 0x000000FF);
        hor_trigger_data[13] = (char) (value & 0x000000FF);

        window_ptr->append_log("Send Horiz Pos Command");
        scope_send_message(hor_trigger_data, sizeof(hor_trigger_data));
    }
    else
    {
        window_ptr->append_log("Value out of range!");
    }
}

void set_acqu_mode(void)
{
    acqu_e mode = acqu_mode;

    if(mode < NUM_ACQU_ENTRIES)
    {
        acqu_mode_data[9] = mode;

        window_ptr->append_log("Send Acquisition Mode Command");
        scope_send_message(acqu_mode_data, sizeof(acqu_mode_data));
    }
}

void set_avg_acqu_mode(void)
{
    samples_e value = acqu_samples;

    if(value < NUM_SAMPLE_ENTRIES)
    {
        acqu_avg_mode_data[10] = value;

        window_ptr->append_log("Send Avg Acquisition Mode Command");
        scope_send_message(acqu_avg_mode_data, sizeof(acqu_avg_mode_data));
    }
    else
    {
        window_ptr->append_log("Value out of range");
    }
}


void set_edge_or_alt_trigger(void)
{

    if(trig_vars.chan < NUM_CHANNEL_ENTRIES)
    {
            edge_trigger_data[10] = trig_vars.chan;
            edge_trigger_data[18] = trig_vars.chan;
            edge_trigger_data[26] = trig_vars.chan;
            edge_trigger_data[37] = trig_vars.chan;
            edge_trigger_data[45] = trig_vars.chan;
    }
    else
    {
            window_ptr->append_log("Out of range");
            return;
    }

    if(trig_vars.type < NUM_TRIG_ENTRIES)
    {

        switch(trig_vars.type)
        {
            case TRIG_TYPE_SINGLE:
                edge_trigger_data[9] = 0x73;
                edge_trigger_data[17] = 0x73;
                edge_trigger_data[25] = 0x73;
                edge_trigger_data[36] = 0x73;
                edge_trigger_data[44] = 0x73;
                break;
            case TRIG_TYPE_ALT:
                edge_trigger_data[9] = 0x61;
                edge_trigger_data[17] = 0x61;
                edge_trigger_data[25] = 0x61;
                edge_trigger_data[36] = 0x61;
                edge_trigger_data[44] = 0x61;
                break;
            default:
                window_ptr->append_log("Out of range");
                return;
                break;
        }
    }
    else
    {
        window_ptr->append_log("Value out of range");
        return;
    }

    if(trig_vars.edge < NUM_EDGE_ENTRIES)
    {
        edge_trigger_data[40] = trig_vars.edge;
    }
    else
    {
        window_ptr->append_log("Out of range");
        return;
    }


    if(trig_vars.coupling < NUM_TRIG_CPL_ENTRIES)
    {
        edge_trigger_data[13] = trig_vars.coupling;
    }
    else
    {
        window_ptr->append_log("Out of range");
        return;
    }

    if(trig_vars.trig_mode < NUM_TRIG_MODE_ENTRIES)
    {
        edge_trigger_data[21] = trig_vars.trig_mode;
    }
    else
    {
        window_ptr->append_log("Out of range");
        return;
    }

    if( (trig_vars.trig_voltage >= -10000) && (trig_vars.trig_voltage <= 10000) )
    {
        edge_trigger_data[48] = (char) ((trig_vars.trig_voltage >> 24) & 0x000000FF);
        edge_trigger_data[49] = (char) ((trig_vars.trig_voltage >> 16) & 0x000000FF);
        edge_trigger_data[50] = (char) ((trig_vars.trig_voltage >> 8) & 0x000000FF);
        edge_trigger_data[51] = (char) (trig_vars.trig_voltage & 0x000000FF);

        window_ptr->append_log("Send Trigger Opts Command");
        scope_send_message(edge_trigger_data, sizeof(edge_trigger_data));
    }
    else
    {
        window_ptr->append_log("Value out of range!");
        return;
    }
}

void set_video_trigger(void)
{

    if(trig_vars.chan < NUM_CHANNEL_ENTRIES)
    {
        /* We dont know if we will use line mode or not. Best to update
           both messaages at the same time */

        video_trigger_data[10] = trig_vars.chan;
        video_trigger_data[18] = trig_vars.chan;
        video_trigger_data[26] = trig_vars.chan;

        /* Line mode settings */
        video_trig_line_no_data[10] = trig_vars.chan;
        video_trig_line_no_data[18] = trig_vars.chan;
        video_trig_line_no_data[30] = trig_vars.chan;
    }
    else
    {
            window_ptr->append_log("Out of range");
            return;
    }

    if(trig_vars.modulation < NUM_MODU_ENTRIES)
    {
        /* We dont know if we will use line mode or not. Best to update
           both messaages at the same time */
        video_trigger_data[13] = trig_vars.modulation;

        /* Line mode settings */
        video_trig_line_no_data[13] = trig_vars.modulation;
    }
    else
    {
            window_ptr->append_log("Out of range");
            return;
    }

    if(trig_vars.sync < NUM_SYNC_ENTRIES)
    {
        if(trig_vars.sync != SYNC_LINE_NO)
        {
            video_trigger_data[21] = trig_vars.sync;
            window_ptr->append_log("Send Video Trigger Command");
            scope_send_message(video_trigger_data, sizeof(video_trigger_data));
        }
        /*Line Number Mode. More options to set before send */
        else
        {
            if(video_trig_line_no_data[13] == MODU_NTSC)
            {
                if( (trig_vars.line < 0) || (trig_vars.line > 525) )
                {
                    /* Value out of range. Force Line 1 */
                    window_ptr->append_log("Out of range");
                    return;
                }
            }
            else
            {
                if( (trig_vars.line < 0) || (trig_vars.line > 625) )
                {
                    /* Value out of range. Force Line 1 */
                    window_ptr->append_log("Out of range");
                    return;
                }
            }

            /* Set the value before send */
            video_trig_line_no_data[22] = (char) ((trig_vars.line >> 24) & 0x000000FF);
            video_trig_line_no_data[23] = (char) ((trig_vars.line >> 16) & 0x000000FF);
            video_trig_line_no_data[24] = (char) ((trig_vars.line >> 8) & 0x000000FF);
            video_trig_line_no_data[25] = (char) (trig_vars.line & 0x000000FF);

            window_ptr->append_log("Send Video LineNo Command");
            scope_send_message(video_trig_line_no_data, sizeof(video_trig_line_no_data));
        }
    }
    else
    {
        window_ptr->append_log("Out of range");
        return;
    }
}

void send_autoset_command(void)
{
    window_ptr->append_log("Send Autoset Command");
    scope_send_message(autoset_data, sizeof(autoset_data));
}


void send_self_cal_command(void)
{
    window_ptr->append_log("Send Self Cal Command");
    scope_send_message(self_cal_data, sizeof(self_cal_data));
}

void send_factory_reset_command(void)
{
    window_ptr->append_log("Send Factory Reset Command");
    scope_send_message(factory_reset_data, sizeof(factory_reset_data));
}
