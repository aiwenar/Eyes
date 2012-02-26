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
                                calculate();
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
                                calculate();
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
    int                         total_mod,
                                friendship;
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
                                wall_15;
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
                                fship_at_calm,
                                calm_perc;
    void                        update(),
                                flue_check(),
                                critical_services();
};

class auto_calc
{
public:
    double                      c_cpu,
                                c_mem,
                                c_temp,
                                impact;
    unsigned long               save_next,
                                save_interval;
    bool                        enabled;
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
                                goodstep,
                                badstep,
                                hit_time,
                                hit_time_multi,
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

extern eyes_view     * eyes;

#endif //gbssl_core_new_hxx
