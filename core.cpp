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

#include <iostream>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>
#include <qstring.h>
#include <qvector.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "core.hxx"
#include "eyes.hxx"
#include "camera.hxx"
#include "debug.hxx"
#include "cdbg.hxx"
#include "hardware.hxx"
#include "defines.hxx"

using namespace std;

unsigned short  battery_state;
unsigned short  mod_bat_plug;
unsigned int    prev_bat_plug;
bool            once_plugged;
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
friendship      fship;
rootcontrol     rtctrl;

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
    s_anim.face_prev = face_next;

    //info << "sended pics:\n" << s_anim.open.toStdString() << " - open anim\n"
    //     << s_anim.close.toStdString() << " - close anim\n"
    //     << "start/end " << s_anim.start << " " << s_anim.end
    //     << "\nface_next = " << face_next.toStdString() << "\n\n";
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

void bul::wake_up_chk()
{
    if (wake_up)
        return;
    no_update = true;
    bulwers.outline = 21;
    if (get_time ().day != 7)
    {
        if (times.value >= wkup_time )
        {
            energy.start = nrg_std*3600;
            wake_up = true;
            wkup_reason = 1;
        }
        else
            energy.start = nrg_low*3600;
    }
    else
    {
        if (times.value >= wkup_timew )
        {
            energy.start = nrg_boost*3600;
            wake_up = true;
            wkup_reason = 1;
        }
        else if (times.value >= wkup_time)
            energy.start = nrg_std*3600;
        else
            energy.start = nrg_low*3600;
    }
    //if (!wake_up)
    //    sleep (5);
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
            fship.value/100 -
            mousea.mod      ;

    //total_mod = 0;
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

    if (eMu.bulwers)
        value = eMu.bulwers_val;

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
}

void bul::flue_check()
{
    if (!flue && core_step > 2*temperature.buff_size)
    {
        if ((double)temperature.value < fluelowval)
            fluelowval=temperature.value;
        if ((double)temperature.value > fluehighval)
            fluehighval = temperature.value;
        fluehighval-=(fluehighval-(double)temperature.stable)/(double)flueimpact;
        fluelowval+=((double)temperature.stable-fluelowval)/(double)flueimpact;
        if (fluehighval - fluelowval > (double)flueamplitude)
            flue = true;
    }
    else
    {
        if (fluetimer > 3*fluestepdelay)
        {
            hot = 3;
            shy = 3;
            energy.value+=6;
            if (value < 12)
                value = 12;
        }
        else if (fluetimer > 2*fluestepdelay)
        {
            hot = 2;
            shy = 2;
            fluetimer++;
            energy.value+=2;
            if (value < 8)
                value = 8;
        }
        else if (fluetimer > fluestepdelay)
        {
            hot = 1;
            shy = 1;
            fluetimer++;
            energy.value++;
            if (value < 6)
                value = 6;
        }
        else
            fluetimer++;
    }
}

void bul::critical_services( Configuration * cfg )
{
    mousea.multiplier = 1;//= (double)value/10.0;
    if (mousea.hit_time > 2 && core_step%10 == 0)
        mousea.hit_time--;

    if (core_step%fship.calm_timer == 0)
    {
        fship.timeimpact(1);
        fship.to_save = 1;
    }

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

    if (value != 0 && outline != 20 && outline != 21)
        outline = value + 3;
    if (outline != 20 && value == 0)
        outline = 0;


    //TODO 01: It must works on cfg values, not static.
    wkup_active = 0;
    if (get_time ().day != 7)
    {
        if (times.value < timelow_1 || times.value > timehigh_1 || energy.value > energy.start + energy.wide - 5 || fluetimer > fluestepdelay)
        {
            wkup_active = 1;
            tired = 1;

            if (outline <= 7)
                outline = 1;

            if (outline > 7 && outline < 11)
            {
                if ((rand () % 2))
                    outline = 1;
            }
        }
        if (times.value < timelow_2 || times.value > timehigh_2 || energy.value > energy.start + energy.wide - 3 || fluetimer > 2*fluestepdelay)
        {
            wkup_active = 1;
            tired = 2;
            if (outline <= 8)
                outline = 1;

            if (outline > 8 && outline < 12)
            {
                if ((rand () % 2))
                    outline = 2;
            }
        }
        if (times.value < timelow_3 || times.value > timehigh_3 || energy.value > energy.start + energy.wide - 1 || fluetimer > 3*fluestepdelay )
        {
            wkup_active = 1;
            tired = 3;
            if (outline <= 9)
                outline = 1;

            if (outline > 9 && outline < 13)
            {
                if ((rand () % 2))
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
        if (times.value < timelow_1w || times.value > timehigh_1w ||  energy.value > energy.start + energy.wide - 5*3600 || fluetimer > fluestepdelay)
        {
            wkup_active = 1;
            tired = 1;
            if (outline <= 7)
                outline = 1;

            if (outline > 7 && outline < 11)
            {
                if ((rand () % 2))
                    outline = 1;
            }
        }
        if (times.value < timelow_2w || times.value > timehigh_2w ||  energy.value > energy.start + energy.wide - 4*3600 || fluetimer > 2*fluestepdelay)
        {
            wkup_active = 1;
            tired = 2;
            if (outline <= 8)
                outline = 2;

            if (outline > 8 && outline < 12)
            {
                if ((rand () % 2))
                    outline = 2;
            }
        }
        if (times.value < timelow_3w || times.value > timehigh_3w ||  energy.value > energy.start + energy.wide - 3*3600 || fluetimer > 3*fluestepdelay)
        {
            wkup_active = 1;
            tired = 3;
            if (outline <= 9)
                outline = 1;

            if (outline > 9 && outline < 13)
            {
                if ((rand () % 2))
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
    if(current_wkup_delay > 0)
    {
        current_wkup_delay--;
        if (100*current_wkup_delay/wake_up_delay > 60 && outline < 3)
        {
            outline = 3;
            tired = 3;
            if (wkup_active == 1)
                wkup_active = 3;
            else
                wkup_active = 2;
        }
        else if (100*current_wkup_delay/wake_up_delay > 30 && outline < 2)
        {
            outline = 2;
            tired = 2;
            if (wkup_active == 1)
                wkup_active = 3;
            else
                wkup_active = 2;
        }
        else if (outline < 1)
        {
            outline = 1;
            tired = 1;
            if (wkup_active == 1)
                wkup_active = 3;
            else
                wkup_active = 2;
        }
    }
    if (battery_state == 0)
    {
        outline = 20;
        eye = 10;
    }
    else if (outline == 20)
        outline = 0;

    unsigned int atime = get_time().day_num*24*60 + get_time().hour/60;
    unsigned int dtime = 0;
    bool recover = false;
    if (remembered_time > atime)
    {
        dtime = (24*60 - (remembered_time - (remembered_time%(24*60)))) + atime;
        recover = true;
    }
    if (remembered_time < atime-max_mem_lag)
    {
        dtime = atime - remembered_time;
        recover = true;
    }
    if (recover)
    {
        if (get_time().day != 7)
        {
            unsigned int dtperc = 100*dtime/(rest_time_std*60);
            if (dtperc > 100)
                dtperc = 100;
            if (energy.start > dtperc*nrg_std*3600/100 + remembered_nrg)
                energy.start = dtperc*nrg_std*3600/100 + remembered_nrg;
        }
        else
        {
            unsigned int dtperc = 100*dtime/(rest_time_wkend*60);
            if (dtperc > 100)
                dtperc = 100;
            if (dtperc < 100)
            {
                if (energy.start >= nrg_std*3600 + remembered_nrg)
                    energy.start = nrg_std*3600 + remembered_nrg;
                else if (energy.start > dtperc*(nrg_std)*3600/100 + remembered_nrg)
                    energy.start = dtperc*(nrg_std)*3600/100 + remembered_nrg;
            }
        }
    }
    remembered_time = atime;
    remembered_nrg = energy.start - energy.value;

    //saving

    cfg->setValue("core.bulwers.remembered_time", (int)remembered_time);
    cfg->setValue("core.bulwers.remembered_energy", (int)remembered_nrg);
    if (fship.to_save)
        cfg->setValue("core.friendship.value", (int)fship.value);

    rtctrl.action("battery");
    rtctrl.action("temperature");
}

void Core::bulwers_init ()
{
    HRDWR.pid = getpid();

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
    mousea.hpp_active                   = false;

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
    bulwers.wake_up                     = false;
    bulwers.no_update                   = false;
    bulwers.wkup_active                 = 0    ;
    bulwers.wkup_reason                 = 0    ;
    bulwers.current_wkup_delay          = bulwers.wake_up_delay;
    bulwers.flue                        = false;
    bulwers.fluetimer                   = 0    ;
    bulwers.fluehighval                 = temperature.stable;
    bulwers.fluelowval                  = temperature.stable;
    if (bulwers.remembered_time == 0)
        bulwers.remembered_time         = get_time().day_num*60*24 + get_time().hour/60;

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
            sector_small[current_probe_small] = 99;

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
            sector_small[current_probe_small] = 99;

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
        int tmp = rand () % 4;
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
            int tmp = rand () % 4;
            if (tmp == 0)
                face_send = "slp_01";
            if (tmp == 1)
                face_send = "slp_02";
            if (tmp == 2)
                face_send = "slp_03";
            if (tmp == 3)
                face_send = "slp_04";
        }
        if (bulwers.outline == 2)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "slp_04";
            if (tmp == 1)
                face_send = "slp_05";
        }
        if (bulwers.outline == 3)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "slp_05";
            if (tmp == 1)
                face_send = "slp_06";
        }

        if (bulwers.outline == 4)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "cusual_01";
            if (tmp == 1)
                face_send = "bul_01";
            if (tmp == 2)
                face_send = "bul_02";
        }
        if (bulwers.outline == 5)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_01";
            if (tmp == 1)
                face_send = "bul_02";
            if (tmp == 2)
                face_send = "bul_03";
        }
        if (bulwers.outline == 6)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_02";
            if (tmp == 1)
                face_send = "bul_03";
            if (tmp == 2)
                face_send = "bul_04";
        }
        if (bulwers.outline == 7)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_03";
            if (tmp == 1)
                face_send = "bul_04";
            if (tmp == 2)
                face_send = "bul_05";
        }
        if (bulwers.outline == 8)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_04";
            if (tmp == 1)
                face_send = "bul_05";
            if (tmp == 2)
                face_send = "bul_06";
        }
        if (bulwers.outline == 9)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_05";
            if (tmp == 1)
                face_send = "bul_06";
            if (tmp == 2)
                face_send = "bul_07";
        }
        if (bulwers.outline == 10)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_06";
            if (tmp == 1)
                face_send = "bul_07";
            if (tmp == 2)
                face_send = "bul_08";
        }
        if (bulwers.outline == 11)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_07";
            if (tmp == 1)
                face_send = "bul_08";
            if (tmp == 2)
                face_send = "bul_09";
        }
        if (bulwers.outline == 12)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_08";
            if (tmp == 1)
                face_send = "bul_09";
            if (tmp == 2)
                face_send = "bul_10";
        }
        if (bulwers.outline == 13)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_09";
            if (tmp == 1)
                face_send = "bul_10";
            if (tmp == 2)
                face_send = "bul_11";
        }
        if (bulwers.outline == 14)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_10";
            if (tmp == 1)
                face_send = "bul_11";
            if (tmp == 2)
                face_send = "bul_12";
        }
        if (bulwers.outline == 15)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_11";
            if (tmp == 1)
                face_send = "bul_12";
            if (tmp == 2)
                face_send = "bul_13";
        }
        if (bulwers.outline == 16)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_12";
            if (tmp == 1)
                face_send = "bul_13";
            if (tmp == 2)
                face_send = "bul_14";
        }
        if (bulwers.outline == 17)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_13";
            if (tmp == 1)
                face_send = "bul_14";
            if (tmp == 2)
                face_send = "bul_15";
        }
        if (bulwers.outline == 18)
        {
            int tmp = rand () % 3;
            if (tmp == 0)
                face_send = "bul_14";
            if (tmp == 1)
                face_send = "bul_15";
            if (tmp == 2)
                face_send = "bul_16";
        }
        if (bulwers.outline == 19)
        {
            int tmp = rand () % 2;
            if (tmp == 0)
                face_send = "bul_15";
            if (tmp == 1)
                face_send = "bul_16";
        }
    }

    if (bulwers.tired == 0)
        toggle_layer(SLEEPY, false);
    else if (bulwers.tired == 1)
    {
        toggle_layer(SLEEPY, true);
        set_layer(SLEEPY, "tired_01");
    }
    else if (bulwers.tired == 2)
    {
        toggle_layer(SLEEPY, true);
        set_layer(SLEEPY, "tired_02");
    }
    else if (bulwers.tired == 3)
    {
        toggle_layer(SLEEPY, true);
        set_layer(SLEEPY, "tired_03");
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
               interrupt();
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

           //cerr << bulwers.outline << " " << face_send.toStdString() << " " << s_anim.face_prev.toStdString() << " " << mousea.hpp_active << "\n";

           if (s_anim.face_prev == "hpp_07")
           {
               if (!mousea.hpp_active)
               {
                   //cerr << "hpp07 N\n";
                   anim_num_1 = 0;
                   if (bulwers.outline <= 4)
                   {
                       anims_send (face_send, "hpp_continue", "hpp_01_open", anim_num_1, 6);
                   }
                   else if (bulwers.outline <= 6)
                   {
                       anims_send (face_send, "hpp_continue", "hpp_02_open", anim_num_1, 5);
                   }
                   else if (bulwers.outline <= 8)
                   {
                       anims_send (face_send, "hpp_continue", "hpp_03_open", anim_num_1, 4);
                   }
                   else
                   {
                       anims_send (face_send, "hpp_continue", "hpp_04_open", anim_num_1, 3);
                   }
                   interrupt();
               }
               else
               {
                   anims_send ("hpp_07", "hpp_continue", "hpp_continue", 0, 0);
                   mousea.hpp_active = false;
               }
           }
           else if (mousea.hpp_active && bulwers.outline <= mousea.max_hpp_bul+3)
           {
               hpp_evoke();
               mousea.hpp_active = false;
           }
           if (mousea.hit_active)
           {
               mousea.hit_active = 0;
               interrupt();
           }
       }
}

