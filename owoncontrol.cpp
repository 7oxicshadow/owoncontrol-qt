#include "owoncontrol.h"
#include "ui_owoncontrol.h"
#include <libusb-1.0/libusb.h>
#include "usb_interface.h"
#include "lan_interface.h"
#include "owon_commands.h"
#include <QGraphicsScene>
#include <QFileDialog>
#include "consetting.h"
#include "main.h"
#include <QImage>
#include "connection.h"

owoncontrol::owoncontrol(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::owoncontrol)
{
    ui->setupUi(this);
}

owoncontrol::~owoncontrol()
{
    delete ui;
}

void owoncontrol::init_vars()
{
    current_ch1_vars.enabled = ui->ch1_enable_check->isChecked();
    current_ch1_vars.ch_coupling = (coupling_e) ui->ch1_coupling_combo->currentIndex();
    current_ch1_vars.ch_prb_scale = (prb_scale_e) ui->ch1_probe_combo->currentIndex();
    current_ch1_vars.ch_volt_range = (volt_range_e) ui->ch1_scale_combo->currentIndex();
    current_ch1_vars.tracepos = ui->ch1_position_spin->value();

    current_ch2_vars.enabled = ui->ch2_enable_check->isChecked();
    current_ch2_vars.ch_coupling = (coupling_e) ui->ch2_coupling_combo->currentIndex();
    current_ch2_vars.ch_prb_scale = (prb_scale_e) ui->ch2_probe_combo->currentIndex();
    current_ch2_vars.ch_volt_range = (volt_range_e) ui->ch2_scale_combo->currentIndex();
    current_ch2_vars.tracepos = ui->ch2_position_spin->value();

    current_memory_var = (memory_e) ui->deep_mem_combo->currentIndex();
    current_timebase_var = (tbase_e) ui->timebase_combo->currentIndex();
    horiz_pos = ui->hor_trig_spin->value();

    acqu_mode = (acqu_e) ui->sample_mode_combo->currentIndex();
    acqu_samples = (samples_e) ui->samples_combo->currentIndex();

    if( ui->trigger_ch1_radio->isChecked() == true)
        trig_vars.chan = CHANNEL_1;
    else
        trig_vars.chan = CHANNEL_2;

    if( ui->trigger_single_radio->isChecked() == true)
        trig_vars.type = TRIG_TYPE_SINGLE;
    else
        trig_vars.type = TRIG_TYPE_ALT;

    if( ui->trigger_edge_radio->isChecked() == true)
        trig_vars.master_mode = TRIG_EDGE;
    else
        trig_vars.master_mode = TRIG_VIDEO;

    trig_vars.edge = (trig_edge_e) ui->trigger_edge_combo->currentIndex();
    trig_vars.coupling = (trig_cpl_e) ui->trigger_edge_coupling_combo->currentIndex();
    trig_vars.trig_mode = (trig_mode_e) ui->trigger_edge_trigmode_combo->currentIndex();
    trig_vars.trig_voltage = ui->trigger_edge_trigvoltage_spin->value();
    trig_vars.modulation = (video_modu_e) ui->trigger_video_mod_combo->currentIndex();
    trig_vars.sync = (video_sync_e) ui->trigger_video_sync_combo->currentIndex();
    trig_vars.line = ui->trigger_video_line_spin->value();

}

void owoncontrol::append_log(const QString &text)
{
    ui->textEdit->append(text);
}

void owoncontrol::on_connectButton_clicked()
{
    int status;

    status = scope_connect();

    if(status != 0)
    {
        ui->textEdit->append("Connect reported a fail condition.");
    }
    else
    {
        ui->tabWidget->setEnabled(true);
        ui->actionAutoset->setEnabled(true);
        ui->actionFactory_Reset->setEnabled(true);
        ui->actionSelf_Cal->setEnabled(true);
        ui->forceupdate_btn->setEnabled(true);
        ui->actionScreenshot->setEnabled(true);
        ui->capture_btn->setEnabled(true);

        ui->trigger_edge_combo->setEnabled(true);
        ui->trigger_edge_coupling_combo->setEnabled(true);
        ui->trigger_edge_trigmode_combo->setEnabled(true);
        ui->trigger_edge_trigvoltage_spin->setEnabled(true);

        ui->trigger_video_sync_combo->setEnabled(false);
        ui->trigger_video_mod_combo->setEnabled(false);
        ui->trigger_video_line_spin->setEnabled(false);

        ui->connectButton->setEnabled(false);
        ui->actionConnect->setEnabled(false);
        ui->disconnectButton->setEnabled(true);
        ui->actionDisconnect->setEnabled(true);
        ui->info_label->hide();

        owoncontrol::on_capture_btn_clicked();

        ui->actionConnection_Settings->setEnabled(false);
        cest_frm_ptr->close();
    }
}

