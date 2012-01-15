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

#include <iostream>
#include <string.h>
#include <time.h>
#include <vector>
#include <qstring.h>
#include <qvector.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <cmath>
#include "core.hxx"
#include "eyes.hxx"
#include "debug.hxx"
#include "cdbg.hxx"
#include "hardware.hxx"

using namespace std;

unsigned short  battery_state;
unsigned short  mod_bat_plug;
unsigned int    temp_t;
unsigned int    flu_timer;
unsigned int    prev_bat_plug;
bool            once_plugged;
bool            get_flu;
bool            cdebug;
int             core_step;
percental       cpu;
percental       memory;
percental       battery;
unital          temperature;
timal           times;
timal           energy;
bul             bulwers;
sended_anims    s_anim;
auto_calc       autocalc;
mouse_actions   mousea;
QString         face_send;
QTime           mousetime;
eMu_zone        eMu;
data_source     d_src;

void eyes_view::anims_send ( QString fac, QString nstart, QString nend, unsigned short nfrom, unsigned short nto )
{
    s_anim.open = nstart;
    s_anim.close = nend;
    s_anim.start = nfrom;
    s_anim.end = nto;
    face_next = fac;
}
void eyes_view::anims_reload()
{
    //info << "anims_reload begin\n";

    //if (s_anim.close == "" || s_anim.open == "")
    //    cerr << "\033[32mOops! My bad :]\n\n";

    set_animation(s_anim.open, s_anim.close, s_anim.start, s_anim.end);

    //info << "sended pics:\n" << s_anim.open.toStdString() << " - open anim\n"
    //     << s_anim.close.toStdString() << " - close anim\n"
    //     << "start/end " << s_anim.start << " " << s_anim.end
    //     << "\nface_next = " << face_next.toStdString() << "\n";
}

void eyes_view::reload_eyes()
{
    set_eyes ( s_anim.eyes );
}

void eyes_view::send_eyes( QString neyes )
{
    if ( neyes == "" )
    {
        //cerr << "033[32mOops! My bad :]";
        neyes = "eye_06";
    }
    s_anim.eyes = neyes;
    //cerr << "core sets \"" << s_anim.eyes.toStdString() << "\" eye\n";
}