void friendship::save(Configuration *cfg)
{
    if (to_save)
        cfg->setValue("core.friendship.value", (int)value);
}

double friendship::funccalc(double angle, unsigned int current)
{
    return pow(current, angle)*func_scale/pow(100, angle);
}

void friendship::timeimpact(unsigned int delay)
{
    long double absval = max_below + value;
    if (value < stable)
    {
        for (int i = 0; i<delay; i++)
        {
            value+=calm_perc_low*(funccalc(func_calm_low, (100*absval)/(max_below+stable)))/100.0;
        }
    }
    else if (value > stable)
    {
        for (int i = 0; i<delay; i++)
        {
            value-=calm_perc_high*(funccalc(func_calm_high, 100*(value-stable)/(max_over-stable)))/100.0;
        }
    }
    //cerr << value << "\n";
}
void friendship::mouseimpact(unsigned int impact)
{
    long double absval = max_below + value;
    if (impact == 0)
    {
        if (value < stable)
        {
            value+=mouse_good*funccalc(func_mouse_low, (100*absval)/(max_below+stable));
        }
        else if (value > stable)
        {
            value+=mouse_good*func_scale-funccalc(func_mouse_low, 100*(value-stable)/(max_over-stable));
        }
        else
            value+=mouse_good;
    }
    else
    {
        for (int i = 0; i < impact; i++)
        {
            value-=pow((double)mouse_bad, func_mouse_hit);
            if (value < -(int)max_below)
                value = -(int)max_below;
        }
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


    cpu.mod = cpu.convert(cpu.calculate());
    memory.mod = memory.convert(memory.calculate());
    battery.mod = battery.convert(battery.calculate());
    temperature.mod = temperature.convert(HRDWR.temperatura());
    times.mod = times.calculate();
    energy.mod = energy.calculate();
    mousea.mod = mousea.impact*mousea.convert()/100;
    bulwers.wake_up_chk();
    if (!bulwers.no_update)
    {
        bulwers.update();
        bulwers.flue_check();
        bulwers.critical_services( Configuration::getInstance () );
    }
    bulwers.no_update = false;

    if (autocalc.enabled)
        autocalc_reload ( Configuration::getInstance () );
}

Core::Core ( eyes_view * neyes )
{
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
    connect ( neyes, SIGNAL ( mousemoved (int,int) ), this, SLOT ( handle_mouse(int,int) ) );
    eyes = neyes;
}

Core::Core ()
{
  timer = new QTimer ( this );
  connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
  eyes = 0;
}

void Core::autocalc_init ()
{
    autocalc.cpu_simple = false;
    autocalc.mem_simple = false;
    autocalc.bat_simple = false;
    autocalc.temp_simple = false;
    autocalc.save_next = autocalc.save_interval;

    if (autocalc.cpu_enabled)
    {
        autocalc.c_cpu = cpu.stable;
        if (cpu.EQsize > 1)
            for (uint i = 0; i<=cpu.EQsize; i++)
            {
                autocalc.cpu_freq.push_back(0);
                autocalc.cpu_curve.push_back(0);
                autocalc.cpu_virtualEQ.push_back((long double)cpu.EQ[i]);
            }
        else
        {
            autocalc.cpu_enabled = false;
            warning << "cpu EQ smaller than 3 brands - disabling cpu autocalc\n";
        }
    }
    if (autocalc.memory_enabled)
    {
        autocalc.c_mem = memory.stable;
        if (memory.EQsize > 1)
            for (uint i = 0; i<=memory.EQsize; i++)
            {
                autocalc.memory_freq.push_back(0);
                autocalc.memory_curve.push_back(0);
                autocalc.memory_virtualEQ.push_back((long double)memory.EQ[i]);
            }
        else
        {
            autocalc.memory_enabled = false;
            warning << "memory EQ smaller than 3 brands - disabling memory autocalc\n";
        }
    }
    if (autocalc.battery_enabled)
    {
        autocalc.c_battery = battery.stable;
        if (battery.EQsize > 1)
            for (int i = 0; i<=battery.EQsize; i++)
            {
                autocalc.battery_freq.push_back(0);
                autocalc.battery_curve.push_back(0);
                autocalc.battery_virtualEQ.push_back((long double)battery.EQ[i]);
            }
        else
        {
            autocalc.battery_enabled = false;
            warning << "battery EQ smaller than 3 brands - disabling battery autocalc\n";
        }
    }
    if (autocalc.temperature_enabled)
    {
        autocalc.c_temp = temperature.stable;
        if (temperature.EQsize > 1)
            for (int i = 0; i<=temperature.EQsize; i++)
            {
                autocalc.temperature_freq.push_back(0);
                autocalc.temperature_curve.push_back(0);
                autocalc.temperature_virtualEQ.push_back((long double)temperature.EQ[i]);
            }
        else
        {
            autocalc.temperature_enabled = false;
            warning << "temperature EQ smaller than 3 brands - disabling temperature autocalc\n";
        }
    }
}

void Core::autocalc_reload ( Configuration * cfg )
{
    if (autocalc.start_delay > 0)
    {
        autocalc.start_delay--;
        return;
    }
    autocalc.save_next--;

    //frequency tracking

    if (autocalc.cpu_enabled)
    {
        autocalc.cpu_curstep = 0;
        autocalc.cpu_perc = 0;
        autocalc.cpu_common = 0.0;
        autocalc.cpu_exoticlow = 100.0;
        autocalc.cpu_exotichigh = 100.0;
        autocalc.cpu_curmid = 0;
        autocalc.cpu_stablepointlow = 0;
        autocalc.cpu_stablepointhigh = 0;

        for (int i = 0; i<=cpu.EQsize; i++)
        {
            if (cpu.load >= (100/cpu.EQsize)*i)
                autocalc.cpu_curstep = i;
            else
                break;
        }
        if (autocalc.cpu_curstep == cpu.EQsize)
            autocalc.cpu_freq[cpu.EQsize]++;
        else
        {
            autocalc.cpu_perc = 100*(cpu.load-(100/cpu.EQsize)*autocalc.cpu_curstep)/(100/cpu.EQsize);
            autocalc.cpu_freq[autocalc.cpu_curstep]+=((long double)autocalc.cpu_perc)/100.0;
            autocalc.cpu_freq[autocalc.cpu_curstep]+=(long double)(100-autocalc.cpu_perc)/100.0;
        }
        for (int i = 0; i<=cpu.EQsize; i++)
        {
            if (autocalc.cpu_freq[i] > autocalc.cpu_common)
            {
                autocalc.cpu_common =autocalc.cpu_freq[i];
                autocalc.cpu_curmid = i;
            }
        }
        if (autocalc.cpu_curmid == 0)
            autocalc.cpu_curmid = 1;
        else if (autocalc.cpu_curmid == cpu.EQsize)
            autocalc.cpu_curmid = cpu.EQsize-1;

        for (int i = 0; i<autocalc.cpu_curmid; i++)
        {
            if (autocalc.cpu_freq[i] < autocalc.cpu_exoticlow )
                autocalc.cpu_exoticlow = autocalc.cpu_freq[i];
        }
        for (int i = autocalc.cpu_curmid+1; i<=cpu.EQsize; i++)
        {
            if (autocalc.cpu_freq[i] < autocalc.cpu_exotichigh )
                autocalc.cpu_exotichigh = autocalc.cpu_freq[i];
        }
        bool correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = 0; i<autocalc.cpu_curmid; i++)
            {
                if (autocalc.cpu_freq[i] > autocalc.cpu_freq[i+1])
                {
                    swap(autocalc.cpu_freq[i], autocalc.cpu_freq[i+1]);
                    correct = false;
                }
            }
        }
        correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = autocalc.cpu_curmid; i<cpu.EQsize; i++)
            {
                if (autocalc.cpu_freq[i] < autocalc.cpu_freq[i+1])
                {
                    swap(autocalc.cpu_freq[i], autocalc.cpu_freq[i+1]);
                    correct = false;
                }
            }
        }
        for (int i = 0; i<autocalc.cpu_curmid; i++)
        {
            if (autocalc.cpu_freq[i] < autocalc.cpu_exoticlow+(long double)(autocalc.cpu_swalll*(autocalc.cpu_common-autocalc.cpu_exoticlow))/100 )
                autocalc.cpu_stablepointlow = i;
        }
        for (int i = autocalc.cpu_curmid+1; i<=cpu.EQsize; i++)
        {
            if (autocalc.cpu_freq[i] > autocalc.cpu_exotichigh+(long double)(autocalc.cpu_swallh*(autocalc.cpu_common-autocalc.cpu_exotichigh))/100 )
                autocalc.cpu_stablepointhigh = i;
        }
        if (autocalc.cpu_stablepointhigh == 0)
            autocalc.cpu_stablepointhigh = autocalc.cpu_curmid+1;
        if (autocalc.auto_cpu)
        {
            autocalc.cpu_freq_angle_low = 1;
            autocalc.cpu_freq_angle_high = 1;
            for (int i = 0; i<autocalc.cpu_curmid; i++)
            {
                if (autocalc.cpu_freq[i+1]-autocalc.cpu_freq[i] > autocalc.cpu_freq_angle_low)
                    autocalc.cpu_freq_angle_low = autocalc.cpu_freq[i+1]-autocalc.cpu_freq[i];
            }
            autocalc.cpu_freq_angle_low*=(100.0/(autocalc.cpu_common));
            autocalc.cpu_mult_low = (autocalc.cpu_mult_low_converter/autocalc.cpu_freq_angle_low)*100;
            if (autocalc.cpu_mult_low > 6)
                autocalc.cpu_mult_low = 6;
            for (int i = autocalc.cpu_curmid; i<cpu.EQsize; i++)
            {
                if (autocalc.cpu_freq[i]-autocalc.cpu_freq[i+1] > autocalc.cpu_freq_angle_high)
                    autocalc.cpu_freq_angle_high = autocalc.cpu_freq[i]-autocalc.cpu_freq[i+1];
            }
            autocalc.cpu_freq_angle_high*=(100.0/(autocalc.cpu_common));
            autocalc.cpu_mult_high = (autocalc.cpu_mult_high_converter/autocalc.cpu_freq_angle_high)*100;
            if (autocalc.cpu_mult_high > 6)
                autocalc.cpu_mult_high = 6;
        }

        for (int i = autocalc.cpu_stablepointlow; i >= 0 ; i--)
        {
            autocalc.cpu_curve[i] = pow((long double)(autocalc.cpu_stablepointlow-i), autocalc.cpu_mult_low);
        }
        if (autocalc.cpu_curve[0] != 0)
            autocalc.cpu_curve_correct = (long double)cpu.stable/autocalc.cpu_curve[0];
        else
            autocalc.cpu_curve_correct = 1;
        for (int i = 0; i <= autocalc.cpu_stablepointlow ; i++)
        {
            autocalc.cpu_curve[i] *= autocalc.cpu_curve_correct;
            if (autocalc.cpu_curve[i] > (long double)cpu.stable)
                autocalc.cpu_curve[i] = (long double)cpu.stable;
            autocalc.cpu_curve[i] = (long double)cpu.stable - autocalc.cpu_curve[i];
        }
        for (int i = autocalc.cpu_stablepointlow; i <= autocalc.cpu_stablepointhigh ; i++)
        {
            autocalc.cpu_curve[i] = (long double)cpu.stable;
        }
        for (int i = 0; i <= cpu.EQsize-autocalc.cpu_stablepointhigh; i++)
        {
            autocalc.cpu_curve[i+autocalc.cpu_stablepointhigh] = pow((long double)i, autocalc.cpu_mult_high);
        }
        if (autocalc.cpu_curve[cpu.EQsize] != 0)
            autocalc.cpu_curve_correct = (long double)(100.0-cpu.stable)/autocalc.cpu_curve[cpu.EQsize];
        else
            autocalc.battery_curve_correct = 1;
        for (int i = 0; i <= cpu.EQsize-autocalc.cpu_stablepointhigh; i++)
        {
            autocalc.cpu_curve[i+autocalc.cpu_stablepointhigh] *= autocalc.cpu_curve_correct;
            if (autocalc.cpu_curve[i+autocalc.cpu_stablepointhigh] > 100.0-(long double)cpu.stable)
                autocalc.cpu_curve[i+autocalc.cpu_stablepointhigh] = 100.0-(long double)cpu.stable;
            autocalc.cpu_curve[i+autocalc.cpu_stablepointhigh] += (long double)cpu.stable;
        }
        for (int i = 0; i <= cpu.EQsize; i++)
        {
            autocalc.cpu_virtualEQ[i] = (autocalc.cpu_virtualEQ[i]*(100.0-autocalc.impact)+autocalc.cpu_curve[i]*autocalc.impact)/100.0;
        }

    }

    if (autocalc.memory_enabled)
    {
        autocalc.memory_curstep = 0;
        autocalc.memory_perc = 0;
        autocalc.memory_common = 0.0;
        autocalc.memory_exoticlow = 100.0;
        autocalc.memory_exotichigh = 100.0;
        autocalc.memory_curmid = 0;
        autocalc.memory_stablepointlow = 0;
        autocalc.memory_stablepointhigh = 0;

        for (int i = 0; i<=memory.EQsize; i++)
        {
            if (memory.load >= (100/memory.EQsize)*i)
                autocalc.memory_curstep = i;
            else
                break;
        }
        if (autocalc.memory_curstep == memory.EQsize)
            autocalc.memory_freq[memory.EQsize]++;
        else
        {
            autocalc.memory_perc = 100*(memory.load-(100/memory.EQsize)*autocalc.memory_curstep)/(100/memory.EQsize);
            autocalc.memory_freq[autocalc.memory_curstep]+=((long double)autocalc.memory_perc)/100.0;
            autocalc.memory_freq[autocalc.memory_curstep]+=(long double)(100-autocalc.memory_perc)/100.0;
        }
        for (int i = 0; i<=memory.EQsize; i++)
        {
            if (autocalc.memory_freq[i] > autocalc.memory_common)
            {
                autocalc.memory_common =autocalc.memory_freq[i];
                autocalc.memory_curmid = i;
            }
        }
        if (autocalc.memory_curmid == 0)
            autocalc.memory_curmid = 1;
        else if (autocalc.memory_curmid == memory.EQsize)
            autocalc.memory_curmid = memory.EQsize-1;

        for (int i = 0; i<autocalc.memory_curmid; i++)
        {
            if (autocalc.memory_freq[i] < autocalc.memory_exoticlow )
                autocalc.memory_exoticlow = autocalc.memory_freq[i];
        }
        for (int i = autocalc.memory_curmid+1; i<=memory.EQsize; i++)
        {
            if (autocalc.memory_freq[i] < autocalc.memory_exotichigh )
                autocalc.memory_exotichigh = autocalc.memory_freq[i];
        }
        bool correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = 0; i<autocalc.memory_curmid; i++)
            {
                if (autocalc.memory_freq[i] > autocalc.memory_freq[i+1])
                {
                    swap(autocalc.memory_freq[i], autocalc.memory_freq[i+1]);
                    correct = false;
                }
            }
        }
        correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = autocalc.memory_curmid; i<memory.EQsize; i++)
            {
                if (autocalc.memory_freq[i] < autocalc.memory_freq[i+1])
                {
                    swap(autocalc.memory_freq[i], autocalc.memory_freq[i+1]);
                    correct = false;
                }
            }
        }
        for (int i = 0; i<autocalc.memory_curmid; i++)
        {
            if (autocalc.memory_freq[i] < autocalc.memory_exoticlow+(long double)(autocalc.memory_swalll*(autocalc.memory_common-autocalc.memory_exoticlow))/100 )
                autocalc.memory_stablepointlow = i;
        }
        for (int i = autocalc.memory_curmid+1; i<=memory.EQsize; i++)
        {
            if (autocalc.memory_freq[i] > autocalc.memory_exotichigh+(long double)(autocalc.memory_swallh*(autocalc.memory_common-autocalc.memory_exotichigh))/100 )
                autocalc.memory_stablepointhigh = i;
        }
        if (autocalc.memory_stablepointhigh == 0)
            autocalc.memory_stablepointhigh = autocalc.memory_curmid+1;
        if (autocalc.auto_memory)
        {
            autocalc.memory_freq_angle_low = 1;
            autocalc.memory_freq_angle_high = 1;
            for (int i = 0; i<autocalc.memory_curmid; i++)
            {
                if (autocalc.memory_freq[i+1]-autocalc.memory_freq[i] > autocalc.memory_freq_angle_low)
                    autocalc.memory_freq_angle_low = autocalc.memory_freq[i+1]-autocalc.memory_freq[i];
            }
            autocalc.memory_freq_angle_low*=(100.0/(autocalc.memory_common));
            autocalc.memory_mult_low = (autocalc.memory_mult_low_converter/autocalc.memory_freq_angle_low)*100;
            if (autocalc.memory_mult_low > 6)
                autocalc.memory_mult_low = 6;
            for (int i = autocalc.memory_curmid; i<memory.EQsize; i++)
            {
                if (autocalc.memory_freq[i]-autocalc.memory_freq[i+1] > autocalc.memory_freq_angle_high)
                    autocalc.memory_freq_angle_high = autocalc.memory_freq[i]-autocalc.memory_freq[i+1];
            }
            autocalc.memory_freq_angle_high*=(100.0/(autocalc.memory_common));
            autocalc.memory_mult_high = (autocalc.memory_mult_high_converter/autocalc.memory_freq_angle_high)*100;
            if (autocalc.memory_mult_high > 6)
                autocalc.memory_mult_high = 6;
        }

        for (int i = autocalc.memory_stablepointlow; i >= 0 ; i--)
        {
            autocalc.memory_curve[i] = pow((long double)(autocalc.memory_stablepointlow-i), autocalc.memory_mult_low);
        }
        if (autocalc.memory_curve[0] != 0)
            autocalc.memory_curve_correct = (long double)memory.stable/autocalc.memory_curve[0];
        else
            autocalc.memory_curve_correct = 1;
        for (int i = 0; i <= autocalc.memory_stablepointlow ; i++)
        {
            autocalc.memory_curve[i] *= autocalc.memory_curve_correct;
            if (autocalc.memory_curve[i] > (long double)memory.stable)
                autocalc.memory_curve[i] = (long double)memory.stable;
            autocalc.memory_curve[i] = (long double)memory.stable - autocalc.memory_curve[i];
        }
        for (int i = autocalc.memory_stablepointlow; i <= autocalc.memory_stablepointhigh ; i++)
        {
            autocalc.memory_curve[i] = (long double)memory.stable;
        }
        for (int i = 0; i <= memory.EQsize-autocalc.memory_stablepointhigh; i++)
        {
            autocalc.memory_curve[i+autocalc.memory_stablepointhigh] = pow((long double)i, autocalc.memory_mult_high);
        }
        if (autocalc.memory_curve[memory.EQsize] != 0)
            autocalc.memory_curve_correct = (long double)(100.0-memory.stable)/autocalc.memory_curve[memory.EQsize];
        else
            autocalc.memory_curve_correct = 1;
        for (int i = 0; i <= memory.EQsize-autocalc.memory_stablepointhigh; i++)
        {
            autocalc.memory_curve[i+autocalc.memory_stablepointhigh] *= autocalc.memory_curve_correct;
            if (autocalc.memory_curve[i+autocalc.memory_stablepointhigh] > 100.0-(long double)memory.stable)
                autocalc.memory_curve[i+autocalc.memory_stablepointhigh] = 100.0-(long double)memory.stable;
            autocalc.memory_curve[i+autocalc.memory_stablepointhigh] += (long double)memory.stable;
        }
        for (int i = 0; i <= memory.EQsize; i++)
        {
            autocalc.memory_virtualEQ[i] = (autocalc.memory_virtualEQ[i]*(100.0-autocalc.impact)+autocalc.memory_curve[i]*autocalc.impact)/100.0;
        }

    }

    if (autocalc.battery_enabled)
    {
        autocalc.battery_curstep = 0;
        autocalc.battery_perc = 0;
        autocalc.battery_common = 0.0;
        autocalc.battery_exoticlow = 100.0;
        autocalc.battery_exotichigh = 100.0;
        autocalc.battery_curmid = 0;
        autocalc.battery_stablepointlow = 0;
        autocalc.battery_stablepointhigh = 0;

        for (int i = 0; i<=battery.EQsize; i++)
        {
            if (battery.load >= (100/battery.EQsize)*i)
                autocalc.battery_curstep = i;
            else
                break;
        }
        if (autocalc.battery_curstep == battery.EQsize)
            autocalc.battery_freq[battery.EQsize]++;
        else
        {
            autocalc.battery_perc = 100*(battery.load-(100/battery.EQsize)*autocalc.battery_curstep)/(100/battery.EQsize);
            autocalc.battery_freq[autocalc.battery_curstep]+=((long double)autocalc.battery_perc)/100.0;
            autocalc.battery_freq[autocalc.battery_curstep]+=(long double)(100-autocalc.battery_perc)/100.0;
        }
        for (int i = 0; i<=battery.EQsize; i++)
        {
            if (autocalc.battery_freq[i] > autocalc.battery_common)
            {
                autocalc.battery_common =autocalc.battery_freq[i];
                autocalc.battery_curmid = i;
            }
        }
        if (autocalc.battery_curmid == 0)
            autocalc.battery_curmid = 1;
        else if (autocalc.battery_curmid == battery.EQsize)
            autocalc.battery_curmid = battery.EQsize-1;

        for (int i = 0; i<autocalc.battery_curmid; i++)
        {
            if (autocalc.battery_freq[i] < autocalc.battery_exoticlow )
                autocalc.battery_exoticlow = autocalc.battery_freq[i];
        }
        for (int i = autocalc.battery_curmid+1; i<=battery.EQsize; i++)
        {
            if (autocalc.battery_freq[i] < autocalc.battery_exotichigh )
                autocalc.battery_exotichigh = autocalc.battery_freq[i];
        }
        bool correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = 0; i<autocalc.battery_curmid; i++)
            {
                if (autocalc.battery_freq[i] > autocalc.battery_freq[i+1])
                {
                    swap(autocalc.battery_freq[i], autocalc.battery_freq[i+1]);
                    correct = false;
                }
            }
        }
        correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = autocalc.battery_curmid; i<battery.EQsize; i++)
            {
                if (autocalc.battery_freq[i] < autocalc.battery_freq[i+1])
                {
                    swap(autocalc.battery_freq[i], autocalc.battery_freq[i+1]);
                    correct = false;
                }
            }
        }
        for (int i = 0; i<autocalc.battery_curmid; i++)
        {
            if (autocalc.battery_freq[i] < autocalc.battery_exoticlow+(long double)(autocalc.battery_swalll*(autocalc.battery_common-autocalc.battery_exoticlow))/100 )
                autocalc.battery_stablepointlow = i;
        }
        for (int i = autocalc.battery_curmid+1; i<=battery.EQsize; i++)
        {
            if (autocalc.battery_freq[i] > autocalc.battery_exotichigh+(long double)(autocalc.battery_swallh*(autocalc.battery_common-autocalc.battery_exotichigh))/100 )
                autocalc.battery_stablepointhigh = i;
        }
        if (autocalc.battery_stablepointhigh == 0)
            autocalc.battery_stablepointhigh = autocalc.battery_curmid+1;
        if (autocalc.auto_battery)
        {
            autocalc.battery_freq_angle_low = 1;
            autocalc.battery_freq_angle_high = 1;
            for (int i = 0; i<autocalc.battery_curmid; i++)
            {
                if (autocalc.battery_freq[i+1]-autocalc.battery_freq[i] > autocalc.battery_freq_angle_low)
                    autocalc.battery_freq_angle_low = autocalc.battery_freq[i+1]-autocalc.battery_freq[i];
            }
            autocalc.battery_freq_angle_low*=(100.0/(autocalc.battery_common));
            autocalc.battery_mult_low = (autocalc.battery_mult_low_converter/autocalc.battery_freq_angle_low)*100;
            if (autocalc.battery_mult_low > 6)
                autocalc.battery_mult_low = 6;
            for (int i = autocalc.battery_curmid; i<battery.EQsize; i++)
            {
                if (autocalc.battery_freq[i]-autocalc.battery_freq[i+1] > autocalc.battery_freq_angle_high)
                    autocalc.battery_freq_angle_high = autocalc.battery_freq[i]-autocalc.battery_freq[i+1];
            }
            autocalc.battery_freq_angle_high*=(100.0/(autocalc.battery_common));
            autocalc.battery_mult_high = (autocalc.battery_mult_high_converter/autocalc.battery_freq_angle_high)*100;
            if (autocalc.battery_mult_high > 6)
                autocalc.battery_mult_high = 6;
        }

        for (int i = autocalc.battery_stablepointlow; i >= 0 ; i--)
        {
            autocalc.battery_curve[i] = pow((long double)(autocalc.battery_stablepointlow-i), autocalc.battery_mult_low);
        }
        if (autocalc.battery_curve[0] != 0)
            autocalc.battery_curve_correct = (long double)battery.stable/autocalc.battery_curve[0];
        else
            autocalc.battery_curve_correct = 1;
        for (int i = 0; i <= autocalc.battery_stablepointlow ; i++)
        {
            autocalc.battery_curve[i] *= autocalc.battery_curve_correct;
            if (autocalc.battery_curve[i] > (long double)battery.stable)
                autocalc.battery_curve[i] = (long double)battery.stable;
            autocalc.battery_curve[i] = (long double)battery.stable - autocalc.battery_curve[i];
        }
        for (int i = autocalc.battery_stablepointlow; i <= autocalc.battery_stablepointhigh ; i++)
        {
            autocalc.battery_curve[i] = (long double)battery.stable;
        }
        for (int i = 0; i <= battery.EQsize-autocalc.battery_stablepointhigh; i++)
        {
            autocalc.battery_curve[i+autocalc.battery_stablepointhigh] = pow((long double)i, autocalc.battery_mult_high);
        }
        if (autocalc.battery_curve[battery.EQsize] != 0)
            autocalc.battery_curve_correct = (long double)(100.0-battery.stable)/autocalc.battery_curve[battery.EQsize];
        else
            autocalc.battery_curve_correct = 1;
        for (int i = 0; i <= battery.EQsize-autocalc.battery_stablepointhigh; i++)
        {
            autocalc.battery_curve[i+autocalc.battery_stablepointhigh] *= autocalc.battery_curve_correct;
            if (autocalc.battery_curve[i+autocalc.battery_stablepointhigh] > 100.0-(long double)battery.stable)
                autocalc.battery_curve[i+autocalc.battery_stablepointhigh] = 100.0-(long double)battery.stable;
            autocalc.battery_curve[i+autocalc.battery_stablepointhigh] += (long double)battery.stable;
        }
        for (int i = 0; i <= battery.EQsize; i++)
        {
            autocalc.battery_virtualEQ[i] = (autocalc.battery_virtualEQ[i]*(100.0-autocalc.impact)+autocalc.battery_curve[i]*autocalc.impact)/100.0;
        }

    }

    if (autocalc.temperature_enabled)
    {
        autocalc.temperature_curstep = 0;
        autocalc.temperature_perc = 0;
        autocalc.temperature_common = 0.0;
        autocalc.temperature_exoticlow = 100.0;
        autocalc.temperature_exotichigh = 100.0;
        autocalc.temperature_curmid = 0;
        autocalc.temperature_stablepointlow = 0;
        autocalc.temperature_stablepointhigh = 0;

        if (temperature.value < temperature.EQbegin)
        {
            autocalc.temperature_freq[0]++;
            cfg->setValue("core.temperature.EQbegin", (int)temperature.value);
            autocalc.forcesave = true;
            info << "autocalc caught new temperature: " << (int)temperature.value << " EQbegin switched to new value";
            warning << "it's recomended to restart Eyes as quick as it's possible!";
        }
        for (int i = 0; i<=temperature.EQsize; i++)
        {
            if (temperature.value >= temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*i)
                autocalc.temperature_curstep = i;
            else
                break;
        }
        if (autocalc.temperature_curstep == temperature.EQsize)
        {
            autocalc.temperature_freq[temperature.EQsize]++;
            cfg->setValue("core.temperature.EQend", (int)temperature.value);
            autocalc.forcesave = true;
            info << "autocalc caught new temperature: " << (int)temperature.value << " EQend switched to new value";
            warning << "it's recomended to restart Eyes as quick as it's possible!";
        }
        else
        {
            autocalc.temperature_perc = 100*(temperature.value-(temperature.EQbegin + ((temperature.EQend-temperature.EQbegin)/temperature.EQsize)*autocalc.temperature_curstep))/((temperature.EQend-temperature.EQbegin)/temperature.EQsize);
            autocalc.temperature_freq[autocalc.temperature_curstep]+=((long double)autocalc.temperature_perc)/100.0;
            autocalc.temperature_freq[autocalc.temperature_curstep]+=(long double)(100-autocalc.temperature_perc)/100.0;
        }
        for (int i = 0; i<=temperature.EQsize; i++)
        {
            if (autocalc.temperature_freq[i] > autocalc.temperature_common)
            {
                autocalc.temperature_common =autocalc.temperature_freq[i];
                autocalc.temperature_curmid = i;
            }
        }
        if (autocalc.temperature_curmid == 0)
            autocalc.temperature_curmid = 1;
        else if (autocalc.temperature_curmid == temperature.EQsize)
            autocalc.temperature_curmid = temperature.EQsize-1;

        for (int i = 0; i<autocalc.temperature_curmid; i++)
        {
            if (autocalc.temperature_freq[i] < autocalc.temperature_exoticlow )
                autocalc.temperature_exoticlow = autocalc.temperature_freq[i];
        }
        for (int i = autocalc.temperature_curmid+1; i<=temperature.EQsize; i++)
        {
            if (autocalc.temperature_freq[i] < autocalc.temperature_exotichigh )
                autocalc.temperature_exotichigh = autocalc.temperature_freq[i];
        }
        bool correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = 0; i<autocalc.temperature_curmid; i++)
            {
                if (autocalc.temperature_freq[i] > autocalc.temperature_freq[i+1])
                {
                    swap(autocalc.temperature_freq[i], autocalc.temperature_freq[i+1]);
                    correct = false;
                }
            }
        }
        correct = false;
        while(!correct)
        {
            correct = true;
            for (int i = autocalc.temperature_curmid; i<temperature.EQsize; i++)
            {
                if (autocalc.temperature_freq[i] < autocalc.temperature_freq[i+1])
                {
                    swap(autocalc.temperature_freq[i], autocalc.temperature_freq[i+1]);
                    correct = false;
                }
            }
        }
        for (int i = 0; i<autocalc.temperature_curmid; i++)
        {
            if (autocalc.temperature_freq[i] < autocalc.temperature_exoticlow+(long double)(autocalc.temperature_swalll*(autocalc.temperature_common-autocalc.temperature_exoticlow))/100 )
                autocalc.temperature_stablepointlow = i;
        }
        for (int i = autocalc.temperature_curmid+1; i<=temperature.EQsize; i++)
        {
            if (autocalc.temperature_freq[i] > autocalc.temperature_exotichigh+(long double)(autocalc.temperature_swallh*(autocalc.temperature_common-autocalc.temperature_exotichigh))/100 )
                autocalc.temperature_stablepointhigh = i;
        }
        if (autocalc.temperature_stablepointhigh == 0)
            autocalc.temperature_stablepointhigh = autocalc.temperature_curmid+1;
        if (autocalc.auto_temperature)
        {
            autocalc.temperature_freq_angle_low = 1;
            autocalc.temperature_freq_angle_high = 1;
            for (int i = 0; i<autocalc.temperature_curmid; i++)
            {
                if (autocalc.temperature_freq[i+1]-autocalc.temperature_freq[i] > autocalc.temperature_freq_angle_low)
                    autocalc.temperature_freq_angle_low = autocalc.temperature_freq[i+1]-autocalc.temperature_freq[i];
            }
            autocalc.temperature_freq_angle_low*=(100.0/(autocalc.temperature_common));
            autocalc.temperature_mult_low = (autocalc.temperature_mult_low_converter/autocalc.temperature_freq_angle_low)*100;
            if (autocalc.temperature_mult_low > 6)
                autocalc.temperature_mult_low = 6;
            for (int i = autocalc.temperature_curmid; i<temperature.EQsize; i++)
            {
                if (autocalc.temperature_freq[i]-autocalc.temperature_freq[i+1] > autocalc.temperature_freq_angle_high)
                    autocalc.temperature_freq_angle_high = autocalc.temperature_freq[i]-autocalc.temperature_freq[i+1];
            }
            autocalc.temperature_freq_angle_high*=(100.0/(autocalc.temperature_common));
            autocalc.temperature_mult_high = (autocalc.temperature_mult_high_converter/autocalc.temperature_freq_angle_high)*100;
            if (autocalc.temperature_mult_high > 6)
                autocalc.temperature_mult_high = 6;
        }

        for (int i = autocalc.temperature_stablepointlow; i >= 0 ; i--)
        {
            autocalc.temperature_curve[i] = pow((long double)(autocalc.temperature_stablepointlow-i), autocalc.temperature_mult_low);
        }
        if (autocalc.temperature_curve[0] != 0)
            autocalc.temperature_curve_correct = (long double)temperature.stable/autocalc.temperature_curve[0];
        else
            autocalc.temperature_curve_correct = 1;
        for (int i = 0; i <= autocalc.temperature_stablepointlow ; i++)
        {
            autocalc.temperature_curve[i] *= autocalc.temperature_curve_correct;
            if (autocalc.temperature_curve[i] > (long double)temperature.stable)
                autocalc.temperature_curve[i] = (long double)temperature.stable;
            autocalc.temperature_curve[i] = (long double)temperature.stable - autocalc.temperature_curve[i];
        }
        for (int i = autocalc.temperature_stablepointlow; i <= autocalc.temperature_stablepointhigh ; i++)
        {
            autocalc.temperature_curve[i] = (long double)temperature.stable;
        }
        for (int i = 0; i <= temperature.EQsize-autocalc.temperature_stablepointhigh; i++)
        {
            autocalc.temperature_curve[i+autocalc.temperature_stablepointhigh] = pow((long double)i, autocalc.temperature_mult_high);
        }
        if (autocalc.temperature_curve[temperature.EQsize] != 0)
            autocalc.temperature_curve_correct = (long double)(100.0-temperature.stable)/autocalc.temperature_curve[temperature.EQsize];
        else
            autocalc.temperature_curve_correct = 1;
        for (int i = 0; i <= temperature.EQsize-autocalc.temperature_stablepointhigh; i++)
        {
            autocalc.temperature_curve[i+autocalc.temperature_stablepointhigh] *= autocalc.temperature_curve_correct;
            if (autocalc.temperature_curve[i+autocalc.temperature_stablepointhigh] > 100.0-(long double)temperature.stable)
                autocalc.temperature_curve[i+autocalc.temperature_stablepointhigh] = 100.0-(long double)temperature.stable;
            autocalc.temperature_curve[i+autocalc.temperature_stablepointhigh] += (long double)temperature.stable;
        }
        for (int i = 0; i <= temperature.EQsize; i++)
        {
            autocalc.temperature_virtualEQ[i] = (autocalc.temperature_virtualEQ[i]*(100.0-autocalc.impact)+autocalc.temperature_curve[i]*autocalc.impact)/100.0;
        }

    }

    if (autocalc.save_next == 0 || autocalc.forcesave)
    {
        info << "Dropping stable values\n";
        if (autocalc.cpu_enabled && !autocalc.cpu_simple)
        {
            for (unsigned short i = 0; i<=cpu.EQsize; i++)
            {
                stringstream ss;
                ss << i;
                cfg->setValue ( &("core.cpu.EQ"+ss.str())[0], (int)autocalc.cpu_virtualEQ[i] );
                cpu.EQ[i] = (int)autocalc.cpu_virtualEQ[i];
                autocalc.cpu_freq[i]/=2.0;
            }
        }
        if (autocalc.memory_enabled && !autocalc.mem_simple)
        {
            for (unsigned short i = 0; i<=memory.EQsize; i++)
            {
                stringstream ss;
                ss << i;
                cfg->setValue ( &("core.memory.EQ"+ss.str())[0], (int)autocalc.memory_virtualEQ[i] );
                memory.EQ[i] = (int)autocalc.memory_virtualEQ[i];
                autocalc.memory_freq[i]/=2.0;
            }
        }
        if (autocalc.battery_enabled && !autocalc.bat_simple)
        {
            for (unsigned short i = 0; i<=battery.EQsize; i++)
            {
                stringstream ss;
                ss << i;
                cfg->setValue ( &("core.battery.EQ"+ss.str())[0], (int)autocalc.battery_virtualEQ[i] );
                battery.EQ[i] = (int)autocalc.battery_virtualEQ[i];
                autocalc.battery_freq[i]/=2.0;
            }
        }
        if (autocalc.temperature_enabled && !autocalc.temp_simple)
        {
            for (unsigned short i = 0; i<=temperature.EQsize; i++)
            {
                stringstream ss;
                ss << i;
                cfg->setValue ( &("core.temperature.EQ"+ss.str())[0], (int)autocalc.temperature_virtualEQ[i] );
                temperature.EQ[i] = (int)autocalc.temperature_virtualEQ[i];
                autocalc.temperature_freq[i]/=2.0;
            }
        }
        cfg->save();
        autocalc.save_next = autocalc.save_interval;
        autocalc.forcesave = false;
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
    cpu.EQsize              = cfg->lookupValue ( "core.cpu.EQsize",                     30          );
    for (unsigned short i = 0; i<=cpu.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        cpu.EQ.push_back (cfg->lookupValue ( &("core.cpu.EQ"+ss.str())[0], (int)cpu.stable ));
    }

    //mem_section

    memory.frequency        = cfg->lookupValue ( "core.memory.frequency",               'q'         );
    memory.lin_num          = cfg->lookupValue ( "core.memory.linear_modifier",         2           );
    memory.stable           = cfg->lookupValue ( "core.memory.stable",                  25          );
    memory.steps            = cfg->lookupValue ( "core.memory.steps",                   8           );
    memory.loseless         = cfg->lookupValue ( "core.memory.adaptation",              10          );
    memory.buffered         = cfg->lookupValue ( "core.memory.buffered",                true        );
    memory.buff_size        = cfg->lookupValue ( "core.memory.buffer_size",             10          );
    memory.EQsize           = cfg->lookupValue ( "core.memory.EQsize",                  30          );
    for (unsigned short i = 0; i<=memory.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        memory.EQ.push_back (cfg->lookupValue ( &("core.memory.EQ"+ss.str())[0], (int)memory.stable ));
    }

    //temperature_section

    temperature.frequency   = cfg->lookupValue ( "core.temperature.frequency",          'q'         );
    temperature.lin_num     = cfg->lookupValue ( "core.temperature.linear_modifier",    2           );
    temperature.stable      = cfg->lookupValue ( "core.temperature.stable",             56          );
    temperature.steps       = cfg->lookupValue ( "core.temperature.steps",              12          );
    temperature.loseless    = cfg->lookupValue ( "core.temperature.adaptation",         2           );
    temperature.buffered    = cfg->lookupValue ( "core.temperature.buffered",           true        );
    temperature.buff_size   = cfg->lookupValue ( "core.temperature.buffer_size",        10          );
    temperature.unit        = cfg->lookupValue ( "core.temperature.unit",               1           );
    temperature.EQbegin     = cfg->lookupValue ( "core.temperature.EQbegin",            30          );
    temperature.EQend       = cfg->lookupValue ( "core.temperature.EQend",              70          );
    temperature.EQsize      = cfg->lookupValue ( "core.temperature.EQsize",             30          );
    for (unsigned short i = 0; i<=temperature.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        temperature.EQ.push_back (cfg->lookupValue ( &("core.temperature.EQ"+ss.str())[0], (int)temperature.stable ));
    }

    //battery_section

    battery.frequency       = cfg->lookupValue ( "core.battery.frequency",              'l'         );
    battery.lin_num         = cfg->lookupValue ( "core.battery.linear_modifier",        0           );
    battery.stable          = cfg->lookupValue ( "core.battery.stable",                 25          );
    battery.steps           = cfg->lookupValue ( "core.battery.steps",                  8           );
    battery.loseless        = cfg->lookupValue ( "core.battery.adaptation",             10          );
    battery.buffered        = cfg->lookupValue ( "core.battery.buffered",               false       );
    battery.buff_size       = cfg->lookupValue ( "core.battery.buffer_size",            10          );
    battery.EQsize          = cfg->lookupValue ( "core.battery.EQsize",                 30          );
    for (unsigned short i = 0; i<=battery.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        battery.EQ.push_back (cfg->lookupValue ( &("core.battery.EQ"+ss.str())[0], (int)battery.stable ));
    }

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

    //bulwers_sector

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
    bulwers.flueamplitude   = cfg->lookupValue ("core.bulwers.flueamplitude",           20          );
    bulwers.flueimpact      = cfg->lookupValue ("core.bulwers.flueimpact",              100         );
    bulwers.fluestepdelay   = cfg->lookupValue ("core.bulwers.fluestepdelay",           300         );
    bulwers.remembered_nrg  = cfg->lookupValue ("core.bulwers.remembered_energy",       0           );
    bulwers.remembered_time = cfg->lookupValue ("core.bulwers.remembered_time",         0           );
    bulwers.max_mem_lag     = cfg->lookupValue ("core.bulwers.max_memory_lag",          10          );
    bulwers.rest_time_std   = cfg->lookupValue ("core.bulwers.rest_time_standard",      9           );
    bulwers.rest_time_wkend = cfg->lookupValue ("core.bulwers.rest_time_weekend",       12          );
    bulwers.nrg_boost       = cfg->lookupValue ("core.bulwers.energyboost",             20          );
    bulwers.nrg_std         = cfg->lookupValue ("core.bulwers.energystd",               16          );
    bulwers.nrg_low         = cfg->lookupValue ("core.bulwers.energylow",               10          );
    bulwers.timelow_1       = cfg->lookupValue ("core.bulwers.timelow_1",               7           );
    bulwers.timelow_2       = cfg->lookupValue ("core.bulwers.timelow_2",               6           );
    bulwers.timelow_3       = cfg->lookupValue ("core.bulwers.timelow_3",               5           );
    bulwers.timehigh_1      = cfg->lookupValue ("core.bulwers.timehigh_1",              21          );
    bulwers.timehigh_2      = cfg->lookupValue ("core.bulwers.timehigh_2",              22          );
    bulwers.timehigh_3      = cfg->lookupValue ("core.bulwers.timehigh_3",              23          );
    bulwers.timelow_1w      = cfg->lookupValue ("core.bulwers.timelow_1",               10          );
    bulwers.timelow_2w      = cfg->lookupValue ("core.bulwers.timelow_2",               7           );
    bulwers.timelow_3w      = cfg->lookupValue ("core.bulwers.timelow_3",               5           );
    bulwers.timehigh_1w     = cfg->lookupValue ("core.bulwers.timehigh_1",              23          );
    bulwers.timehigh_2w     = cfg->lookupValue ("core.bulwers.timehigh_2",              1           );
    bulwers.timehigh_3w     = cfg->lookupValue ("core.bulwers.timehigh_3",              3           );
    bulwers.wkup_time       = cfg->lookupValue ("core.bulwers.wkup_time",               7           );
    bulwers.wkup_timew      = cfg->lookupValue ("core.bulwers.wkup_time_weekend",       10          );
    bulwers.wake_up_delay   = cfg->lookupValue ("core.bulwers.wkup_delay",              120         );

    //friendship_sector

    fship.calm_perc_high    = cfg->lookupValue ("core.friendship.calm_percentage_high", 5           );
    fship.calm_perc_low     = cfg->lookupValue ("core.friendship.calm_percentage_low",  10          );
    fship.calm_timer        = cfg->lookupValue ("core.friendship.calm_timer",           60          );
    fship.func_calm_high    = cfg->lookupValue ("core.friendship.func_calm_high",       1.8         );
    fship.func_calm_low     = cfg->lookupValue ("core.friendship.func_calm_low",        1.8         );
    fship.func_mouse_high   = cfg->lookupValue ("core.friendship.func_mouse_high",      1.8         );
    fship.func_mouse_low    = cfg->lookupValue ("core.friendship.func_mouse_low",       1.8         );
    fship.func_mouse_hit    = cfg->lookupValue ("core.friendship.func_mouse_hit",       1.8         );
    fship.func_scale        = cfg->lookupValue ("core.friendship.func_scale",           20          );
    fship.max_below         = cfg->lookupValue ("core.friendship.max_below_0",          5000        );
    fship.max_over          = cfg->lookupValue ("core.friendship.max_over_0",           5000        );
    fship.mouse_bad         = cfg->lookupValue ("core.friendship.mouse_bad",            20          );
    fship.mouse_good        = cfg->lookupValue ("core.friendship.mouse_good",           1           );
    fship.stable            = cfg->lookupValue ("core.friendship.stable",               200         );
    fship.value             = cfg->lookupValue ("core.friendship.value",                0           );

    //basic_sector

    cdbg_enabled            = cfg->lookupValue("debug.cdebug.on",                       true        );
    hdbg_enabled            = cfg->lookupValue("debug.HDBG_enabled",                    false       );

    //autocalc_sector

    autocalc.enabled                            = cfg->lookupValue("core.autocalc.enabled",                         true        );
    autocalc.save_interval                      = cfg->lookupValue("core.autocalc.interval",                        300         );
    autocalc.start_delay                        = cfg->lookupValue("core.autocalc.delay",                           120         );
    autocalc.impact                             = cfg->lookupValue("core.autocalc.impact",                          1           );
    autocalc.cpu_enabled                        = cfg->lookupValue("core.autocalc.cpu.enabled",                     true        );
    autocalc.auto_cpu                           = cfg->lookupValue("core.autocalc.cpu.auto_angle",                  true        );
    autocalc.cpu_swalll                         = cfg->lookupValue("core.autocalc.cpu.stable_wall_low",             50          );
    autocalc.cpu_swallh                         = cfg->lookupValue("core.autocalc.cpu.stable_wall_high",            50          );
    autocalc.cpu_mult_low                       = cfg->lookupValue("core.autocalc.cpu.multiple_low",                2           );
    autocalc.cpu_mult_low_converter             = cfg->lookupValue("core.autocalc.cpu.low_converter",               1           );
    autocalc.cpu_mult_high                      = cfg->lookupValue("core.autocalc.cpu.multiple_high",               2           );
    autocalc.cpu_mult_high_converter            = cfg->lookupValue("core.autocalc.cpu.high_converter",              1           );
    autocalc.memory_enabled                     = cfg->lookupValue("core.autocalc.memory.enabled",                  true        );
    autocalc.auto_memory                        = cfg->lookupValue("core.autocalc.memory.auto_angle",               true        );
    autocalc.memory_swalll                      = cfg->lookupValue("core.autocalc.memory.stable_wall_low",          50          );
    autocalc.memory_swallh                      = cfg->lookupValue("core.autocalc.memory.stable_wall_high",         50          );
    autocalc.memory_mult_low                    = cfg->lookupValue("core.autocalc.memory.multiple_low",             2           );
    autocalc.memory_mult_low_converter          = cfg->lookupValue("core.autocalc.memory.low_converter",            1           );
    autocalc.memory_mult_high                   = cfg->lookupValue("core.autocalc.memory.multiple_high",            2           );
    autocalc.memory_mult_high_converter         = cfg->lookupValue("core.autocalc.memory.high_converter",           1           );
    autocalc.battery_enabled                    = cfg->lookupValue("core.autocalc.battery.enabled",                 true        );
    autocalc.auto_battery                       = cfg->lookupValue("core.autocalc.battery.auto_angle",              true        );
    autocalc.battery_swalll                     = cfg->lookupValue("core.autocalc.battery.stable_wall_low",         50          );
    autocalc.battery_swallh                     = cfg->lookupValue("core.autocalc.battery.stable_wall_high",        50          );
    autocalc.battery_mult_low                   = cfg->lookupValue("core.autocalc.battery.multiple_low",            2           );
    autocalc.battery_mult_low_converter         = cfg->lookupValue("core.autocalc.battery.low_converter",           1           );
    autocalc.battery_mult_high                  = cfg->lookupValue("core.autocalc.battery.multiple_high",           2           );
    autocalc.battery_mult_high_converter        = cfg->lookupValue("core.autocalc.battery.high_converter",          1           );
    autocalc.temperature_enabled                = cfg->lookupValue("core.autocalc.temperature.enabled",             true        );
    autocalc.auto_temperature                   = cfg->lookupValue("core.autocalc.temperature.auto_angle",          true        );
    autocalc.temperature_swalll                 = cfg->lookupValue("core.autocalc.temperature.stable_wall_low",     50          );
    autocalc.temperature_swallh                 = cfg->lookupValue("core.autocalc.temperature.stable_wall_high",    50          );
    autocalc.temperature_mult_low               = cfg->lookupValue("core.autocalc.temperature.multiple_low",        2           );
    autocalc.temperature_mult_low_converter     = cfg->lookupValue("core.autocalc.temperature.low_converter",       1           );
    autocalc.temperature_mult_high              = cfg->lookupValue("core.autocalc.temperature.multiple_high",       2           );
    autocalc.temperature_mult_high_converter    = cfg->lookupValue("core.autocalc.temperature.high_converter",      1           );
    if (!autocalc.enabled)
    {
        autocalc.cpu_enabled = false;
        autocalc.memory_enabled = false;
        autocalc.battery_enabled = false;
        autocalc.temperature_enabled = false;
    }

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
    eMu.bulwers             = cfg->lookupValue ("core.eMu_zone.bulwers",                false       );
    eMu.bulwers_val         = cfg->lookupValue ("core.eMu_zone.bulwers_val",            0           );

    //mousea_actions_sector

    mousea.buff_size        = cfg->lookupValue ("core.mouse_actions.buff_size",         120         );
    mousea.scale            = cfg->lookupValue ("core.mouse_actions.scale",             8000        );
    mousea.wall             = cfg->lookupValue ("core.mouse_actions.wall",              500         );
    mousea.force_wall       = cfg->lookupValue ("core.mouse_actions.force_wall",        3000        );
    mousea.opt_speed        = cfg->lookupValue ("core.mouse_actions.opt_speed",         250         );
    mousea.impact           = cfg->lookupValue ("core.mouse_actions.impact",            10          );
    mousea.heavycalm        = cfg->lookupValue ("core.mouse_actions.heavycalm",         100         );
    mousea.max_delay        = cfg->lookupValue ("core.mouse_actions.max_delay",         400         );
    mousea.max_hpp_bul      = cfg->lookupValue ("core.mouse_actions.max_hpp_bul",       5           );

    // hardware_section

    HRDWR.battery_capacity  = cfg->lookupValue ( "core.hardware.batt_capacity",         4700        );
    HRDWR.special_batname   = cfg->lookupValue ( "core.hardware.use_batname",           false       );
    HRDWR.special_thername  = cfg->lookupValue ( "core.hardware.use_thername",          false       );
    HRDWR.cfg_battname      = cfg->lookupValue ( "core.hardware.battname",              "BAT0"      );
    HRDWR.cfg_thername      = cfg->lookupValue ( "core.hardware.thername",              "thermal_zone0"        );
    HRDWR.backlight_path    = cfg->lookupValue ( "core.rootcontrol.backlight_device",   "intel_backlight"      );

    //rootcontrol

    rtctrl.screenctrl       = cfg->lookupValue ( "core.rootcontrol.screen_management",  true        );
    rtctrl.batt_min_backl   = cfg->lookupValue ( "core.rootcontrol.batt_min_backl",     20          );
    rtctrl.batt_start_perc  = cfg->lookupValue ( "core.rootcontrol.batt_start_perc",    25          );
    rtctrl.batt_suspend_perc= cfg->lookupValue ( "core.rootcontrol.batt_suspend_perc",  5           );
    rtctrl.roottype         = cfg->lookupValue ( "core.rootcontrol.roottype",           false       );
    rtctrl.suspendtohdd     = cfg->lookupValue ( "core.rootcontrol.suspendtohdd",       false       );
    rtctrl.temp_halt_enabled= cfg->lookupValue ( "core.rootcontrol.temp_halt_enabled",  true        );
    rtctrl.temp_halt_start  = cfg->lookupValue ( "core.rootcontrol.temp_halt_start",    85          );

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
    info << "(core) wake up finished\n";
    if (!core_only_mode)
        eyes->anims_send ("cusual_01", "slp_10_close", "cusual_01_open", 0, 4);
    HRDWR.system_check();
    info << "(core) end of core preparing\n";
    timer->start ( 1000 );
    _cdbg = new cdbg ( this );
}