void owoncontrol::on_disconnectButton_clicked()
{
    int status;

    status = scope_disconnect();

    if(status == 0)
    {
        ui->tabWidget->setEnabled(false);
        ui->actionAutoset->setEnabled(false);
        ui->actionFactory_Reset->setEnabled(false);
        ui->actionSelf_Cal->setEnabled(false);
        ui->forceupdate_btn->setEnabled(false);
        ui->actionScreenshot->setEnabled(false);
        ui->capture_btn->setEnabled(false);

        ui->connectButton->setEnabled(true);
        ui->actionConnect->setEnabled(true);
        ui->disconnectButton->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);

        ui->actionConnection_Settings->setEnabled(true);
    }
}

void owoncontrol::on_actionConnect_triggered()
{
    on_connectButton_clicked();
}

void owoncontrol::on_actionClose_triggered()
{
    int status;

    status = scope_disconnect();

    if(status == 0)
    {
        ui->tabWidget->setEnabled(false);
        ui->actionAutoset->setEnabled(false);
        ui->actionFactory_Reset->setEnabled(false);
        ui->actionSelf_Cal->setEnabled(false);
        ui->forceupdate_btn->setEnabled(false);
    }

    QCoreApplication::quit();
}

void owoncontrol::on_actionDisconnect_triggered()
{
    on_disconnectButton_clicked();
}

void owoncontrol::on_pushButton_2_clicked()
{
    force_trigger();
}

void owoncontrol::on_pushButton_3_clicked()
{
    set_50pct_trigger();
}

void owoncontrol::on_pushButton_4_clicked()
{
    set_0_trigger();
}

void owoncontrol::on_actionAutoset_triggered()
{
    send_autoset_command();
}

void owoncontrol::on_actionSelf_Cal_triggered()
{
    send_self_cal_command();
}

void owoncontrol::on_actionFactory_Reset_triggered()
{
    send_factory_reset_command();
}

void owoncontrol::on_ch1_coupling_combo_currentIndexChanged(int index)
{
    current_ch1_vars.ch_coupling = (coupling_e) index;
    set_coupling(CHANNEL_1);
}

void owoncontrol::on_ch1_probe_combo_currentIndexChanged(int index)
{
    current_ch1_vars.ch_prb_scale = (prb_scale_e) index;
    set_probescale(CHANNEL_1);
}

void owoncontrol::on_ch1_scale_combo_currentIndexChanged(int index)
{
    current_ch1_vars.ch_volt_range = (volt_range_e) index;
    set_voltscale(CHANNEL_1);
}

void owoncontrol::on_ch2_coupling_combo_currentIndexChanged(int index)
{
    current_ch2_vars.ch_coupling = (coupling_e) index;
    set_coupling(CHANNEL_2);
}

void owoncontrol::on_ch2_probe_combo_currentIndexChanged(int index)
{
    current_ch2_vars.ch_prb_scale = (prb_scale_e) index;
    set_probescale(CHANNEL_2);
}

void owoncontrol::on_ch2_scale_combo_currentIndexChanged(int index)
{
    current_ch2_vars.ch_volt_range = (volt_range_e) index;
    set_voltscale(CHANNEL_2);
}

void owoncontrol::on_deep_mem_combo_currentIndexChanged(int index)
{
    current_memory_var = (memory_e) index;
    set_memrange();
}

void owoncontrol::on_timebase_combo_currentIndexChanged(int index)
{
    current_timebase_var = (tbase_e) index;
    set_timebase();
}

void owoncontrol::on_sample_mode_combo_currentIndexChanged(int index)
{
    acqu_mode = (acqu_e) index;
    set_acqu_mode();
}

void owoncontrol::on_samples_combo_currentIndexChanged(int index)
{
    acqu_samples = (samples_e) index;
    set_avg_acqu_mode();
}