void bul::update()
{
    total_mod =
            cpu.mod         +
            memory.mod      +
            times.mod       +
            energy.mod      +
            temperature.mod -
            battery.mod     -
            mod_bat_plug    -
            friendship/100  -
            mousea.mod      ;
    if ((step > -total_mod && total_mod < 0) || total_mod >= 0)
        step += total_mod;
    else
        step = 0;

    if (step < wall_01)
        value = 0;
    if (step > wall_01 && step <= wall_02)
        value = 1;
    if (step > wall_02 && step <= wall_03)
        value = 2;
    if (step > wall_03 && step <= wall_04)
        value = 3;
    if (step > wall_04 && step <= wall_05)
        value = 4;
    if (step > wall_05 && step <= wall_06)
        value = 5;
    if (step > wall_06 && step <= wall_07)
        value = 6;
    if (step > wall_07 && step <= wall_08)
        value = 7;
    if (step > wall_08 && step <= wall_09)
        value = 8;
    if (step > wall_09 && step <= wall_10)
        value = 9;
    if (step > wall_10 && step <= wall_11)
        value = 10;
    if (step > wall_11 && step <= wall_12)
        value = 11;
    if (step > wall_12 && step <= wall_13)
        value = 12;
    if (step > wall_13 && step <= wall_14)
        value = 13;
    if (step > wall_14 && step <= wall_15)
        value = 14;
    if (step > wall_15)
        value = 15;

    if (step != 0)
        step--;

    mousea.multiplier = 1;//= (double)value/10.0;
    if (mousea.hit_time > 2 && core_step%10 == 0)
        mousea.hit_time--;

    if (core_step%fship_at_calm == 0)
    {
        friendship-=calm_perc*friendship/100;
    }

    if (battery_state == 0)
    {
        outline = 20;
        eye = 10;
    }
    else if (outline == 20)
        outline = 0;

    if (once_plugged)
    {
        if (battery_state == 2 && prev_bat_plug != 2)
        {
            if (prev_bat_plug == 3)
            {
                mod_bat_plug--;
            }

            if (prev_bat_plug == 1)
            {
                mod_bat_plug-=2;
            }
        }
        if (battery_state == 1 && prev_bat_plug != 1)
        {
            mod_bat_plug+=2;
        }
        if (battery_state == 3 && prev_bat_plug != 3)
        {
            mod_bat_plug--;
        }
    }
    if (!once_plugged)
    {
        if (battery_state == 1)
        {
            once_plugged = true;
            mod_bat_plug +=2;
        }
        if (battery_state == 3)
        {
            once_plugged = true;
            mod_bat_plug++;
        }
    }
    prev_bat_plug = battery_state;


    if (value != 0 && outline != 20 && outline != 21)
        outline = value + 3;
    if (outline != 20 && value == 0)
        outline = 0;


    if (get_time ().month == 12 || get_time ().month == 1 )
    if (outline != 20)
    {
        if (times.value < 7 || times.value >= 17)
            eye = 1;
        if ((times.value >= 7 && times.value < 8) || (times.value >= 16 && times.value < 17))
            eye = 2;
        if ((times.value >= 8 && times.value < 9) || (times.value >= 15 && times.value < 16))
            eye = 3;
        if ((times.value >= 9 && times.value < 10) || (times.value >= 14 && times.value < 15))
            eye = 4;
        if ((times.value >= 10 && times.value < 11) || (times.value >= 13 && times.value < 14))
            eye = 5;
        if ((times.value >= 11 && times.value < 13))
            eye = 6;
    }

    if (get_time ().month == 2 || get_time ().month == 3 || get_time ().month == 10 || get_time ().month == 11 )
    if (outline != 20)
    {
        if (times.value < 6 || times.value >= 18)
            eye = 1;
        if ((times.value >= 6 && times.value < 7) || (times.value >= 17 && times.value < 18))
            eye = 2;
        if ((times.value >= 7 && times.value < 8) || (times.value >= 16 && times.value < 17))
            eye = 3;
        if ((times.value >= 8 && times.value < 9) || (times.value >= 15 && times.value < 16))
            eye = 4;
        if ((times.value >= 9 && times.value < 10) || (times.value >= 14 && times.value < 15))
            eye = 5;
        if ((times.value >= 10 && times.value < 11) || (times.value >= 13 && times.value < 14))
            eye = 6;
        if ((times.value >= 11 && times.value < 13))
            eye = 7;
    }

    if (get_time ().month == 4 || get_time ().month == 5 || get_time ().month == 8 || get_time ().month == 9 )
    if (outline != 20)
        {
            if (times.value < 5 || times.value >= 19)
                eye = 1;
            if ((times.value >= 5 && times.value < 6) || (times.value >= 18 && times.value < 19))
                eye = 2;
            if ((times.value >= 6 && times.value < 7) || (times.value >= 17 && times.value < 18))
                eye = 3;
            if ((times.value >= 7 && times.value < 8) || (times.value >= 16 && times.value < 17))
                eye = 4;
            if ((times.value >= 8 && times.value < 9) || (times.value >= 15 && times.value < 16))
                eye = 5;
            if ((times.value >= 9 && times.value < 10) || (times.value >= 14 && times.value < 15))
                eye = 6;
            if ((times.value >= 10 && times.value < 11) || (times.value >= 13 && times.value < 14))
                eye = 7;
            if ((times.value >= 11 && times.value < 13))
                eye = 8;
        }

    if (get_time ().month == 6 || get_time ().month == 7 )
    if (outline != 20)
    {
        if (times.value < 3 || times.value >= 21)
            eye = 1;
        if ((times.value >= 3 && times.value < 4) || (times.value >= 20 && times.value < 21))
            eye = 2;
        if ((times.value >= 4 && times.value < 5) || (times.value >= 19 && times.value < 20))
            eye = 3;
        if ((times.value >= 5 && times.value < 6) || (times.value >= 18 && times.value < 19))
            eye = 4;
        if ((times.value >= 6 && times.value < 7) || (times.value >= 17 && times.value < 18))
            eye = 5;
        if ((times.value >= 7 && times.value < 8) || (times.value >= 16 && times.value < 17))
            eye = 6;
        if ((times.value >= 8 && times.value < 9) || (times.value >= 15 && times.value < 16))
            eye = 7;
        if ((times.value >= 9 && times.value < 10) || (times.value >= 14 && times.value < 15))
            eye = 8;
        if ((times.value >= 10 && times.value < 11) || (times.value >= 13 && times.value < 14))
            eye = 9;
        if ((times.value >= 11 && times.value < 13))
            eye = 10;
    }

    if (eye == 0)
    {
        cerr << "eye sieze recognize failed!: setting emergency value (6)\n";
        eye = 6;
    }


    //TODO 01: It must works on cfg values, not static.

    if (get_time ().day != 7)
    {
        if (times.value < 6 || times.value > 21 || energy.value > energy.start + energy.wide - 5)
        {
            tired = 1;

            if (outline <= 7)
                outline = 1;

            if (outline > 7 && outline < 11)
            {
                if ((rand () % 1))
                    outline = 1;
            }
        }
        if (times.value < 6 || times.value > 22 || energy.value > energy.start + energy.wide - 3)
        {
            tired = 2;
            if (outline <= 8)
                outline = 1;

            if (outline > 8 && outline < 12)
            {
                if ((rand () % 1))
                    outline = 2;
            }
        }
        if (times.value < 5 || times.value > 23 || energy.value > energy.start + energy.wide - 1 )
        {
            tired = 3;
            if (outline <= 9)
                outline = 1;

            if (outline > 9 && outline < 13)
            {
                if ((rand () % 1))
                    outline = 3;
            }
        }
        else
        {
            if (tired > 0)
                tired--;
        }
    }
    else
    {
        if (times.value < 10 || times.value > 23 ||  energy.value > energy.start + energy.wide - 5*3600)
        {
            tired = 1;
            if (outline <= 7)
                outline = 1;

            if (outline > 7 && outline < 11)
            {
                if ((rand () % 1))
                    outline = 1;
            }
        }
        if ((times.value < 8 && times.value > 1) ||  energy.value > energy.start + energy.wide - 3*3600)
        {
            tired = 2;
            if (outline <= 8)
                outline = 1;

            if (outline > 8 && outline < 12)
            {
                if ((rand () % 1))
                    outline = 2;
            }
        }
        if ((times.value < 7 && times.value > 3) ||  energy.value > energy.start + energy.wide - 3*3600 )
        {
            tired = 3;
            if (outline <= 9)
                outline = 1;

            if (outline > 9 && outline < 13)
            {
                if ((rand () % 1))
                    outline = 3;
            }
        }
        else
        {
            if (tired > 0)
                tired--;
            if (outline <= 3 && outline >= 1)
                outline--;
        }
    }

    //--flue

    if (temperature.value >= 56 && temperature.value < 58)
    {
        hot = 1;
        if (temp_t < 30)
            temp_t = 30;
        if (temp_t > 60)
            get_flu = true;
    }
    if (temperature.value >= 58 && temperature.value < 60)
    {
        hot = 2;
        if (temp_t < 60)
            temp_t = 60;
        if (temp_t > 80)
            get_flu = true;
    }
    if (temperature.value >= 60 && temperature.value < 62)
    {
        hot = 3;
        if (temp_t < 80)
            temp_t = 80;
        if (temp_t > 120)
            get_flu = true;
    }
    if (temperature.value >= 62 && temperature.value < 64)
    {
        hot = 4;
        if (temp_t < 120)
            temp_t = 120;
    }
    if (temperature.value >= 64)
    {
        hot = 5;
        if (temp_t < 180)
            temp_t = 180;
    }

    else
    {
        if (hot > 0)
            hot--;
    }

    if (temp_t > 0)
        temp_t--;

    if (get_flu)
    {

        if (flu_timer > 0)
            flu_timer--;

        if (flu_timer <= 240)
        {
            hot = 1;
            shy = 1;
            tired = 1;
            if (value < 8)
                value = 8;
        }
        if (flu_timer <= 120)
        {
            hot = 2;
            shy = 2;
            tired = 2;
            if (value < 8)
                value = 8;
        }
        if (flu_timer == 0)
        {
            hot = 3;
            shy = 3;
            tired = 3;
            if (value < 10)
                value = 10;
            energy.value++;
        }
    }
}

