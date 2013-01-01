#ifndef DEVHELPER_H
#define DEVHELPER_H

#include "eyes.hxx"

namespace Ui {
class devHelper;
}

class devHelper : public QWidget
{
    Q_OBJECT
public:
    explicit    devHelper(QWidget *parent = 0);
    void        refresh();
    ~devHelper();

private:
    Ui::devHelper *ui;

private slots:
    void on_mir_enabled_toggled(bool checked);
    void on_mir_fisheye_toggled(bool checked);
    void on_mir_bloom_toggled(bool checked);
    void on_mir_blurtype_activated(int index);
    void on_mir_alpha_correction_valueChanged(double arg1);
    void on_eye_enabled_toggled(bool checked);
    void on_eye_minbright_dialMoved(int value);
    void on_eye_maxbright_dialMoved(int value);
    void on_eye_fixed_toggled(bool checked);
    void on_eye_fixedsize_valueChanged(int arg1);
    void on_close_dev_helper_accepted();
    void on_eMu_cpu_toggled(bool checked);
    void on_eMu_mem_toggled(bool checked);
    void on_eMu_batt_toggled(bool checked);
    void on_eMu_cpu_v_dialMoved(int value);
    void on_eMu_mem_v_dialMoved(int value);
    void on_eMu_batt_v_dialMoved(int value);
    void on_eMu_temp_toggled(bool checked);
    void on_eMu_temp_flush_clicked();
    void on_eMu_batts_toggled(bool checked);
    void on_eMu_batts_v_currentIndexChanged(int index);
    void on_eMu_bulwers_toggled(bool checked);
    void on_eMu_bulwers_flush_clicked();
    void on_eMu_time_toggled(bool checked);
    void on_eMu_time_v_timeChanged(const QTime &date);
    void on_eMu_energy_toggled(bool checked);
    void on_eMu_energy_v_timeChanged(const QTime &date);
};

#endif // DEVHELPER_H
