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

#ifndef HARDWARE_HXX
#define HARDWARE_HXX

#include "core.hxx"

class hardware
{
public:
    bool                    special_batname,
                            special_thername,
                            screen_support,
                            set_file(string path, string value),
                            set_file(string path, long double value),
                            set_backlight (unsigned short percentage);
    double                  C_LOAD              (),
                            M_LOAD              (),
                            owncpu;
    int                     U_TIME              (),
                            bateria             (),
                            bat_plugged         (),
                            pid;
    unsigned int            battery_capacity,
                            sys_backlight_now   (string path),
                            sys_backlight_full  (string path);
    unsigned short          P_LIST              (),
                            temperatura         (),
                            emubat              (string path),
                            emustat             (string path),
                            emutemp (string path),
                            (hardware::*src_batt_now)     (string),
                            (hardware::*src_batt_state)   (string),
                            (hardware::*src_temp)         (string),
                            proc_temp           (string path),
                            proc_bat_now        (string path),
                            proc_bat_full       (string path),
                            proc_bat_state      (string path),
                            sys_temp            (string path),
                            sys_bat_uni         (string path),
                            sys_bat_state       (string path),
                            sensors_temp        (string path);
    char                    final_temp_solution,
                            final_now_solution,
                            final_full_solution,
                            final_state_solution;
    void                    system_check        ();
    string                  final_path_temp,
                            final_path_now,
                            final_path_full,
                            final_path_state,
                            cfg_battname,
                            cfg_thername,
                            backlight_path,
                            backlight_fpath,
                            backlight_npath,
                            get_file            (char* path);
    vector <string>         cores_paths;
};


struct sdate
{
    unsigned short              day,
                                month,
                                day_num;
    unsigned int                hour,
                                year;
};

struct a_calc_data
{
    vector<long double>         freq,
                                virtualEQ,
                                curve;
    long double                 impact,
                                current,
                                mult_low,
                                mult_low_converter,
                                mult_high,
                                mult_high_converter,
                                common,
                                exoticlow,
                                exotichigh,
                                curve_correct,
                                freq_angle_low,
                                freq_angle_high;
    int                         curstep,
                                perc,
                                curmid,
                                stablepointlow,
                                stablepointhigh;
    unsigned short              swalll,
                                swallh;
    bool                        enabled,
                                auto_degree,
                                simple,
                                forcesave;
    string                      name;
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
                                max_mod_pos,
                                max_mod_neg,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                safezone,
                                calculate();
    int                         convert(unsigned short val);
    vector <int>                EQ;
    double                      degree,
                                mod_correction_pos,
                                mod_correction_neg;
    void                        get_load (double),
                                init (QString adress);
    bool                        buffered,
                                ready(),
                                autocalc(),
                                ac_save( Configuration * cfg ),
                                ac_init( string name );
    a_calc_data                 ac;
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
                                max_mod_pos,
                                max_mod_neg,
                                unit,
                                lin_num,
                                current_probe,
                                current_probe_small,
                                safezone,
                                calculate();
    int                         convert(unsigned short val);
    vector <int>                EQ;
    double                      degree,
                                mod_correction_pos,
                                mod_correction_neg;
    void                        get_load (unsigned short),
                                init (QString adress);
    bool                        buffered,
                                ready(),
                                autocalc( Configuration * cfg ),
                                ac_save( Configuration * cfg ),
                                ac_init( string name);
    a_calc_data                 ac;
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

extern          hardware        HRDWR;
extern          sdate           get_time    ();
extern          unital          temperature;
extern          percental       battery;
extern          unsigned int    battery_capacity;
extern          data_source     d_src;

using namespace std;

#endif // HARDWARE_HXX