void Core::bulwers_init ()
{

if (cpu.buffered)
{
    cpu.current_probe        = 0  ;
    cpu.current_probe_small  = 0  ;

    for (unsigned short i = 0; i<=cpu.buff_size; i++)
    {
        cpu.probes.push_back (cpu.stable);
    }
    for (unsigned short i = 0; i<=cpu.buff_size; i++)
    {
        cpu.sector_small.push_back (cpu.stable);
    }
}

if (memory.buffered)
{
    memory.current_probe        = 0  ;
    memory.current_probe_small  = 0  ;

    for (unsigned short i = 0; i<=memory.buff_size; i++)
    {
        memory.probes.push_back (memory.stable);
    }
    for (unsigned short i = 0; i<=memory.buff_size; i++)
    {
        memory.sector_small.push_back (memory.stable);
    }
}

if (battery.buffered)
{
    battery.current_probe       = 0  ;
    battery.current_probe_small = 0  ;

    for (unsigned short i = 0; i<=battery.buff_size; i++)
    {
        battery.probes.push_back (battery.stable);
    }
    for (unsigned short i = 0; i<=battery.buff_size; i++)
    {
        battery.sector_small.push_back (battery.stable);
    }
}

if (temperature.buffered)
{
    temperature.current_probe       = 0  ;
    temperature.current_probe_small = 0  ;

    for (unsigned short i = 0; i<=temperature.buff_size; i++)
    {
        temperature.probes.push_back (temperature.stable);
    }
    for (unsigned short i = 0; i<=temperature.buff_size; i++)
    {
        temperature.sector_small.push_back (temperature.stable);
    }
}

mousea.cur                          = 0    ;
mousea.result                       = 0    ;
mousea.prev_x                       = 0    ;
mousea.prev_y                       = 0    ;
mousea.hit_time                     = 1    ;

for (unsigned int i = 0; i<=mousea.buff_size; i++)
{
    mousea.buffer.push_back (0);
}

energy.value                        = 0    ;
energy.start                       *= 3600 ;
energy.wide                        *= 3600 ;
once_plugged                        = false;
mod_bat_plug                        = 0    ;
bulwers.step                        = 0    ;


}

void percental::get_load( double function )
{
    if (buffered)
    {
        current_probe_small ++;

        if (current_probe_small == buff_size)
            current_probe_small = 0;

        sector_small[current_probe_small] = function;
        if (sector_small[current_probe_small] > 100)
            sector_small[current_probe_small] = stable;
        if (sector_small[current_probe_small] == 100)
            sector_small[current_probe_small] = stable;

        for (unsigned short i = 0; i< buff_size;i++)
        {
            probes[current_probe] += sector_small[i];
        }
        probes [current_probe] /= buff_size + 1;

        if (core_step % 10 == 0)
        {
           current_probe ++;

           for (unsigned short i = 0; i<buff_size;i++)
           {
               load += probes [i];
           }

           load /= buff_size + 1;

           if (current_probe == buff_size)
               current_probe = 0;
        }


    }
    else
        load = function;
}

