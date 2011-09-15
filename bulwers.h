/* Eyes
 * Copyright (C) 2011  Krzysztof Mędrzycki, Damian Chiliński
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

#ifndef _bulwers_h
#define _bulwers_h

#define IMG_NORMAL  0

#include <iostream>
#include "defines.hxx"

struct core_stats
{
    uint32  core_day,                 //get_time ().day;
            core_dnum,                //get_time ().day_num;
            core_month,               //get_time ().month;
            core_year,                //get_time ().year;
            core_time,                //get_time ().hour;
            core_battery_plugged,     //bat_plugged ();
            core_temperature,         //temperatura ();
            core_battery,             //bateria();
            core_cpu_load,            /* (cpu_probes[0] +
                                                cpu_probes[1] +
                                                cpu_probes[2] +
                                                cpu_probes[3] +
                                                cpu_probes[4] +
                                                cpu_probes[5] +
                                                cpu_probes[6] +
                                                cpu_probes[7] +
                                                cpu_probes[8] +
                                                cpu_probes[9]) / 10;*/
            current_probe,            //0
            current_probe_small,      //0
            cpu_probes[10],           //20 / C_LOAD ();
            cpu_sector_small[10],     //20 / C_LOAD ();
            core_memory,              //M_LOAD ();
            core_proclist,            //P_LIST ();
            core_uptime,              //U_TIME ();
            energy,                   //54000
            prev_bat_plug,            //core_battery_plugged
            battery_buffer,           //0
            temp_t,                   //0
            flu_timer,                //360
            prev_happy;               //0
    bool    once_plugged;             //false

};

struct bulwers_core
{
    int special;
    int longev;
    int happy;
    int step;
    int level;
};

struct pict_layers
{
int outline;
int eye;
int tired;
int hot;
int shy;
int layer2;
int layer3;
int layer4;

};

extern bool is_finished;
extern bool images_ready;

#endif // bulwers_h
