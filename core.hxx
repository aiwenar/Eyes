/* Eyes
 * Copyright (C) 2011, 2012  Krzysztof Mędrzycki, Damian Chiliński
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _gbssl_core_new_hxx
#define _gbssl_core_new_hxx

#include <QString>
#include <QObject>
#include <vector>
#include <QTimer>
#include <QProcess>
#include <fstream>

#include "eyes.hxx"

using namespace std;



struct environment_data
{
    double                      Rperc,
                                Yperc,
                                Gperc,
                                Bperc,
                                Pperc,
                                Hperc;
    unsigned long long          spenttime;
    unsigned short              timer;
};

class bul
{
public:
    char                        wkup_reason,
                                wkup_active;
    bool                        wake_up,
                                no_update,
                                force_autosave,
                                check_env(bool enabled, Configuration * cfg );
    int                         total_mod,
                                envindex;
    unsigned int                step,
                                wall_01,
                                wall_02,
                                wall_03,
                                wall_04,
                                wall_05,
                                wall_06,
                                wall_07,
                                wall_08,
                                wall_09,
                                wall_10,
                                wall_11,
                                wall_12,
                                wall_13,
                                wall_14,
                                wall_15,
                                limiter,
                                remembered_time,
                                remembered_nrg,
                                lastnap_remembered_time,
                                lastnap_atime,
                                lastnap_saved,
                                lastnap_rest,
                                lastnap_dtime,
                                dtime,
                                atime,
                                max_mem_lag,
                                rest_time_std,
                                rest_time_wkend,
                                env_saveinterval,
                                autosave_interval;
    unsigned short              outline,
                                prev_outline,
                                eye,
                                tired,
                                hot,
                                shy,
                                hpp,
                                max_fun_hpp_bul,
                                smile_probability,
                                value,
                                wake_up_delay,
                                current_wkup_delay,
                                nrg_low,
                                nrg_std,
                                nrg_boost,
                                wkup_time,
                                wkup_timew,
                                timelow_1,
                                timelow_2,
                                timelow_3,
                                timehigh_1,
                                timehigh_2,
                                timehigh_3,
                                timelow_1w,
                                timelow_2w,
                                timelow_3w,
                                timehigh_1w,
                                timehigh_2w,
                                timehigh_3w,
                                quickcalm_bulwers;
    double                      sweat_perc_1,
                                sweat_perc_2,
                                sweat_perc_3,
                                hpp_fun_perc_1,
                                hpp_fun_perc_2,
                                env_min_compability,
                                env_update_impact,
                                env_max_exotic_spenttime,
                                env_max_compability,
                                quickcalm;
    void                        update(),
                                critical_services( Configuration * cfg ),
                                wake_up_chk(),
                                fun_check(),
                                autosave ( Configuration * cfg );
    vector <environment_data>   envs;
    environment_data            curenv;
};

struct disease_time
{
    unsigned short              day,
                                month,
                                year,
                                lenght;
    double                      minute_perc,
                                invertion_step,
                                progress;
};

class disease
{
public:
    bool                        active,
                                expired(disease_time disease_data);
    unsigned short              amplitude,
                                step_perc_1,
                                step_perc_2,
                                step_perc_3,
                                step_perc_4,
                                step_perc_5,
                                visual_impact_probability_1,
                                visual_impact_probability_2,
                                visual_impact_probability_3,
                                visual_impact_probability_4,
                                visual_impact_probability_5;
    double                      lowval,
                                highval,
                                update_impact,
                                fun_impact,
                                pet_impact,
                                hit_impact,
                                bul_impact,
                                max_bul_booster,
                                invertion_perc;
    void                        check( Configuration * cfg ),
                                attack( Configuration * cfg ),
                                visual_impact(double progress);
    disease_time                last_date;
    QTimer                      uptimer;
};

class friendship
{
public:
    void                        timeimpact(unsigned int delay),
                                mouseimpact(unsigned int impact),
                                save( Configuration * cfg );
    unsigned int                calm_timer,
                                calm_perc_low,
                                calm_perc_high,
                                max_below,
                                max_over,
                                max_bul_reduction;
    double                      func_calm_low,
                                func_calm_high,
                                func_mouse_low,
                                func_mouse_high,
                                func_mouse_hit,
                                stable,
                                func_scale,
                                mouse_good,
                                mouse_bad,
                                funboost,
                                funccalc(double angle, unsigned int current);
    long double                 value;
    bool                        to_save;
};

class auto_calc
{
public:
    //vector<long double>         //cpu_freq,
                                //cpu_virtualEQ,
                                //cpu_curve,
                                //memory_freq,
                                //memory_virtualEQ,
                                //memory_curve,
                                //battery_freq,
                                //battery_virtualEQ,
                                //battery_curve,
                                //temperature_freq,
                                //temperature_virtualEQ,
                                //temperature_curve,
                                //time_freq;
    //long double                 //c_cpu,
                                //c_mem,
                                //c_battery,
                                //c_temp,
                                //c_time,
                                //impact;
                                //cpu_mult_low,
                                //cpu_mult_low_converter,
                                //cpu_mult_high,
                                //cpu_mult_high_converter,
                                //memory_mult_low,
                                //memory_mult_low_converter,
                                //memory_mult_high,
                                //memory_mult_high_converter,
                                //battery_mult_low,
                                //battery_mult_low_converter,
                                //battery_mult_high,
                                //battery_mult_high_converter,
                                //temperature_mult_low,
                                //temperature_mult_low_converter,
                                //temperature_mult_high,
                                //temperature_mult_high_converter,
                                //time_mult,
                                //cpu_common,
                                //cpu_exoticlow,
                                //cpu_exotichigh,
                                //cpu_curve_correct,
                                //cpu_freq_angle_low,
                                //cpu_freq_angle_high,
                                //memory_common,
                                //memory_exoticlow,
                                //memory_exotichigh,
                                //memory_curve_correct,
                                //memory_freq_angle_low,
                                //memory_freq_angle_high,
                                //battery_common,
                                //battery_exoticlow,
                                //battery_exotichigh,
                                //battery_curve_correct,
                                //battery_freq_angle_low,
                                //battery_freq_angle_high,
                                //temperature_common,
                                //temperature_exoticlow,
                                //temperature_exotichigh,
                                //temperature_curve_correct,
                                //temperature_freq_angle_low,
                                //temperature_freq_angle_high;
    //int                         //cpu_curstep,
                                //cpu_perc,
                                //cpu_curmid,
                                //cpu_stablepointlow,
                                //cpu_stablepointhigh,
                                //memory_curstep,
                                //memory_perc,
                                //memory_curmid,
                                //memory_stablepointlow,
                                //memory_stablepointhigh,
                                //battery_curstep,
                                //battery_perc,
                                //battery_curmid,
                                //battery_stablepointlow,
                                //battery_stablepointhigh,
                                //temperature_curstep,
                                //temperature_perc,
                                //temperature_curmid,
                                //temperature_stablepointlow,
                                //temperature_stablepointhigh;
    unsigned long               save_next,
                                save_interval,
                                start_delay;
    //unsigned short              //cpu_swalll,
                                //cpu_swallh,
                                //memory_swalll,
                                //memory_swallh,
                                //battery_swalll,
                                //battery_swallh,
                                //temperature_swalll,
                                //temperature_swallh,
                                //time_swalll,
                                //time_swallh;
    bool                        enabled;
                                //cpu_enabled,
                                //memory_enabled,
                                //battery_enabled,
                                //temperature_enabled,
                                //time_enabled,
                                //auto_cpu,
                                //auto_memory,
                                //auto_battery,
                                //auto_temperature,
                                //auto_time,
                                //cpu_simple,
                                //mem_simple,
                                //bat_simple,
                                //temp_simple,
                                //time_simple,
                                //forcesave;
};

struct sended_anims
{
    QString                     close,
                                open,
                                eyes,
                                face_prev;
    unsigned short              start,
                                end;
};

class mouse_actions
{
public:
    int                         cur,
                                buff_size,
                                prev_x,
                                prev_y,
                                result,
                                mod,
                                tr,
                                impact,
                                heavycalm,
                                convert();
    vector<long double>         buffer;
    unsigned int                wall,
                                force_wall,
                                scale,
                                hit_time,
                                max_delay,
                                opt_speed,
                                max_hpp_bul;
    double                      multiplier;
    bool                        hpp_active,
                                hit_active;
    eyes_view                   * eyes;
};

struct eMu_zone
{
    bool                        cpu,
                                mem,
                                temp,
                                time,
                                energy,
                                batt,
                                batt_s,
                                bulwers;
    unsigned short              cpu_val,
                                mem_val,
                                temp_val,
                                time_val,
                                batt_s_val;
    unsigned int                energy_val,
                                batt_val,
                                bulwers_val;
};

struct data_source
{
    char                        batt_pow_now,
                                batt_pow_max,
                                batt_stat,
                                temp;
};

class rootcontrol
{
public:
    void                        action (string command),
                                shelldetect(),
                                shell(string command),
                                scrnsav(signed short screensaverstate, bool activate, bool deactivate, bool turnoff);
    unsigned short              batt_min_backl,
                                batt_start_perc,
                                batt_suspend_perc,
                                temp_halt_start;
    bool                        roottype,
                                temp_halt_enabled,
                                suspendtohdd,
                                screenctrl,
                                scrnsaver_management,
                                customshell,
                                scrnsav_X,
                                scrnsav_kde,
                                scrnsav_gnome,
                                scrnsav_mac,
                                scrnsav_custom,
                                scrnsav_switched;
    string                      shellname,
                                scrnsav_custom_on,
                                scrnsav_custom_off;
private:
    void                        execute(bool roottype, QString command, QStringList arguments),
                                execute(bool roottype, QString command);
};

extern eyes_view     * eyes;

#endif //gbssl_core_new_hxx