void unital::get_load( unsigned short function )
{
    if (buffered)
    {
        current_probe_small ++;

        if (current_probe_small == buff_size)
            current_probe_small = 0;

        sector_small[current_probe_small] = function;

        if (sector_small[current_probe_small] > 100)
            sector_small[current_probe_small] = stable;
        if (sector_small[current_probe_small] == 100)
            sector_small[current_probe_small] = stable;

        for (unsigned short i = 0; i< buff_size;i++)
        {
            probes[current_probe] += sector_small[i];
        }

        probes [current_probe] /= buff_size + 1;

        if (core_step % 10 == 0)
        {
            current_probe ++;

            for (unsigned short i = 0; i<buff_size;i++)
            {
                value += probes [i];
            }
            value /= buff_size + 1;

            if (current_probe == buff_size)
                current_probe = 0;
        }
    }
    else
        value = function;
}

bool Core::wake_up_prepare()
{

        wake_up == false;
        eyes->anims_send ( "slp_10", "slp_10_close", "slp_10_open", 0, 0);
        bulwers.outline = 21;
        if (get_time ().day != 7)
        {
            if (times.value >= 7 )
            {
                wake_up = true;
                bulwers.outline = 3;
            }
            else
                energy.start = 10;
        }
        else
        {

            if (times.value >= 10 )
            {
                energy.start += 4*3600;
                wake_up = true;
                bulwers.outline = 3;
            }
            else if (times.value >= 7)
                energy.start = 16;
            else
                energy.start = 10;
        }
        if (!wake_up)
            sleep (5);
}