void owoncontrol::on_trigger_edge_combo_currentIndexChanged(int index)
{
    trig_vars.edge = (trig_edge_e) index;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_edge_coupling_combo_currentIndexChanged(int index)
{
    trig_vars.coupling = (trig_cpl_e) index;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_edge_trigmode_combo_currentIndexChanged(int index)
{
    trig_vars.trig_mode = (trig_mode_e) index;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_video_mod_combo_currentIndexChanged(int index)
{
    trig_vars.modulation = (video_modu_e) index;

    if(trig_vars.modulation == MODU_NTSC)
    {
        if( ui->trigger_video_line_spin->value() > 525 )
            trig_vars.line = 525;

        ui->trigger_video_line_spin->setMaximum(525);
    }
    else
    {
        ui->trigger_video_line_spin->setMaximum(625);
    }

    set_video_trigger();
}

void owoncontrol::on_trigger_video_sync_combo_currentIndexChanged(int index)
{
    trig_vars.sync = (video_sync_e) index;

    if(trig_vars.sync == SYNC_LINE_NO)
        ui->trigger_video_line_spin->setEnabled(true);
    else
        ui->trigger_video_line_spin->setEnabled(false);

    set_video_trigger();
}

void owoncontrol::on_trigger_single_radio_clicked()
{
    trig_vars.type = TRIG_TYPE_SINGLE;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_alternate_radio_clicked()
{
    trig_vars.type = TRIG_TYPE_ALT;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_ch1_radio_clicked()
{
    trig_vars.chan = CHANNEL_1;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_ch2_radio_clicked()
{
    trig_vars.chan = CHANNEL_2;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_edge_radio_clicked()
{
    ui->trigger_edge_combo->setEnabled(true);
    ui->trigger_edge_coupling_combo->setEnabled(true);
    ui->trigger_edge_trigmode_combo->setEnabled(true);
    ui->trigger_edge_trigvoltage_spin->setEnabled(true);

    ui->trigger_video_sync_combo->setEnabled(false);
    ui->trigger_video_mod_combo->setEnabled(false);
    ui->trigger_video_line_spin->setEnabled(false);

    ui->tabWidget_3->setCurrentIndex(0);

    trig_vars.master_mode = TRIG_EDGE;
    set_edge_or_alt_trigger();
}

void owoncontrol::on_trigger_video_radio_clicked()
{
    ui->trigger_edge_combo->setEnabled(false);
    ui->trigger_edge_coupling_combo->setEnabled(false);
    ui->trigger_edge_trigmode_combo->setEnabled(false);
    ui->trigger_edge_trigvoltage_spin->setEnabled(false);

    ui->trigger_video_sync_combo->setEnabled(true);
    ui->trigger_video_mod_combo->setEnabled(true);

    if(trig_vars.sync == SYNC_LINE_NO)
        ui->trigger_video_line_spin->setEnabled(true);
    else
        ui->trigger_video_line_spin->setEnabled(false);

    ui->tabWidget_3->setCurrentIndex(1);

    trig_vars.master_mode = TRIG_VIDEO;
    set_video_trigger();
}

void owoncontrol::on_trigger_edge_trigvoltage_spin_editingFinished()
{
    trig_vars.trig_voltage = ui->trigger_edge_trigvoltage_spin->value();
    set_edge_or_alt_trigger();
}

void owoncontrol::on_ch1_position_spin_editingFinished()
{
    current_ch1_vars.tracepos = ui->ch1_position_spin->value();
    set_tracepos(CHANNEL_1);
}

void owoncontrol::on_ch2_position_spin_editingFinished()
{
    current_ch2_vars.tracepos = ui->ch2_position_spin->value();
    set_tracepos(CHANNEL_2);
}

void owoncontrol::on_hor_trig_spin_editingFinished()
{
    horiz_pos = ui->hor_trig_spin->value();
    set_horiz_trigger_pos();
}

void owoncontrol::on_trigger_video_line_spin_editingFinished()
{
    trig_vars.line = ui->trigger_video_line_spin->value();
    set_video_trigger();
}

void owoncontrol::on_actionScreenshot_triggered()
{
    FILE *fp;
    const char * name_ptr;
    QString fileName;

    if(NULL != bmp_buffer)
    {
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
                tr("BMP Files (*.bmp)"));

        if (!fileName.isEmpty())
        {
            name_ptr = qPrintable(fileName);

            fp = fopen(name_ptr, "wb");

            // Write data out
            fwrite(bmp_buffer, sizeof(char), 1440054, fp);

            fclose(fp);
        }
    }
}

void owoncontrol::on_capture_btn_clicked()
{
    static QPixmap pixmap;
    QGraphicsScene *scene = new QGraphicsScene (this);
    QImage temp_a, temp_b, image;

    if( 0 == scope_get_bmp() )
    {
        ui->textEdit->append("BMP read success");

        if(NULL != bmp_buffer)
        {
            /* Create image from buffer + header size */
            image = QImage((const uchar*)bmp_buffer + 54, 800, 600, QImage::Format_RGB888);
            temp_a = image.mirrored(false, true);
            temp_b = temp_a.rgbSwapped();
            pixmap = QPixmap::fromImage(temp_b);

            scene->addPixmap(pixmap);
            ui->graphicsView->setScene(scene);
            ui->graphicsView->show();
        }
    }
}

void owoncontrol::on_forceupdate_btn_clicked()
{
    set_coupling(CHANNEL_1);
    set_probescale(CHANNEL_1);
    set_voltscale(CHANNEL_1);
    set_coupling(CHANNEL_2);
    set_probescale(CHANNEL_2);
    set_voltscale(CHANNEL_2);
    set_memrange();
    set_timebase();
    set_acqu_mode();
    set_edge_or_alt_trigger();
    set_tracepos(CHANNEL_1);
    set_tracepos(CHANNEL_2);
}


void owoncontrol::on_actionConnection_Settings_triggered()
{
    cest_frm_ptr->show();
}
