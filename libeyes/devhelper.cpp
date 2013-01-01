#include "devhelper.h"
#include "ui_devhelper.h"
#include "core.hxx"
#include "camera.hxx"
#include "hardware.hxx"

extern bul          bulwers;
extern camcapture   ccap;
extern percental    cpu;
extern percental    memory;
extern percental    battery;
extern unital       temperature;
extern timal        times;
extern timal        energy;
extern eMu_zone     eMu;
extern unsigned short  battery_state;

devHelper::devHelper (QWidget *parent) :
    QWidget(parent),
    ui(new Ui::devHelper)
{
    ui->setupUi(this);
    bulwers.fixed_eyes = 0;


    if (!ccap.enabled)
    {
        ui->mir_enabled->setEnabled(0);
        ui->eye_enabled->setEnabled(0);
    }

    if (!ccap.mir.enabled)
        ui->mir_enabled->setEnabled(0);
    else
        ui->mir_enabled->setChecked(1);
    {
        if (!ccap.mir.enabled)
        {
            ui->mir_fisheye->setEnabled(0);
            ui->mir_bloom->setEnabled(0);
            ui->mir_blurtype->setEnabled(0);
            ui->mir_alpha_correction->setEnabled(0);
        }
        ui->mir_fisheye->setChecked(ccap.mir.distort);
        ui->mir_bloom->setChecked(ccap.mir.bloom);
        if (ccap.mir.gaussian)
            ui->mir_blurtype->setCurrentIndex(1);
        else if (ccap.mir.boxblur)
            ui->mir_blurtype->setCurrentIndex(2);
        else
            ui->mir_blurtype->setCurrentIndex(0);
        ui->mir_alpha_correction->setValue(ccap.mir.alphacorrection);
    }

    ui->eye_enabled->setChecked(ccap.env.eyemanagement);
    {
        if (!ccap.env.eyemanagement)
        {
            ui->eye_maxbright->setEnabled(0);
            ui->eye_minbright->setEnabled(0);
        }
        ui->eye_maxbright->setValue(ccap.env.maxbright);
        ui->eye_minbright->setValue(ccap.env.minbright);
    }

    if (!eMu.cpu)
        ui->eMu_cpu_v->setEnabled(0);
    else
        ui->eMu_cpu->setChecked(1);
    ui->eMu_cpu_v->setValue(eMu.cpu_val);

    if (!eMu.mem)
        ui->eMu_mem_v->setEnabled(0);
    else
        ui->eMu_mem->setChecked(1);
    ui->eMu_mem_v->setValue(eMu.mem_val);

    if (!eMu.batt)
        ui->eMu_batt_v->setEnabled(0);
    else
        ui->eMu_batt->setChecked(1);
    ui->eMu_batt_v->setValue(eMu.batt_val);

    if (!eMu.temp)
    {
        ui->eMu_temp_v->setEnabled(0);
        ui->eMu_temp_flush->setEnabled(0);
    }
    else
        ui->eMu_temp->setChecked(1);

    if (!eMu.batt_s)
        ui->eMu_batts_v->setEnabled(0);
    else
        ui->eMu_batts->setChecked(1);
    ui->eMu_batts_v->setCurrentIndex(battery_state);

    if (!eMu.bulwers)
    {
        ui->eMu_bulwers_v->setEnabled(0);
        ui->eMu_bulwers_flush->setEnabled(0);
    }
    else
        ui->eMu_bulwers->setChecked(1);

    if (!eMu.time)
        ui->eMu_time_v->setEnabled(0);
    else
        ui->eMu_time->setChecked(1);

    if (!eMu.energy)
        ui->eMu_energy_v->setEnabled(0);
    else
        ui->eMu_energy->setChecked(1);

    ui->lcd_cpu->setSegmentStyle(QLCDNumber::Flat);
    ui->lcd_mem->setSegmentStyle(QLCDNumber::Flat);
    ui->lcd_temp->setSegmentStyle(QLCDNumber::Flat);
    ui->lcd_batt->setSegmentStyle(QLCDNumber::Flat);
}

devHelper::~devHelper()
{
    delete ui;
}

void devHelper::refresh()
{
    ui->lcd_cpu->display(cpu.load);
    ui->lcd_mem->display(memory.load);
    ui->lcd_temp->display((double)temperature.value);
    ui->lcd_batt->display(battery.load);
    ui->lcd_energy->setMaximum((energy.wide)/60);
    if (energy.value > energy.start)
        ui->lcd_energy->setValue((energy.wide - (energy.value - energy.start))/60);
    else
        ui->lcd_energy->setValue((energy.wide)/60);
}

void devHelper::on_mir_enabled_toggled(bool checked)
{
    if (checked)
    {
        ccap.mir.enabled = true;
        ui->mir_fisheye->setEnabled(1);
        ui->mir_bloom->setEnabled(1);
        ui->mir_blurtype->setEnabled(1);
        ui->mir_alpha_correction->setEnabled(1);
    }
    else
    {
        ccap.mir.enabled = false;
        ui->mir_fisheye->setEnabled(0);
        ui->mir_bloom->setEnabled(0);
        ui->mir_blurtype->setEnabled(0);
        ui->mir_alpha_correction->setEnabled(0);
    }
}

void devHelper::on_mir_fisheye_toggled(bool checked)
{
    ccap.mir.distort = checked;
}

void devHelper::on_mir_bloom_toggled(bool checked)
{
    ccap.mir.bloom = checked;
}