void eyes_view::graphics_prepare()
{
    if (images_ready)
    {
    if (bulwers.eye == 1)
        send_eyes ( "eye_01" );
    if (bulwers.eye == 2)
        send_eyes ( "eye_02" );
    if (bulwers.eye == 3)
        send_eyes ( "eye_03" );
    if (bulwers.eye == 4)
        send_eyes ( "eye_04" );
    if (bulwers.eye == 5)
        send_eyes ( "eye_05" );
    if (bulwers.eye == 6)
        send_eyes ( "eye_06" );
    if (bulwers.eye == 7)
        send_eyes ( "eye_07" );
    if (bulwers.eye == 8)
        send_eyes ( "eye_08" );
    if (bulwers.eye == 9)
        send_eyes ( "eye_09" );
    if (bulwers.eye == 10)
        send_eyes ( "eye_10" );



    if (bulwers.outline == 0)
    {
        int tmp = rand () % 3;
        if (tmp == 0)
            face_send = "cusual_01";
        if (tmp == 1)
            face_send = "bul_01";
        if (tmp == 2)
            face_send = "bul_02";
        if (tmp == 3)
            face_send = "bul_03";
    }
    else
    {
        if (bulwers.outline == 1)
        {
            int tmp = rand () % 3 + 1;
            if (tmp == 1)
                face_send = "slp_01";
            if (tmp == 2)
                face_send = "slp_02";
            if (tmp == 3)
                face_send = "slp_03";
            if (tmp == 4)
                face_send = "slp_04";
        }
        if (bulwers.outline == 2)
        {
            int tmp = rand () % 1 + 4;
            if (tmp == 4)
                face_send = "slp_04";
            if (tmp == 5)
                face_send = "slp_05";
        }
        if (bulwers.outline == 3)
        {
            int tmp = rand () % 5 + 1;
            if (tmp == 5)
                face_send = "slp_05";
            if (tmp == 6)
                face_send = "slp_06";
        }

        if (bulwers.outline == 4)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "cusual_01";
            if (tmp == 1)
                face_send = "bul_01";
            if (tmp == 2)
                face_send = "bul_02";
        }
        if (bulwers.outline == 5)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_01";
            if (tmp == 1)
                face_send = "bul_02";
            if (tmp == 2)
                face_send = "bul_03";
        }
        if (bulwers.outline == 6)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_02";
            if (tmp == 1)
                face_send = "bul_03";
            if (tmp == 2)
                face_send = "bul_04";
        }
        if (bulwers.outline == 7)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_03";
            if (tmp == 1)
                face_send = "bul_04";
            if (tmp == 2)
                face_send = "bul_05";
        }
        if (bulwers.outline == 8)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_04";
            if (tmp == 1)
                face_send = "bul_05";
            if (tmp == 2)
                face_send = "bul_06";
        }
        if (bulwers.outline == 9)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_05";
            if (tmp == 1)
                face_send = "bul_06";
            if (tmp == 2)
                face_send = "bul_07";
        }
        if (bulwers.outline == 10)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_06";
            if (tmp == 1)
                face_send = "bul_07";
            if (tmp == 2)
                face_send = "bul_08";
        }
        if (bulwers.outline == 11)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_07";
            if (tmp == 1)
                face_send = "bul_08";
            if (tmp == 2)
                face_send = "bul_09";
        }
        if (bulwers.outline == 12)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_08";
            if (tmp == 1)
                face_send = "bul_09";
            if (tmp == 2)
                face_send = "bul_10";
        }
        if (bulwers.outline == 13)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_09";
            if (tmp == 1)
                face_send = "bul_10";
            if (tmp == 2)
                face_send = "bul_11";
        }
        if (bulwers.outline == 14)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_10";
            if (tmp == 1)
                face_send = "bul_11";
            if (tmp == 2)
                face_send = "bul_12";
        }
        if (bulwers.outline == 15)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_11";
            if (tmp == 1)
                face_send = "bul_12";
            if (tmp == 2)
                face_send = "bul_13";
        }
        if (bulwers.outline == 16)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_12";
            if (tmp == 1)
                face_send = "bul_13";
            if (tmp == 2)
                face_send = "bul_14";
        }
        if (bulwers.outline == 17)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_13";
            if (tmp == 1)
                face_send = "bul_14";
            if (tmp == 2)
                face_send = "bul_15";
        }
        if (bulwers.outline == 18)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_14";
            if (tmp == 1)
                face_send = "bul_15";
            if (tmp == 2)
                face_send = "bul_16";
        }
        if (bulwers.outline == 19)
        {
            int tmp = rand () % 1;
            if (tmp == 0)
                face_send = "bul_15";
            if (tmp == 1)
                face_send = "bul_16";
        }


    }


    int anim_num_1 = 0;
    int anim_num_2 = 0;

           if (s_anim.face_prev == "bul_16" ||
               s_anim.face_prev == "sh_01"  ||
               s_anim.face_prev == "slp_10" ||
               s_anim.face_prev == "cusual_01" ||
               s_anim.face_prev == "bul_01" ||
               s_anim.face_prev == "bul_02" ||
               s_anim.face_prev == "bul_03" ||
               s_anim.face_prev == "bul_04" ||
               s_anim.face_prev == "bul_05" ||
               s_anim.face_prev == "bul_06" ||
               s_anim.face_prev == "bul_07" ||
               s_anim.face_prev == "bul_08" ||
               s_anim.face_prev == "bul_09" ||
               s_anim.face_prev == "bul_10" ||
               s_anim.face_prev == "bul_11" ||
               s_anim.face_prev == "bul_12" ||
               s_anim.face_prev == "bul_13" ||
               s_anim.face_prev == "bul_14" ||
               s_anim.face_prev == "bul_15" ||
               s_anim.face_prev == "slp_01" ||
               s_anim.face_prev == "slp_02" ||
               s_anim.face_prev == "slp_03" ||
               s_anim.face_prev == "slp_04" ||
               s_anim.face_prev == "slp_05" ||
               s_anim.face_prev == "slp_06" )
               anim_num_1 = 0;


               anim_num_2 = 0;

           if (face_send == "bul_16" ||
               face_send == "slp_10" )
               anim_num_2 = 0;
           if (face_send == "bul_09" ||
               face_send == "bul_10" ||
               face_send == "bul_11" ||
               face_send == "bul_12" ||
               face_send == "bul_13" ||
               face_send == "bul_14" ||
               face_send == "bul_15" )
               anim_num_2 = 5;
           if (face_send == "cusual_01" ||
               face_send == "bul_01" ||
               face_send == "bul_02" ||
               face_send == "bul_03" ||
               face_send == "bul_04" ||
               face_send == "slp_01" ||
               face_send == "slp_02" ||
               face_send == "slp_03" ||
               face_send == "slp_04" ||
               face_send == "slp_05" )
               anim_num_2 = 4;
           if (face_send == "bul_05" ||
               face_send == "bul_06" ||
               face_send == "bul_07" ||
               face_send == "bul_08" ||
               face_send == "slp_06" )
               anim_num_2 = 3;




           if (face_send == "")
               face_send = "slp_10";
           if (s_anim.face_prev == "")
               s_anim.face_prev = "slp_10";


            //info << "core pics settings begin\n";

           anims_send (face_send, s_anim.face_prev + "_close", face_send + "_open", anim_num_1, anim_num_2);

           if (bulwers.outline == 20 && bulwers.prev_outline != 20)
           {
               face_send = "sh_01";
               anims_send (face_send, s_anim.face_prev + "_close", "sh_02_open", anim_num_1, 7);
           }

           if (bulwers.outline != 20 && bulwers.prev_outline == 20)
           {
               face_send = "cusual_01";
               anims_send (face_send, "sh_01_close", "cusual_01_open", 0, 4);
           }
           if (bulwers.outline == 20 && bulwers.prev_outline == 20)
           {
               face_send = "sh_01";
               anims_send (face_send, "sh_02_close", "sh_01_open", 0, 0);
           }

           if (bulwers.outline == 21)
           {
               face_send = "slp_10";
               anims_send ("slp_10", s_anim.face_prev + "_close", "slp_10_open", anim_num_1, 0);
           }

           s_anim.face_prev = face_send;
       }
}

