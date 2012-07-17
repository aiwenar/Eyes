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
#include <libconfig.h++>
#include <QProcess>
#include <fstream>

#include "eyes.hxx"

using namespace std;
using namespace libconfig;

struct sdate
{
    unsigned short              day,
                                month,
                                day_num;
    unsigned int                hour,
                                year;
};

class percental
{
public:
    short                       mod,
                                mod_prev;
    vector <double>             probes,
                                sector_small;
    double                      load,
                                stable;
    unsigned int                buff_size,
                                EQsize,
                                steps,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                loseless,
                                calculate(),
                                convert(unsigned short val);
    vector <int>                EQ;
    char                        frequency;
    void                        get_load (double),
                                init (QString adress);
    bool                        buffered;
};

class unital
{
public:
    short                       mod,
                                mod_prev;
    vector<unsigned short>      probes,
                                sector_small;
    unsigned int                value,
                                stable,
                                buff_size,
                                EQsize,
                                EQbegin,
                                EQend,
                                steps,
                                unit,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                loseless,
                                calculate(),
                                convert(unsigned short val);
    vector <int>                EQ;
    char                        frequency;
    void                        get_load (unsigned short),
                                init (QString adress);
    bool                        buffered;
};

class timal
{
public:
    short                       mod;
    unsigned int                value,
                                start,
                                steps,
                                lin_num,
                                wide,
                                end,
                                calculate();
    char                        frequency;
    void                        init (QString adress);
};

class bul
{
public:
    char                        wkup_reason,
                                wkup_active;
    bool                        wake_up,
                                flue,
                                no_update;
    int                         total_mod;
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
                                fluetimer,
                                remembered_time,
                                remembered_nrg,
                                max_mem_lag,
                                rest_time_std,
                                rest_time_wkend;
    unsigned short              outline,
                                prev_outline,
                                eye,
                                tired,
                                hot,
                                shy,
                                layer2,
                                layer3,
                                layer4,
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
                                flueamplitude,
                                flueimpact,
                                fluestepdelay;
    double                      fluelowval,
                                fluehighval;
    void                        update(),
                                flue_check(),
                                critical_services( Configuration * cfg ),
                                wake_up_chk();
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
                                max_over;
    double                      func_calm_low,
                                func_calm_high,
                                func_mouse_low,
                                func_mouse_high,
                                func_mouse_hit,
                                stable,
                                func_scale,
                                mouse_good,
                                mouse_bad,
                                funccalc(double angle, unsigned int current);
    long double                 value;
    bool                        to_save;
};

class auto_calc
{
public:
    vector<long double>         cpu_freq,
                                cpu_virtualEQ,
                                cpu_curve,
                                memory_freq,
                                memory_virtualEQ,
                                memory_curve,
                                battery_freq,
                                battery_virtualEQ,
                                battery_curve,
                                temperature_freq,
                                temperature_virtualEQ,
                                temperature_curve,
                                time_freq;
    long double                 c_cpu,
                                c_mem,
                                c_battery,
                                c_temp,
                                c_time,
                                impact,
                                cpu_mult_low,
                                cpu_mult_low_converter,
                                cpu_mult_high,
                                cpu_mult_high_converter,
                                memory_mult_low,
                                memory_mult_low_converter,
                                memory_mult_high,
                                memory_mult_high_converter,
                                battery_mult_low,
                                battery_mult_low_converter,
                                battery_mult_high,
                                battery_mult_high_converter,
                                temperature_mult_low,
                                temperature_mult_low_converter,
                                temperature_mult_high,
                                temperature_mult_high_converter,
                                time_mult,
                                cpu_common,
                                cpu_exoticlow,
                                cpu_exotichigh,
                                cpu_curve_correct,
                                cpu_freq_angle_low,
                                cpu_freq_angle_high,
                                memory_common,
                                memory_exoticlow,
                                memory_exotichigh,
                                memory_curve_correct,
                                memory_freq_angle_low,
                                memory_freq_angle_high,
                                battery_common,
                                battery_exoticlow,
                                battery_exotichigh,
                                battery_curve_correct,
                                battery_freq_angle_low,
                                battery_freq_angle_high,
                                temperature_common,
                                temperature_exoticlow,
                                temperature_exotichigh,
                                temperature_curve_correct,
                                temperature_freq_angle_low,
                                temperature_freq_angle_high;
    int                         cpu_curstep,
                                cpu_perc,
                                cpu_curmid,
                                cpu_stablepointlow,
                                cpu_stablepointhigh,
                                memory_curstep,
                                memory_perc,
                                memory_curmid,
                                memory_stablepointlow,
                                memory_stablepointhigh,
                                battery_curstep,
                                battery_perc,
                                battery_curmid,
                                battery_stablepointlow,
                                battery_stablepointhigh,
                                temperature_curstep,
                                temperature_perc,
                                temperature_curmid,
                                temperature_stablepointlow,
                                temperature_stablepointhigh;
    unsigned long               save_next,
                                save_interval,
                                start_delay;
    unsigned short              cpu_swalll,
                                cpu_swallh,
                                memory_swalll,
                                memory_swallh,
                                battery_swalll,
                                battery_swallh,
                                temperature_swalll,
                                temperature_swallh,
                                time_swalll,
                                time_swallh;
    bool                        enabled,
                                cpu_enabled,
                                memory_enabled,
                                battery_enabled,
                                temperature_enabled,
                                time_enabled,
                                auto_cpu,
                                auto_memory,
                                auto_battery,
                                auto_temperature,
                                auto_time,
                                cpu_simple,
                                mem_simple,
                                bat_simple,
                                temp_simple,
                                time_simple,
                                forcesave;
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
    void                        action (string command);
    unsigned short              batt_min_backl,
                                batt_start_perc,
                                batt_suspend_perc,
                                temp_halt_start;
    bool                        roottype,
                                temp_halt_enabled,
                                suspendtohdd,
                                screenctrl;
private:
    void                        execute(bool roottype, QString command, QStringList arguments),
                                execute(bool roottype, QString command);
};

extern eyes_view     * eyes;

#endif //gbssl_core_new_hxx