void Core::on_timer_tick ()
{
    core_step ++;
    bulwers_update ();
    if (!core_only_mode)
        eyes->graphics_prepare();
}

void Core::handle_mouse ( int x, int y )
{
    if (!bulwers.wake_up)
    {
        bulwers.wake_up = true;
        bulwers.wkup_reason = 2;
    }

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
    {
        mousea.hit_active = 1;
        eyes->anims_send("bul_16", "hit_close", "hit_continue", 0, 0);
        eyes->interrupt();
        info << "You\'ve hit Eyes. Be carefull!\n";
    }

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
            fship.to_save = 1;
            if (result >= wall*multiplier)
            {
                fship.mouseimpact(hit_time);
                hit_time++;
                return -(100*result/scale);
            }
            else
            {
                hpp_active = true;
                fship.mouseimpact(0);

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

void eyes_view::hpp_evoke()
{
    if (bulwers.outline <= 4)
    {
        anims_send ("hpp_07", "hpp_01_close", "hpp_continue", 0, 0);
    }
    else if (bulwers.outline <= 6)
    {
        anims_send ("hpp_07", "hpp_02_close", "hpp_continue", 0, 0);
    }
    else if (bulwers.outline <= 8)
    {
        anims_send ("hpp_07", "hpp_03_close", "hpp_continue", 0, 0);
    }
    else
    {
        anims_send ("hpp_07", "hpp_04_close", "hpp_continue", 0, 0);
    }
    interrupt();
}

void Core::handle_enter ()
{
    info << "(core) mouse entered\n";
}

void rootcontrol::execute(bool roottype, QString command, QStringList arguments)
{
    if (roottype)
    {
        QProcess::startDetached (command, arguments);
    }
    else
    {
        QStringList extendedarg;
        extendedarg << command << arguments;
        QProcess::startDetached ("kdesu", extendedarg);
    }
}

void rootcontrol::execute(bool roottype, QString command)
{
    QStringList empty;
    if (roottype)
    {
        QProcess::startDetached (command);
    }
    else
    {
        QStringList extendedarg;
        extendedarg << command;
        QProcess::startDetached ("kdesu", extendedarg);
    }
}

void rootcontrol::action(string command)
{
    if (command == "halt")
        execute(0, QString ( "halt" ));

    if (command == "suspend")
        execute(roottype, QString ( "halt" ));

    if (command == "battery")
    {
        if (battery.load < batt_start_perc && screenctrl)
        {
            if (battery.load < batt_suspend_perc)
                execute(roottype, "suspend");
            else if (HRDWR.screen_support)
            {
                int percentage = 100*(battery.load-batt_suspend_perc)/(batt_start_perc-batt_suspend_perc);
                percentage = batt_min_backl + (percentage*(100-batt_min_backl)/100);
                HRDWR.set_backlight(percentage);
            }
        }
    }

    if (command == "temperature" && temp_halt_enabled)
    {
        QStringList args;
        if (temperature.value > temp_halt_start)
        {
            args << "-h" << "now";
            execute(1, "/sbin/shutdown", args);
        }
    }
}

void camthread::run()
{
    ccap.forget_timer = 10;
    ccap.min_splash_size = 40;
    ccap.max_buff = 10;
    ccap.max_dist = 40;
    ccap.min_dist = 10;
    ccap.motionpicsSize.width = 320;
    ccap.motionpicsSize.height = 180;
    ccap.active_perc = 10;
    ccap.deactive_perc = 5;
    ccap.deactive_delay = 600;

    ccap.cam_init();

    const char *window = "Example 1";
    cvNamedWindow(window, CV_WINDOW_AUTOSIZE);
    cvNamedWindow("DISP2", CV_WINDOW_AUTOSIZE);

    ccap.init_motionpics();

    while (cvWaitKey(4) == -1)
    {

        ccap.motion_detect(ccap.splash_detect(ccap.img2bool(ccap.get_motionpics(ccap.switchmode(), ccap.get_image())), ccap.min_splash_size));
        ccap.sleepdetect();
        //ccap.splash_detect(ccap.img2bool(ccap.get_motionpics(0.2, ccap.get_image())), ccap.min_splash_size);

      //usleep(50000);
      cvShowImage(window, ccap.dst);
      cvShowImage("DISP2", ccap.resized);
    }

    cvDestroyAllWindows();
    //cvReleaseCapture(&cam);

    return 0;
}