void Core::bulwers_update ()
{
    if (eMu.cpu)
    {
        cpu.get_load(eMu.cpu_val);
    }
    else
    {
        cpu.get_load(HRDWR.C_LOAD());
    }
    if (eMu.mem)
    {
        memory.get_load(eMu.mem_val);
    }
    else
    {
        memory.get_load(HRDWR.M_LOAD ());
    }
    if (eMu.temp)
    {
        temperature.get_load(eMu.temp_val);
    }
    else
    {
        temperature.get_load(HRDWR.temperatura());
    }
    if (eMu.batt)
    {
        battery.get_load(eMu.batt_val);
    }
    else
    {
        if (battery_state != 1 && battery_state != 3 && battery_state != 0)
            battery.get_load(HRDWR.bateria());
        else if (battery_state == 0)
            battery.load = 0;
        else
            battery.load = 100;
    }
    if (eMu.time)
    {
        times.value = eMu.time_val;
    }
    else
    {
        times.value = get_time ().hour/3600;
    }
    if (eMu.energy)
    {
        energy.value = eMu.energy_val;
    }
    else
    {
        energy.value ++;
    }
    if (eMu.batt_s)
    {
        battery_state = eMu.batt_s_val;
    }
    else
    {
        battery_state = HRDWR.bat_plugged ();
    }


cpu.mod = cpu.calculate();
memory.mod = memory.calculate();
battery.mod = battery.calculate();
temperature.mod = temperature.calculate();
times.mod = times.calculate();
energy.mod = energy.calculate();
mousea.mod = mousea.impact*mousea.convert()/100;
bulwers.update();

//if (autocalc.enabled)
//    autocalc_reload ( &cfg );
}

Core::Core ( eyes_view * neyes )
{
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
    connect ( neyes, SIGNAL ( mousemoved (int,int) ), this, SLOT ( handle_mouse(int,int) ) );
    eyes = neyes;
}

void Core::autocalc_init ()
{
    autocalc.c_cpu = cpu.stable;
    autocalc.c_mem = memory.stable;
    autocalc.c_temp = temperature.stable;
    autocalc.save_next = autocalc.save_interval;
}

void Core::autocalc_reload ( Configuration * cfg )
{
    autocalc.c_cpu = (((100/autocalc.impact)-1)*cpu.stable + cpu.load)/(100/autocalc.impact);
    autocalc.c_mem = (((100/autocalc.impact)-1)*memory.stable + memory.load)/(100/autocalc.impact);
    autocalc.c_temp = (((100/autocalc.impact)-1)*temperature.stable + temperature.value)/(100/autocalc.impact);

    if (autocalc.save_next == 0)
    {
        //zrzut wartości
        cpu.stable = cfg->lookupValue ( "core.cpu.stable", 0);
        info << "Dropping stable values\n";
    }
}

