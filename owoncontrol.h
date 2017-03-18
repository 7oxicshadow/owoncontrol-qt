#ifndef OWONCONTROL_H
#define OWONCONTROL_H

#include <QMainWindow>
#include <QComboBox>

namespace Ui {
class owoncontrol;
}

class owoncontrol : public QMainWindow
{
    Q_OBJECT

public:
    explicit owoncontrol(QWidget *parent = 0);
    ~owoncontrol();

    void init_vars();
    void append_log(const QString &text);

private slots:
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_actionConnect_triggered();

    void on_actionClose_triggered();

    void on_actionDisconnect_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_actionAutoset_triggered();

    void on_actionSelf_Cal_triggered();

    void on_actionFactory_Reset_triggered();

    void on_ch1_coupling_combo_currentIndexChanged(int index);

    void on_ch1_probe_combo_currentIndexChanged(int index);

    void on_ch1_scale_combo_currentIndexChanged(int index);

    void on_ch2_coupling_combo_currentIndexChanged(int index);

    void on_ch2_probe_combo_currentIndexChanged(int index);

    void on_ch2_scale_combo_currentIndexChanged(int index);

    void on_deep_mem_combo_currentIndexChanged(int index);

    void on_timebase_combo_currentIndexChanged(int index);

    void on_sample_mode_combo_currentIndexChanged(int index);

    void on_samples_combo_currentIndexChanged(int index);

    void on_trigger_edge_combo_currentIndexChanged(int index);

    void on_trigger_edge_coupling_combo_currentIndexChanged(int index);

    void on_trigger_edge_trigmode_combo_currentIndexChanged(int index);

    void on_trigger_video_mod_combo_currentIndexChanged(int index);

    void on_trigger_video_sync_combo_currentIndexChanged(int index);

    void on_trigger_single_radio_clicked();

    void on_trigger_alternate_radio_clicked();

    void on_trigger_ch1_radio_clicked();

    void on_trigger_ch2_radio_clicked();

    void on_trigger_edge_radio_clicked();

    void on_trigger_video_radio_clicked();

    void on_trigger_edge_trigvoltage_spin_editingFinished();

    void on_ch1_position_spin_editingFinished();

    void on_ch2_position_spin_editingFinished();

    void on_hor_trig_spin_editingFinished();

    void on_trigger_video_line_spin_editingFinished();

    void on_actionScreenshot_triggered();

    void on_capture_btn_clicked();

    void on_forceupdate_btn_clicked();

private:
    Ui::owoncontrol *ui;
};

#endif // OWONCONTROL_H