void devHelper::on_mir_blurtype_activated(int index)
{
    if (index == 0)
    {
        ccap.mir.boxblur = 0;
        ccap.mir.gaussian = 0;
    }
    if (index == 1)
    {
        ccap.mir.boxblur = 0;
        ccap.mir.gaussian = 1;
    }
    if (index == 2)
    {
        ccap.mir.boxblur = 1;
        ccap.mir.gaussian = 0;
    }
}

void devHelper::on_mir_alpha_correction_valueChanged(double arg1)
{
    ccap.mir.alphacorrection = arg1;
}

void devHelper::on_eye_enabled_toggled(bool checked)
{
    if (checked)
    {
        ccap.env.eyemanagement = 1;
        ui->eye_maxbright->setEnabled(1);
        ui->eye_minbright->setEnabled(1);
    }
    else
    {
        ccap.env.eyemanagement = 0;
        ui->eye_maxbright->setEnabled(0);
        ui->eye_minbright->setEnabled(0);
    }
}

void devHelper::on_eye_minbright_dialMoved(int value)
{
    ccap.env.minbright = value;
}

void devHelper::on_eye_maxbright_dialMoved(int value)
{
    ccap.env.maxbright = value;
}

void devHelper::on_eye_fixed_toggled(bool checked)
{
    if (checked)
    {
        bulwers.fixed_eyes = 1;
        ui->eye_fixedsize->setEnabled(1);
    }
    else
    {
        bulwers.fixed_eyes = 0;
        ui->eye_fixedsize->setEnabled(0);
    }
}

void devHelper::on_eye_fixedsize_valueChanged(int arg1)
{
    bulwers.eye = arg1;
}

void devHelper::on_close_dev_helper_accepted()
{
    this->hide();
}

void devHelper::on_eMu_cpu_toggled(bool checked)
{
    if (checked)
    {
        eMu.cpu = 1;
        ui->eMu_cpu_v->setEnabled(1);
    }
    else
    {
        eMu.cpu = 0;
        ui->eMu_cpu_v->setEnabled(0);
    }
}

void devHelper::on_eMu_mem_toggled(bool checked)
{
    if (checked)
    {
        eMu.mem = 1;
        ui->eMu_mem_v->setEnabled(1);
    }
    else
    {
        eMu.mem = 0;
        ui->eMu_mem_v->setEnabled(0);
    }
}

void devHelper::on_eMu_batt_toggled(bool checked)
{
    if (checked)
    {
        eMu.batt = 1;
        ui->eMu_batt_v->setEnabled(1);
    }
    else
    {
        eMu.batt = 0;
        ui->eMu_batt_v->setEnabled(0);
    }
}

void devHelper::on_eMu_cpu_v_dialMoved(int value)
{
    eMu.cpu_val = value;
}

void devHelper::on_eMu_mem_v_dialMoved(int value)
{
    eMu.mem_val = value;
}

void devHelper::on_eMu_batt_v_dialMoved(int value)
{
    eMu.batt_val = value;
}

void devHelper::on_eMu_temp_toggled(bool checked)
{
    if (checked)
    {
        ui->eMu_temp_v->setEnabled(1);
        ui->eMu_temp_flush->setEnabled(1);
    }
    else
    {
        eMu.temp = 0;
        ui->eMu_temp_v->setEnabled(0);
        ui->eMu_temp_flush->setEnabled(0);
    }
}

void devHelper::on_eMu_temp_flush_clicked()
{
    eMu.temp_val = ui->eMu_temp_v->text().toInt();
    eMu.temp = 1;
}

void devHelper::on_eMu_batts_toggled(bool checked)
{
    if (checked)
    {
        eMu.batt_s = 1;
        ui->eMu_batts_v->setEnabled(1);
    }
    else
    {
        eMu.batt_s = 0;
        ui->eMu_batts_v->setEnabled(0);
    }
}

void devHelper::on_eMu_batts_v_currentIndexChanged(int index)
{
    eMu.batt_s_val = index;
}

void devHelper::on_eMu_bulwers_toggled(bool checked)
{
    if (checked)
    {
        ui->eMu_bulwers_v->setEnabled(1);
        ui->eMu_bulwers_flush->setEnabled(1);
    }
    else
    {
        eMu.bulwers = 0;
        ui->eMu_bulwers_v->setEnabled(0);
        ui->eMu_bulwers_flush->setEnabled(0);
    }
}

void devHelper::on_eMu_bulwers_flush_clicked()
{
    eMu.bulwers_val = ui->eMu_bulwers_v->value();
    eMu.bulwers = 1;
}

void devHelper::on_eMu_time_toggled(bool checked)
{
    if (checked)
    {
        eMu.time = 1;
        ui->eMu_time_v->setEnabled(1);
    }
    else
    {
        eMu.time = 0;
        ui->eMu_time_v->setEnabled(0);
    }
}

void devHelper::on_eMu_time_v_timeChanged(const QTime &date)
{
    eMu.time_val = ((double)(date.hour()*3600 + date.minute()*60))/3600.0;
}

void devHelper::on_eMu_energy_toggled(bool checked)
{
    if (checked)
    {
        eMu.energy = 1;
        ui->eMu_energy_v->setEnabled(1);
    }
    else
    {
        eMu.energy = 0;
        ui->eMu_energy_v->setEnabled(0);
    }
}



void devHelper::on_eMu_energy_v_timeChanged(const QTime &date)
{
    eMu.energy_val = (double)(date.hour()*3600 + date.minute()*60);
}