void Core::load_config ()
{
    Configuration * cfg = Configuration::getInstance ();

    cpu.frequency           = cfg->lookupValue ( "core.cpu.frequency",                  'f'         );
    cpu.lin_num             = cfg->lookupValue ( "core.cpu.linear_modifier",            0           );
    cpu.stable              = cfg->lookupValue ( "core.cpu.stable",                     25          );
    cpu.steps               = cfg->lookupValue ( "core.cpu.steps",                      10          );
    cpu.loseless            = cfg->lookupValue ( "core.cpu.adaptation",                 10          );
    cpu.buffered            = cfg->lookupValue ( "core.cpu.buffered",                   true        );
    cpu.buff_size           = cfg->lookupValue ( "core.cpu.buffer_size",                10          );

    //mem_section

    memory.frequency        = cfg->lookupValue ( "core.memory.frequency",               'q'         );
    memory.lin_num          = cfg->lookupValue ( "core.memory.linear_modifier",         2           );
    memory.stable           = cfg->lookupValue ( "core.memory.stable",                  25          );
    memory.steps            = cfg->lookupValue ( "core.memory.steps",                   8           );
    memory.loseless         = cfg->lookupValue ( "core.memory.adaptation",              10          );
    memory.buffered         = cfg->lookupValue ( "core.memory.buffered",                true        );
    memory.buff_size        = cfg->lookupValue ( "core.memory.buffer_size",             10          );

    //temperature_section

    temperature.frequency   = cfg->lookupValue ( "core.temperature.frequency",          'q'         );
    temperature.lin_num     = cfg->lookupValue ( "core.temperature.linear_modifier",    2           );
    temperature.stable      = cfg->lookupValue ( "core.temperature.stable",             56          );
    temperature.steps       = cfg->lookupValue ( "core.temperature.steps",              12          );
    temperature.loseless    = cfg->lookupValue ( "core.temperature.adaptation",         2           );
    temperature.buffered    = cfg->lookupValue ( "core.temperature.buffered",           true        );
    temperature.buff_size   = cfg->lookupValue ( "core.temperature.buffer_size",        10          );
    temperature.unit        = cfg->lookupValue ( "core.temperature.unit",               1           );

    //battery_section

    battery.frequency       = cfg->lookupValue ( "core.battery.frequency",              'l'         );
    battery.lin_num         = cfg->lookupValue ( "core.battery.linear_modifier",        0           );
    battery.stable          = cfg->lookupValue ( "core.battery.stable",                 25          );
    battery.steps           = cfg->lookupValue ( "core.battery.steps",                  8           );
    battery.loseless        = cfg->lookupValue ( "core.battery.adaptation",             10          );
    battery.buffered        = cfg->lookupValue ( "core.battery.buffered",               false       );
    battery.buff_size       = cfg->lookupValue ( "core.battery.buffer_size",            10          );

    //times_sector

    times.frequency         = cfg->lookupValue ( "core.times.frequency",                'q'         );
    times.lin_num           = cfg->lookupValue ( "core.times.quad_modifier",            2           );
    times.start             = cfg->lookupValue ( "core.times.start",                    20          );
    times.steps             = cfg->lookupValue ( "core.times.steps",                    6           );
    times.end               = cfg->lookupValue ( "core.times.end",                      6           );
    times.wide              = cfg->lookupValue ( "core.times.wide",                     6           );

    //energy_sector

    energy.frequency        = cfg->lookupValue ( "core.energy.frequency",               'q'         );
    energy.lin_num          = cfg->lookupValue ( "core.energy.quad_modifier",           2           );
    energy.start            = cfg->lookupValue ( "core.energy.start",                   16          );
    energy.steps            = cfg->lookupValue ( "core.energy.steps",                   6           );
    energy.end              = cfg->lookupValue ( "core.energy.end",                     0           );
    energy.wide             = cfg->lookupValue ( "core.energy.wide",                    6           );

    //bulwers_walls_sector

    bulwers.wall_01         = cfg->lookupValue ("core.bulwers.wall_01",                 300         );
    bulwers.wall_02         = cfg->lookupValue ("core.bulwers.wall_02",                 500         );
    bulwers.wall_03         = cfg->lookupValue ("core.bulwers.wall_03",                 800         );
    bulwers.wall_04         = cfg->lookupValue ("core.bulwers.wall_04",                 1300        );
    bulwers.wall_05         = cfg->lookupValue ("core.bulwers.wall_05",                 2100        );
    bulwers.wall_06         = cfg->lookupValue ("core.bulwers.wall_06",                 3400        );
    bulwers.wall_07         = cfg->lookupValue ("core.bulwers.wall_07",                 5500        );
    bulwers.wall_08         = cfg->lookupValue ("core.bulwers.wall_08",                 8900        );
    bulwers.wall_09         = cfg->lookupValue ("core.bulwers.wall_09",                 14400       );
    bulwers.wall_10         = cfg->lookupValue ("core.bulwers.wall_10",                 23300       );
    bulwers.wall_11         = cfg->lookupValue ("core.bulwers.wall_11",                 37700       );
    bulwers.wall_12         = cfg->lookupValue ("core.bulwers.wall_12",                 61600       );
    bulwers.wall_13         = cfg->lookupValue ("core.bulwers.wall_13",                 98700       );
    bulwers.wall_14         = cfg->lookupValue ("core.bulwers.wall_14",                 159700      );
    bulwers.wall_15         = cfg->lookupValue ("core.bulwers.wall_15",                 258400      );
    bulwers.fship_at_calm   = cfg->lookupValue ("core.bulwers.friendship_autocalm_del", 3600        );
    bulwers.friendship      = cfg->lookupValue ("core.bulwers.friendship",              0           );
    bulwers.calm_perc       = cfg->lookupValue ("core.bulwers.fship_calm_percentage",   1           );

    //basic_sector

    cdebug                  = cfg->lookupValue("debug.cdebug.on",                       true        );

    //autocalc_sector

    autocalc.enabled        = cfg->lookupValue("core.autocalc.enabled",                 true        );
    autocalc.save_interval  = cfg->lookupValue("core.autocalc.interval",                300         );
    autocalc.impact         = cfg->lookupValue("core.autocalc.impact",                  20          );

    //eMu_sector

    eMu.cpu                 = cfg->lookupValue ("core.eMu_zone.cpu",                    false       );
    eMu.cpu_val             = cfg->lookupValue ("core.eMu_zone.cpu_val",                0           );
    eMu.mem                 = cfg->lookupValue ("core.eMu_zone.mem",                    false       );
    eMu.mem_val             = cfg->lookupValue ("core.eMu_zone.mem_val",                0           );
    eMu.temp                = cfg->lookupValue ("core.eMu_zone.temp",                   false       );
    eMu.temp_val            = cfg->lookupValue ("core.eMu_zone.temp_val",               0           );
    eMu.batt                = cfg->lookupValue ("core.eMu_zone.batt",                   false       );
    eMu.batt_val            = cfg->lookupValue ("core.eMu_zone.batt_val",               0           );
    eMu.batt_s              = cfg->lookupValue ("core.eMu_zone.batt_s",                 false       );
    eMu.batt_val            = cfg->lookupValue ("core.eMu_zone.batt_s_val",             0           );
    eMu.time                = cfg->lookupValue ("core.eMu_zone.time",                   false       );
    eMu.time_val            = cfg->lookupValue ("core.eMu_zone.time_val",               0           );
    eMu.energy              = cfg->lookupValue ("core.eMu_zone.energy",                 false       );
    eMu.energy_val          = cfg->lookupValue ("core.eMu_zone.energy_val",             0           );

    //mousea_actions_sector

    mousea.buff_size        = cfg->lookupValue ("core.mouse_actions.buff_size",         120         );
    mousea.goodstep         = cfg->lookupValue ("core.mouse_actions.goodstep",          1           );
    mousea.badstep          = cfg->lookupValue ("core.mouse_actions.badstep",           10          );
    mousea.scale            = cfg->lookupValue ("core.mouse_actions.scale",             8000        );
    mousea.wall             = cfg->lookupValue ("core.mouse_actions.wall",              500         );
    mousea.force_wall       = cfg->lookupValue ("core.mouse_actions.force_wall",        3000        );
    mousea.opt_speed        = cfg->lookupValue ("core.mouse_actions.opt_speed",         250         );
    mousea.impact           = cfg->lookupValue ("core.mouse_actions.impact",            10          );
    mousea.hit_time_multi   = cfg->lookupValue ("core.mouse_actions.hit_multi",         2           );
    mousea.heavycalm        = cfg->lookupValue ("core.mouse_actions.heavycalm",         100         );
    mousea.max_delay        = cfg->lookupValue ("core.mouse_actions.max_delay",         400         );

    // hardware_section

    HRDWR.battery_capacity  = cfg->lookupValue ( "core.hardware.batt_capacity",         4700        );
    HRDWR.special_batname   = cfg->lookupValue ( "core.hardware.use_batname",           false       );
    HRDWR.special_thername  = cfg->lookupValue ( "core.hardware.use_thername",          false       );
    HRDWR.cfg_battname      = cfg->lookupValue ( "core.hardware.battname",              "BAT0"      );
    HRDWR.cfg_thername      = cfg->lookupValue ( "core.hardware.thername",              "thermal_zone0"        );

}

void Core::run ()
{
    info << "(core) starting core.\n";
    bulwers_init ();
    info << "(core) bulwers inited\n";
    if (autocalc.enabled)
    {
        autocalc_init ();
        info << "autocalc started\n";
    }
    info << "(core) gui inited\n";
    s_anim.face_prev = "slp_10";
    info << "s_anim.face_prev set to " << s_anim.face_prev.toStdString() << "\n";
    do
    {
        info << "Is in wake up\n";
        times.value = get_time ().hour/3600;

        // TODO 03 : It PROPABLY won't work correctly and it should works on config values.

        wake_up_prepare();
        eyes->anims_reload();
    } while (!wake_up);
    info << "(core) wake up finished\n";
    eyes->anims_send ("cusual_01", "slp_10_close", "cusual_01_open", 0, 4);
    eyes->anims_reload();
    HRDWR.system_check();
    info << "(core) end of core preparing\n";
    timer->start ( 1000 );
    _cdbg = new cdbg ( this );
}

void Core::on_timer_tick ()
{
    core_step ++;
    bulwers_update ();
    eyes->graphics_prepare();
}

void Core::handle_mouse ( int x, int y )
{
    if (mousea.cur > mousea.buff_size)
    {
        mousea.cur = 0;
    }

    long double t_result = mousetime.elapsed();

    if (t_result < mousea.max_delay && t_result > 1)
    {
        mousea.buffer[mousea.cur] = 1000*(long double)sqrt((mousea.prev_x - x)*(mousea.prev_x - x) + (mousea.prev_y - y)*(mousea.prev_y - y))/t_result;
    }
    else
        mousea.buffer[mousea.cur] = 0;

    if (mousea.buffer[mousea.cur] > mousea.force_wall)
        info << "You\'ve hit Eyes. Be carefull!\n";

    //1500
    //3000

    //cout << mousea.buffer[mousea.cur] << "\n";
    //cout << mousea.prev_x << " " << mousea.prev_y << "\n";
    //cout << x << " " << y << "\n";
    //cout << (long double)sqrt((mousea.prev_x - x)*(mousea.prev_x - x) + (mousea.prev_y - y)*(mousea.prev_y - y)) << "\n";
    //cout << t_result << "\n";
    //cout << mousea.buffer[mousea.cur] << "\n";
    //cout << mousetime.elapsed() << "\n";

    mousea.prev_x = x;
    mousea.prev_y = y;
    mousea.cur++;
    mousetime.start();
    mousetime.restart();
}

int mouse_actions::convert()
{
    int sum = 0;
    tr = 0;
    for (int i = 0; i <= buff_size; i++)
    {
        if (buffer[i] != 0)
        {
            sum+=buffer[i];
            buffer[i] = 0;
            tr++;
        }
    }
    if (tr == 0)
        return 0;
    else
    {
        result = sum/tr;
        if (result > 0 )
        {
            if (result >= wall*multiplier)
            {
                bulwers.friendship-=(badstep*hit_time);
                hit_time*=hit_time_multi;
                return -(100*result/scale);
            }
            else
            {
                if (bulwers.friendship > -(int)heavycalm*(int)goodstep)
                {
                    bulwers.friendship+=goodstep;
                }
                else
                    bulwers.friendship-=bulwers.friendship/heavycalm;

                if( hit_time != 1 )
                    hit_time--;

                if ( result > opt_speed )
                {
                    return (100*((wall*multiplier)-result)/((wall*multiplier)-opt_speed));
                }
                else
                {
                    return (100*result/opt_speed);
                }
            }
        }
    }
}

void Core::handle_enter ()
{
    info << "(core) mouse entered\n";
}
