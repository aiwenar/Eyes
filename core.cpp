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
#include <signal.h>
#include <qstring.h>
#include <qvector.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <QMenu>
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
disease         flue;
extern camcapture ccap;

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
            times.mod       +
            energy.mod      -
            mod_bat_plug    -
            mousea.mod      ;
    if (cpu.ready())
        total_mod += cpu.mod;
    if (memory.ready())
        total_mod += memory.mod;
    if (temperature.ready())
        total_mod += temperature.mod;
    if (battery.ready())
        total_mod -= battery.mod;

    if(flue.active && total_mod > 0)
        total_mod+=(double)(total_mod)*flue.last_date.progress*flue.max_bul_booster/100.0;

    //cerr << total_mod << "\n";

    if (!flue.active)
    {
        //cerr << "orginal mod: " << total_mod << "\n";
        if (fship.value > 0)
        {
            if (total_mod > 0)
            {
                total_mod -= ((fship.max_bul_reduction*(100*fship.value/fship.max_over))/100);
                if (total_mod < 0)
                    total_mod = 0;
            }
            else
                total_mod *= (double)(100*fship.value/fship.max_over)/100;
        }
        else
        {
            if (total_mod > 0)
                total_mod *= (100*abs(fship.value)/fship.max_below)/100;
            else
            {
                total_mod += ((fship.max_bul_reduction*(100*fship.value/fship.max_below))/100);
                if (total_mod > 0)
                    total_mod = 0;
            }

        }
        //cerr << "modded mod: " << total_mod << "\n";
    }
    //cerr << total_mod << "\n\n";
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

    if (value >= quickcalm_bulwers)
        step-=(((double)step)*quickcalm)/100.0;

    if (step > limiter)
        step = limiter;

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

void bul::autosave(Configuration *cfg)
{
    cfg->save();
    force_autosave = false;
}

void disease::check(Configuration *cfg)
{
    if (temperature.ready())
    {
        if ((double)temperature.value < lowval)
            lowval=temperature.value;
        if ((double)temperature.value > highval)
            highval = temperature.value;
        highval-=(highval-(double)temperature.stable)*(double)update_impact/100.0;
        lowval+=((double)temperature.stable-lowval)*(double)update_impact/100.0;
        cfg->setValue(".core.flue.temperature_highest", highval);
        cfg->setValue(".core.flue.temperature_lowest", lowval);
        if (highval - lowval > (double)amplitude)
        {
            active = true;
            unsigned short init_day     = get_time().day_num;
            unsigned short init_month   = get_time().month;
            unsigned short init_year    = get_time().year;
            last_date.lenght = rand () % 3 + 3;
            last_date.minute_perc = 100.0/(double)(last_date.lenght*(24-bulwers.rest_time_std)*60);
            last_date.invertion_step = last_date.minute_perc*invertion_perc;
            last_date.progress = last_date.minute_perc;
            cfg->setValue(".core.flue.last_date.day", init_day);
            cfg->setValue(".core.flue.last_date.month", init_month);
            cfg->setValue(".core.flue.last_date.year", init_year);
            cfg->setValue(".core.flue.last_date.lenght", last_date.lenght);
            cfg->setValue(".core.flue.last_date.perc_per_min",  last_date.minute_perc);
            cfg->setValue(".core.flue.last_date.invertion_per_min",  last_date.invertion_step);
            cfg->setValue(".core.flue.last_date.progress", last_date.progress);
            cfg->setValue(".core.flue.active", true);
            bulwers.force_autosave = true;
            info << "flue log:\n" <<
                    "\n  init_day: " << init_day <<
                    "\n  init_month: " << init_month <<
                    "\n  init_year: " << init_year <<
                    "\n  leng: " << last_date.lenght <<
                    "\n  minute step: " << last_date.minute_perc <<
                    "\n  invertion step: " << last_date.invertion_step <<
                    "\n\n";
        }
    }
}

void disease::attack(Configuration *cfg)
{
    //cerr << "orginal progress: " << last_date.progress << "\n";

    if (temperature.ready() && cpu.ready() && memory.ready() && battery.ready())
    {
        last_date.progress+=bulwers.total_mod*bul_impact/100.0;
        //cerr << "after_bulwers: " << last_date.progress << "\n";
    }

    last_date.progress+=ccap.fun.fun*fun_impact/100.0;
    if (mousea.mod > 0)
        last_date.progress+=mousea.mod*pet_impact/100.0;
    else
        last_date.progress-=mousea.mod*hit_impact/100.0;
    //cerr << "after_mouse: " << last_date.progress << "\n";
    //cerr << "bulwers multiplier: " << (double)(bulwers.total_mod)*last_date.progress*max_bul_booster/100.0 << "\n";
    //cerr << "mult: " << last_date.progress*max_bul_booster/100.0 << "\n";

    if (core_step % 60 == 0)
    {
        last_date.progress+=last_date.minute_perc;
        if (expired(last_date))
        {
            last_date.minute_perc-=last_date.invertion_step;
            cfg->setValue(".core.flue.last_date.perc_per_min",  last_date.minute_perc);
        }
        cfg->setValue(".core.flue.last_date.progress", last_date.progress);
        bulwers.force_autosave = true;
    }
    visual_impact(last_date.progress);

    if (last_date.progress < 0)
        flue.active = false;
}

bool disease::expired(disease_time disease_data)
{
    int dtime = 0;
    dtime+=(get_time().year - disease_data.year)*365;
    dtime+=(get_time().month - disease_data.month)*30;
    dtime+=(get_time().day_num - disease_data.day);
    if (dtime > disease_data.lenght)
        return true;
    else
        return false;
}

void disease::visual_impact(double progress)
{
    unsigned short lvl = 0;
    if (progress > step_perc_1)
        lvl = 1;
    if (progress > step_perc_2)
        lvl = 2;
    if (progress > step_perc_3)
        lvl = 3;
    if (progress > step_perc_4)
        lvl = 4;
    if (progress > step_perc_5)
        lvl = 5;

    if (lvl == 0)
        return;

    switch (lvl)
    {
    case 1:
        if (bulwers.tired < 1)
            bulwers.tired = 1;
        if (rand() % visual_impact_probability_1 == 0)
        {
            if (bulwers.outline < 8)
            {
                bulwers.outline = 1;
            }
        }
        break;
    case 2:
        if (bulwers.tired < 2)
            bulwers.tired = 2;
        if (rand() % visual_impact_probability_2 == 0)
        {
            if (bulwers.outline < 8)
            {
                bulwers.outline = 1;
            }
        }
        break;
    case 3:
        if (bulwers.tired < 2)
            bulwers.tired = 2;
        if (bulwers.shy < 1)
            bulwers.shy = 1;
        if (bulwers.hot < 1)
            bulwers.hot = 1;
        if (rand() % visual_impact_probability_3 == 0)
        {
            if (bulwers.outline < 10)
            {
                bulwers.outline = 2;
            }
        }
        break;
    case 4:
        if (bulwers.tired < 3)
            bulwers.tired = 3;
        if (bulwers.shy < 2)
            bulwers.shy = 2;
        if (bulwers.hot < 2)
            bulwers.hot = 2;
        if (rand() % visual_impact_probability_4 == 0)
        {
            if (bulwers.outline < 12)
            {
                bulwers.outline = 3;
            }
        }
        break;
    case 5:
        if (bulwers.tired < 3)
            bulwers.tired = 3;
        if (bulwers.shy < 3)
            bulwers.shy = 3;
        if (bulwers.hot < 3)
            bulwers.hot = 3;
        if (rand() % visual_impact_probability_5 == 0)
        {
            if (bulwers.outline < 13)
            {
                bulwers.outline = 3;
            }
        }
        break;
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

    if (value != 0 && outline != 20 && wake_up)
        outline = value + 3;
    if (outline != 20 && value == 0)
        outline = 0;


    //TODO 01: It must works on cfg values, not static.
    wkup_active = 0;
    if (get_time ().day != 7)
    {
        if (times.value < timelow_1 || times.value > timehigh_1 || energy.value > energy.start + energy.wide - 5)
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
        if (times.value < timelow_2 || times.value > timehigh_2 || energy.value > energy.start + energy.wide - 3)
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
        if (times.value < timelow_3 || times.value > timehigh_3 || energy.value > energy.start + energy.wide - 1)
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
        if (times.value < timelow_1w || times.value > timehigh_1w ||  energy.value > energy.start + energy.wide - 5*3600)
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
        if (times.value < timelow_2w || times.value > timehigh_2w ||  energy.value > energy.start + energy.wide - 4*3600)
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
        if (times.value < timelow_3w || times.value > timehigh_3w ||  energy.value > energy.start + energy.wide - 3*3600)
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
    //cerr << outline << "\n";
    if (battery_state == 0)
    {
        outline = 20;
        eye = 10;
    }
    else if (outline == 20)
        outline = 0;

    atime = get_time().day_num*24*60 + get_time().hour/60;
    bool recover = false;
    if (remembered_time > atime)
    {
        dtime = (24*60 - (remembered_time - (remembered_time%(24*60)))) + atime;
        recover = true;
    }
    else
        dtime = atime - remembered_time;
    if (remembered_time < atime-max_mem_lag)
    {
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
            lastnap_rest = dtperc*(nrg_std)*3600/100;
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
            lastnap_rest = dtperc*(nrg_std)*3600/100;
        }
        if (energy.start > nrg_std*3600)
            energy.start = nrg_std*3600;
        lastnap_remembered_time = remembered_time;
        lastnap_atime = atime;
        lastnap_saved = remembered_nrg;
        lastnap_dtime = dtime;
        cerr << "nap log:\ndelta time was " << dtime << "min - it means: from " << (remembered_time%(24*60))/60 << ":" << remembered_time%60 << " to " << (atime%(24*60))/60 << ":" << atime%60 << "\n"
             << "what is " << 100*dtime/(rest_time_std*60) << "% of daily rest time (" << rest_time_std << "h)\n"
             << "bonus energy is: " << remembered_nrg/3600 << ":" << (remembered_nrg%3600)/60 << ":" << remembered_nrg%60 << " from unused energy and " << lastnap_rest/60 << " minutes from nap\n"
             << "mainimum nap time was set to: " << max_mem_lag << "min.\n\n";
    }
    remembered_time = atime;
    remembered_nrg = energy.start - energy.value;
    if (remembered_nrg > nrg_std*3600)
        remembered_nrg = nrg_std*3600;

    //saving

    cfg->setValue(".core.bulwers.remembered_time", (int)remembered_time);
    cfg->setValue(".core.bulwers.remembered_energy", (int)remembered_nrg);
    if (fship.to_save)
        cfg->setValue(".core.friendship.value", (int)fship.value);
    if (wake_up)
        cfg->setValue(".core.bulwers.saved_step", (int)step);

    rtctrl.action("battery");
    rtctrl.action("temperature");
    if (rtctrl.scrnsaver_management && ccap.enabled)
    {
        int screensaverstate = ccap.screensaver_management();

        //cerr << "Return statement of menagement: " << screensaverstate << "\noverdetect is: " << ccap.overdetect << "\n";

        if (screensaverstate == -1 && ccap.deactivate_screensaver)
        {
            rtctrl.scrnsav_switched = false;
            if (rtctrl.scrnsav_X)
                rtctrl.shell("xscreensaver-command -deactivate > /dev/null");
            if (rtctrl.scrnsav_gnome)
            {
                rtctrl.shell("gnome-screensaver-command -d > /dev/null");
                rtctrl.shell("gnome-screensaver-command -p > /dev/null 2>/dev/null");
            }
            if (rtctrl.scrnsav_kde)
                rtctrl.shell("dbus-send --type=method_call --dest=org.freedesktop.ScreenSaver /ScreenSaver org.freedesktop.ScreenSaver.SetActive boolean:false > /dev/null");
            if (rtctrl.scrnsav_mac)
                rtctrl.shell("killall ScreenSaverEngine > /dev/null 2>/dev/null");
            if (rtctrl.scrnsav_custom)
                rtctrl.shell(rtctrl.scrnsav_custom_off + " >/dev/null 2>/dev/null");

            rtctrl.shell("xset dpms force on > /dev/null");
        }
        if (screensaverstate == 1 && !rtctrl.scrnsav_switched)
        {
            rtctrl.scrnsav_switched = true;
            if (ccap.activate_screensaver)
            {
                if (rtctrl.scrnsav_X)
                    rtctrl.shell("xscreensaver-command -activate >/dev/null 2>/dev/null");
                if (rtctrl.scrnsav_gnome)
                    rtctrl.shell("gnome-screensaver-command -a >/dev/null 2>/dev/null");
                if (rtctrl.scrnsav_kde)
                    rtctrl.shell("dbus-send --type=method_call --dest=org.freedesktop.ScreenSaver /ScreenSaver org.freedesktop.ScreenSaver.SetActive boolean:true > /dev/null 2>/dev/null");
                if (rtctrl.scrnsav_mac);
                    rtctrl.shell("open /System/Library/Frameworks/ScreenSaver.framework/Resources/ScreenSaverEngine.app > /dev/null 2>/dev/null");
                    //defaults -currentHost write com.apple.screensaver idleTime 180
                if (rtctrl.scrnsav_custom)
                    rtctrl.shell(rtctrl.scrnsav_custom_on + " >/dev/null 2>/dev/null");
            }

            if (ccap.turnoff_screen)
            {
                rtctrl.shell("xset dpms force off > /dev/null 2>/dev/null");
            }
        }
    }

    if (temperature.ready())
    {
        bulwers.hot = 0;
        bulwers.shy = 0;
        if (temperature.value > temperature.EQbegin + bulwers.sweat_perc_3*(temperature.EQend-temperature.EQbegin)/100)
        {
            bulwers.hot=3;
            bulwers.shy = 1;
        }
        else if (temperature.value > temperature.EQbegin + bulwers.sweat_perc_2*(temperature.EQend-temperature.EQbegin)/100)
        {
            bulwers.hot=2;
        }
        else if (temperature.value > temperature.EQbegin + bulwers.sweat_perc_1*(temperature.EQend-temperature.EQbegin)/100)
        {
            bulwers.hot=1;
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
    cpu.mod_correction_neg = cpu.max_mod_neg/pow((double)(100.0-cpu.stable-cpu.safezone), cpu.degree);
    cpu.mod_correction_pos = cpu.max_mod_pos/pow((double)(cpu.stable-cpu.safezone), cpu.degree);

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
    memory.mod_correction_neg = memory.max_mod_neg/pow((double)(100.0-memory.stable-memory.safezone), memory.degree);
    memory.mod_correction_pos = memory.max_mod_pos/pow((double)(memory.stable-memory.safezone), memory.degree);

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
    battery.mod_correction_pos = battery.max_mod_pos/pow((double)(100.0-battery.stable-battery.safezone), battery.degree);
    battery.mod_correction_neg = battery.max_mod_neg/pow((double)(battery.stable-battery.safezone), battery.degree);

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
    temperature.mod_correction_pos = temperature.max_mod_pos/pow((double)(100.0-temperature.stable-temperature.safezone), temperature.degree);
    temperature.mod_correction_neg = temperature.max_mod_neg/pow((double)(temperature.stable-temperature.safezone), temperature.degree);

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

    energy.value                        = 0     ;
    energy.start                       *= 3600  ;
    energy.wide                        *= 3600  ;
    once_plugged                        = false ;
    mod_bat_plug                        = 0     ;
    bulwers.wake_up                     = false ;
    bulwers.no_update                   = false ;
    bulwers.wkup_active                 = 0     ;
    bulwers.wkup_reason                 = 0     ;
    bulwers.current_wkup_delay          = bulwers.wake_up_delay;
    if (bulwers.remembered_time == 0)
        bulwers.remembered_time         = get_time().day_num*60*24 + get_time().hour/60;
    bulwers.lastnap_atime               = 0     ;
    bulwers.lastnap_remembered_time     = 0     ;
    bulwers.lastnap_rest                = 0     ;
    bulwers.lastnap_saved               = 0     ;
    bulwers.lastnap_dtime               = 0     ;
    bulwers.dtime                       = 0     ;
    bulwers.force_autosave              = false ;
    rtctrl.shelldetect();

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
        bool fun_evoked=false;
        if (bulwers.hpp > 0 && bulwers.outline+4 < bulwers.max_fun_hpp_bul)
        {
            int tmp = rand () % bulwers.smile_probability;
            if (tmp == 0)
            {
                fun_evoked=true;
                if (bulwers.hpp == 1)
                {
                    int tmp = rand () % 2;
                    if (tmp == 0)
                    {
                        face_send = "hpp_01";
                    }
                    if (tmp == 1)
                    {
                        face_send = "hpp_02";
                    }
                }
                if (bulwers.hpp == 2)
                {
                    int tmp = rand () % 2;
                    if (tmp == 0)
                    {
                        face_send = "hpp_02";
                    }
                    if (tmp == 1)
                    {
                        face_send = "hpp_03";
                    }
                }
            }
        }

        if (bulwers.tired == 0)
            toggle_layer(SLEEPY, false);
        else
        {
            stringstream ss;
            ss << bulwers.tired;
            toggle_layer(SLEEPY, true);
            set_layer(SLEEPY, &("tired_0" + ss.str())[0]);
        }

        if (bulwers.shy == 0)
            toggle_layer(SHY, false);
        else
        {
            stringstream ss;
            ss << bulwers.shy;
            toggle_layer(SHY, true);
            set_layer(SHY, &("shy_0" + ss.str())[0]);
        }

        if (bulwers.hot == 0)
            toggle_layer(HOT, false);
        else
        {
            stringstream ss;
            ss << bulwers.hot;
            toggle_layer(HOT, true);
            set_layer(HOT, &("sweat_0" + ss.str())[0]);
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
       if (face_send == "hpp_01")
           anim_num_2 = 0;
       if (face_send == "bul_09" ||
           face_send == "bul_10" ||
           face_send == "bul_11" ||
           face_send == "bul_12" ||
           face_send == "bul_13" ||
           face_send == "bul_14" ||
           face_send == "bul_15" ||
           face_send == "hpp_02" )
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
           face_send == "slp_05" ||
           face_send == "hpp_03" )
           anim_num_2 = 4;
       if (face_send == "bul_05" ||
           face_send == "bul_06" ||
           face_send == "bul_07" ||
           face_send == "bul_08" ||
           face_send == "slp_06" ||
           face_send == "hpp_04" )
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
           looker->look();
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

       if (fun_evoked)
       {
           if (bulwers.outline <= 4)
           {
               anims_send (face_send, "hpp_01_close", face_send +"_open", anim_num_1, anim_num_2);
           }
           else if (bulwers.outline <= 6)
           {
               anims_send (face_send, "hpp_02_close", face_send +"_open", anim_num_1, anim_num_2);
           }
           else if (bulwers.outline <= 8)
           {
               anims_send (face_send, "hpp_03_close", face_send +"_open", anim_num_1, anim_num_2);
           }
           else
           {
               anims_send (face_send, "hpp_04_close", face_send +"_open", anim_num_1, anim_num_2);
           }
       }
    }
    bulwers.prev_outline = bulwers.outline;
}

void friendship::save(Configuration *cfg)
{
    if (to_save)
        cfg->setValue(".core.friendship.value", (int)value);
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
            bulwers.step += pow((double)mouse_bad, func_mouse_hit);
            value-=pow((double)mouse_bad, func_mouse_hit);
            if (value < -(int)max_below)
                value = -(int)max_below;
        }
    }
}

void Core::bulwers_update ()
{
    if (eMu.cpu)
        cpu.get_load(eMu.cpu_val);
    else
        cpu.get_load(HRDWR.C_LOAD());

    if (eMu.mem)
        memory.get_load(eMu.mem_val);
    else
        memory.get_load(HRDWR.M_LOAD ());

    if (eMu.temp)
        temperature.get_load(eMu.temp_val);
    else
        temperature.get_load(HRDWR.temperatura());

    if (eMu.batt)
        battery.get_load(eMu.batt_val);
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
        times.value = eMu.time_val;
    else
        times.value = get_time ().hour/3600;

    if (eMu.energy)
        energy.value = eMu.energy_val;
    else
        energy.value ++;

    if (eMu.batt_s)
        battery_state = eMu.batt_s_val;
    else
        battery_state = HRDWR.bat_plugged ();


    cpu.mod = cpu.convert(cpu.calculate());
    memory.mod = memory.convert(memory.calculate());
    battery.mod = battery.convert(battery.calculate());
    temperature.mod = temperature.convert(temperature.calculate());
    times.mod = times.calculate();
    energy.mod = energy.calculate();
    mousea.mod = mousea.impact*mousea.convert()/100;
    bulwers.wake_up_chk();
    if (!bulwers.no_update)
    {
        bulwers.update();
        bulwers.critical_services( Configuration::getInstance () );
        if (!flue.active)
            flue.check( Configuration::getInstance () );
        else
            flue.attack( Configuration::getInstance () );
        if ((core_step+1) % bulwers.autosave_interval == 0 || bulwers.force_autosave )
            bulwers.autosave( Configuration::getInstance() );
    }
    bulwers.no_update = false;

    if (autocalc.enabled)
        autocalc_reload ( Configuration::getInstance () );
    bulwers.check_env(ccap.env.checked, Configuration::getInstance ());
    bulwers.fun_check();
}

Core::Core ( eyes_view * neyes )
{
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
    connect ( neyes, SIGNAL ( mousemoved (int,int) ), this, SLOT ( handle_mouse(int,int) ) );
    eyes = neyes;
}

Core::~Core ()
{
  raise ( SIGQUIT );
}

Core::Core ()
{
  timer = new QTimer ( this );
  connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
  eyes = 0;
}

void Core::autocalc_init ()
{
    cpu.ac.simple = false;
    memory.ac.simple = false;
    battery.ac.simple = false;
    temperature.ac.simple = false;
    autocalc.save_next = autocalc.save_interval;

    cpu.ac_init("cpu");
    memory.ac_init("memory");
    battery.ac_init("battery");
    temperature.ac_init("temperature");
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

    if (cpu.ac.enabled)
        cpu.autocalc();
    if (memory.ac.enabled)
        memory.autocalc();
    if (battery.ac.enabled)
        battery.autocalc();
    if (temperature.ac.enabled)
        temperature.autocalc( cfg );


    if (cpu.ac.enabled && !cpu.ac.simple && (autocalc.save_next == 0 || cpu.ac.forcesave))
    {
        info << "Dropping stable values (cpu)\n";
        cpu.ac_save(cfg);
        bulwers.force_autosave = true;
    }
    if (memory.ac.enabled && !memory.ac.simple && (autocalc.save_next == 0 || memory.ac.forcesave))
    {
        info << "Dropping stable values (memory)\n";
        memory.ac_save(cfg);
        bulwers.force_autosave = true;
    }
    if (battery.ac.enabled && !battery.ac.simple && (autocalc.save_next == 0 || battery.ac.forcesave))
    {
        info << "Dropping stable values (battery)\n";
        battery.ac_save(cfg);
        bulwers.force_autosave = true;
    }
    if (temperature.ac.enabled && !temperature.ac.simple && (autocalc.save_next == 0 || temperature.ac.forcesave))
    {
        info << "Dropping stable values (temperature)\n";
        temperature.ac_save(cfg);
        bulwers.force_autosave = true;
    }
    if (autocalc.save_next == 0)
        autocalc.save_next = autocalc.save_interval;
}

void Core::add_menu_items ( QMenu * menu )
{
  // menu->addSeparator (); - separate core actions from others

  // QAction a = new QAction ( tr ( "z&Zz" ), this ); - create an action, char with & 'll be shortcut for action
  // connect ( a, SIGNAL ( triggered () ), this, SLOT ( someRandomFunction() ) ); - QAction send triggered() signal when user preses the action.
}

void Core::load_config ()
{
    Configuration * cfg = Configuration::getInstance ();

    con = new connectionGate();
    con->startServer();
    con->index = new varIndex;

    cpu.degree              = cfg->lookupValue ( ".core.cpu.degree",                     2.0         );
    cpu.stable              = cfg->lookupValue ( ".core.cpu.stable",                     25          );
    cpu.max_mod_neg         = cfg->lookupValue ( ".core.cpu.max_mod_neg",                60          );
    cpu.max_mod_pos         = cfg->lookupValue ( ".core.cpu.max_mod_pos",                50          );
    cpu.safezone            = cfg->lookupValue ( ".core.cpu.safezone",                   10          );
    cpu.buffered            = cfg->lookupValue ( ".core.cpu.buffered",                   true        );
    cpu.buff_size           = cfg->lookupValue ( ".core.cpu.buffer_size",                10          );
    cpu.EQsize              = cfg->lookupValue ( ".core.cpu.EQsize",                     30          );
    for (unsigned short i = 0; i<=cpu.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        cpu.EQ.push_back (cfg->lookupValue ( &(".core.cpu.EQ"+ss.str())[0], (int)cpu.stable ));
    }

    con->index->registerVariable(&cpu.degree, varIndex::Tdouble, QString ("CPU:DEGREE"), false);
    con->index->registerVariable(&cpu.stable, varIndex::Tdouble, QString ("CPU:STABLE"), false);
    con->index->registerVariable((long long*)&cpu.max_mod_neg, varIndex::Tuint, QString ("CPU:MAXNEG"), false);
    con->index->registerVariable((long long*)&cpu.max_mod_pos, varIndex::Tuint, QString ("CPU:MAXPOS"), false);
    con->index->registerVariable((long long*)&cpu.safezone, varIndex::Tuint, QString ("CPU:SAFE"), false);
    con->index->registerVariable(&cpu.buffered, varIndex::Tbool, QString ("CPU:BUFFERED"), true);

    //mem_section

    memory.degree           = cfg->lookupValue ( ".core.memory.degree",                  3.0         );
    memory.stable           = cfg->lookupValue ( ".core.memory.stable",                  25          );
    memory.max_mod_neg      = cfg->lookupValue ( ".core.memory.max_mod_neg",             100         );
    memory.max_mod_pos      = cfg->lookupValue ( ".core.memory.max_mod_pos",             50          );
    memory.safezone         = cfg->lookupValue ( ".core.memory.safezone",                10          );
    memory.buffered         = cfg->lookupValue ( ".core.memory.buffered",                true        );
    memory.buff_size        = cfg->lookupValue ( ".core.memory.buffer_size",             10          );
    memory.EQsize           = cfg->lookupValue ( ".core.memory.EQsize",                  30          );
    for (unsigned short i = 0; i<=memory.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        memory.EQ.push_back (cfg->lookupValue ( &(".core.memory.EQ"+ss.str())[0], (int)memory.stable ));
    }

    con->index->registerVariable(&memory.degree, varIndex::Tdouble, QString ("MEM:DEGREE"), false);
    con->index->registerVariable(&memory.stable, varIndex::Tdouble, QString ("MEM:STABLE"), false);
    con->index->registerVariable((long long*)&memory.max_mod_neg, varIndex::Tuint, QString ("MEM:MAXNEG"), false);
    con->index->registerVariable((long long*)&memory.max_mod_pos, varIndex::Tuint, QString ("MEM:MAXPOS"), false);
    con->index->registerVariable((long long*)&memory.safezone, varIndex::Tuint, QString ("MEM:SAFE"), false);
    con->index->registerVariable(&memory.buffered, varIndex::Tbool, QString ("MEM:BUFFERED"), true);

    //temperature_section

    temperature.degree      = cfg->lookupValue ( ".core.temperature.degree",             1.5         );
    temperature.stable      = cfg->lookupValue ( ".core.temperature.stable",             54          );
    temperature.max_mod_neg = cfg->lookupValue ( ".core.temperature.max_mod_neg",        200         );
    temperature.max_mod_pos = cfg->lookupValue ( ".core.temperature.max_mod_pos",        100         );
    temperature.safezone    = cfg->lookupValue ( ".core.temperature.safezone",           8           );
    temperature.buffered    = cfg->lookupValue ( ".core.temperature.buffered",           true        );
    temperature.buff_size   = cfg->lookupValue ( ".core.temperature.buffer_size",        10          );
    temperature.EQbegin     = cfg->lookupValue ( ".core.temperature.EQbegin",            30          );
    temperature.EQend       = cfg->lookupValue ( ".core.temperature.EQend",              70          );
    temperature.EQsize      = cfg->lookupValue ( ".core.temperature.EQsize",             30          );
    for (unsigned short i = 0; i<=temperature.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        temperature.EQ.push_back (cfg->lookupValue ( &(".core.temperature.EQ"+ss.str())[0], (int)temperature.stable ));
    }

    con->index->registerVariable(&temperature.degree, varIndex::Tdouble, QString ("TEMP:DEGREE"), false);
    con->index->registerVariable(&temperature.stable, varIndex::Tdouble, QString ("TEMP:STABLE"), false);
    con->index->registerVariable((long long*)&temperature.max_mod_neg, varIndex::Tuint, QString ("TEMP:MAXNEG"), false);
    con->index->registerVariable((long long*)&temperature.max_mod_pos, varIndex::Tuint, QString ("TEMP:MAXPOS"), false);
    con->index->registerVariable((long long*)&temperature.safezone, varIndex::Tuint, QString ("TEMP:SAFE"), false);
    con->index->registerVariable(&temperature.buffered, varIndex::Tbool, QString ("TEMP:BUFFERED"), true);

    //battery_section

    battery.degree          = cfg->lookupValue ( ".core.battery.degree",                 1.5         );
    battery.stable          = cfg->lookupValue ( ".core.battery.stable",                 25          );
    battery.max_mod_neg     = cfg->lookupValue ( ".core.battery.max_mod_neg",            100         );
    battery.max_mod_pos     = cfg->lookupValue ( ".core.battery.max_mod_pos",            60          );
    battery.safezone        = cfg->lookupValue ( ".core.battery.safezone",               10          );
    battery.buffered        = cfg->lookupValue ( ".core.battery.buffered",               false       );
    battery.buff_size       = cfg->lookupValue ( ".core.battery.buffer_size",            10          );
    battery.EQsize          = cfg->lookupValue ( ".core.battery.EQsize",                 30          );
    for (unsigned short i = 0; i<=battery.EQsize; i++)
    {
        stringstream ss;
        ss << i;
        battery.EQ.push_back (cfg->lookupValue ( &(".core.battery.EQ"+ss.str())[0], (int)battery.stable ));
    }

    con->index->registerVariable(&battery.degree, varIndex::Tdouble, QString ("BAT:DEGREE"), false);
    con->index->registerVariable(&battery.stable, varIndex::Tdouble, QString ("BAT:STABLE"), false);
    con->index->registerVariable((long long*)&battery.max_mod_neg, varIndex::Tuint, QString ("BAT:MAXNEG"), false);
    con->index->registerVariable((long long*)&battery.max_mod_pos, varIndex::Tuint, QString ("BAT:MAXPOS"), false);
    con->index->registerVariable((long long*)&battery.safezone, varIndex::Tuint, QString ("BAT:SAFE"), false);
    con->index->registerVariable(&battery.buffered, varIndex::Tbool, QString ("BAT:BUFFERED"), true);

    //times_sector

    times.frequency         = cfg->lookupValue ( ".core.times.frequency",                'q'         );
    times.lin_num           = cfg->lookupValue ( ".core.times.quad_modifier",            2           );
    times.start             = cfg->lookupValue ( ".core.times.start",                    20          );
    times.steps             = cfg->lookupValue ( ".core.times.steps",                    6           );
    times.end               = cfg->lookupValue ( ".core.times.end",                      6           );
    times.wide              = cfg->lookupValue ( ".core.times.wide",                     6           );

    //energy_sector

    energy.frequency        = cfg->lookupValue ( ".core.energy.frequency",               'q'         );
    energy.lin_num          = cfg->lookupValue ( ".core.energy.quad_modifier",           2           );
    energy.start            = cfg->lookupValue ( ".core.energy.start",                   16          );
    energy.steps            = cfg->lookupValue ( ".core.energy.steps",                   6           );
    energy.end              = cfg->lookupValue ( ".core.energy.end",                     0           );
    energy.wide             = cfg->lookupValue ( ".core.energy.wide",                    6           );

    //bulwers_sector

    bulwers.step            = cfg->lookupValue (".core.bulwers.saved_step",              0           );
    bulwers.wall_01         = cfg->lookupValue (".core.bulwers.wall_01",                 300         );
    bulwers.wall_02         = cfg->lookupValue (".core.bulwers.wall_02",                 500         );
    bulwers.wall_03         = cfg->lookupValue (".core.bulwers.wall_03",                 800         );
    bulwers.wall_04         = cfg->lookupValue (".core.bulwers.wall_04",                 1300        );
    bulwers.wall_05         = cfg->lookupValue (".core.bulwers.wall_05",                 2100        );
    bulwers.wall_06         = cfg->lookupValue (".core.bulwers.wall_06",                 3400        );
    bulwers.wall_07         = cfg->lookupValue (".core.bulwers.wall_07",                 5500        );
    bulwers.wall_08         = cfg->lookupValue (".core.bulwers.wall_08",                 8900        );
    bulwers.wall_09         = cfg->lookupValue (".core.bulwers.wall_09",                 14400       );
    bulwers.wall_10         = cfg->lookupValue (".core.bulwers.wall_10",                 23300       );
    bulwers.wall_11         = cfg->lookupValue (".core.bulwers.wall_11",                 37700       );
    bulwers.wall_12         = cfg->lookupValue (".core.bulwers.wall_12",                 61600       );
    bulwers.wall_13         = cfg->lookupValue (".core.bulwers.wall_13",                 98700       );
    bulwers.wall_14         = cfg->lookupValue (".core.bulwers.wall_14",                 159700      );
    bulwers.wall_15         = cfg->lookupValue (".core.bulwers.wall_15",                 258400      );
    bulwers.limiter         = cfg->lookupValue (".core.bulwers.wall_15",                 500000      );
    bulwers.remembered_nrg  = cfg->lookupValue (".core.bulwers.remembered_energy",       0           );
    bulwers.remembered_time = cfg->lookupValue (".core.bulwers.remembered_time",         0           );
    bulwers.max_mem_lag     = cfg->lookupValue (".core.bulwers.max_memory_lag",          10          );
    bulwers.rest_time_std   = cfg->lookupValue (".core.bulwers.rest_time_standard",      9           );
    bulwers.rest_time_wkend = cfg->lookupValue (".core.bulwers.rest_time_weekend",       12          );
    bulwers.nrg_boost       = cfg->lookupValue (".core.bulwers.energyboost",             20          );
    bulwers.nrg_std         = cfg->lookupValue (".core.bulwers.energystd",               16          );
    bulwers.nrg_low         = cfg->lookupValue (".core.bulwers.energylow",               10          );
    bulwers.timelow_1       = cfg->lookupValue (".core.bulwers.timelow_1",               7           );
    bulwers.timelow_2       = cfg->lookupValue (".core.bulwers.timelow_2",               6           );
    bulwers.timelow_3       = cfg->lookupValue (".core.bulwers.timelow_3",               5           );
    bulwers.timehigh_1      = cfg->lookupValue (".core.bulwers.timehigh_1",              21          );
    bulwers.timehigh_2      = cfg->lookupValue (".core.bulwers.timehigh_2",              22          );
    bulwers.timehigh_3      = cfg->lookupValue (".core.bulwers.timehigh_3",              23          );
    bulwers.timelow_1w      = cfg->lookupValue (".core.bulwers.timelow_1",               10          );
    bulwers.timelow_2w      = cfg->lookupValue (".core.bulwers.timelow_2",               7           );
    bulwers.timelow_3w      = cfg->lookupValue (".core.bulwers.timelow_3",               5           );
    bulwers.timehigh_1w     = cfg->lookupValue (".core.bulwers.timehigh_1",              23          );
    bulwers.timehigh_2w     = cfg->lookupValue (".core.bulwers.timehigh_2",              1           );
    bulwers.timehigh_3w     = cfg->lookupValue (".core.bulwers.timehigh_3",              3           );
    bulwers.wkup_time       = cfg->lookupValue (".core.bulwers.wkup_time",               7           );
    bulwers.wkup_timew      = cfg->lookupValue (".core.bulwers.wkup_time_weekend",       10          );
    bulwers.wake_up_delay   = cfg->lookupValue (".core.bulwers.wkup_delay",              120         );
    int counter             = cfg->lookupValue (".core.bulwers.envs_number",             0           );
    for (int i = 0; i < counter; i++)
    {
        stringstream ss;
        ss << i;
        environment_data* tmpenv = new environment_data;
        bulwers.envs.push_back(*tmpenv);
        bulwers.envs[i].Rperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Rperc")[0],0 ));
        bulwers.envs[i].Yperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Yperc")[0],0 ));
        bulwers.envs[i].Gperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Gperc")[0],0 ));
        bulwers.envs[i].Bperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Bperc")[0],0 ));
        bulwers.envs[i].Pperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Pperc")[0],0 ));
        bulwers.envs[i].Hperc = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".Hperc")[0],0 ));
        bulwers.envs[i].spenttime = (cfg->lookupValue ( &(".core.environment.env"+ss.str()+".spenttime")[0],0 ));
        bulwers.envs[i].timer = 0;
        delete (tmpenv);
    }
    bulwers.env_min_compability         = cfg->lookupValue (".core.bulwers.env_min_compability",         60.0        );
    bulwers.env_update_impact           = cfg->lookupValue (".core.bulwers.env_update_impact",           5.0         );
    bulwers.env_max_exotic_spenttime    = cfg->lookupValue (".core.bulwers.env_max_exotic_spenttime",    10.0        );
    bulwers.env_saveinterval            = cfg->lookupValue (".core.bulwers.env_saveinterval",            50          );
    bulwers.sweat_perc_1                = cfg->lookupValue (".core.bulwers.sweat_perc_1",                50.0        );
    bulwers.sweat_perc_2                = cfg->lookupValue (".core.bulwers.sweat_perc_2",                75.0        );
    bulwers.sweat_perc_3                = cfg->lookupValue (".core.bulwers.sweat_perc_3",                90.0        );
    bulwers.hpp_fun_perc_1              = cfg->lookupValue (".core.bulwers.hpp_fun_perc_1",              60.0        );
    bulwers.hpp_fun_perc_2              = cfg->lookupValue (".core.bulwers.hpp_fun_perc_2",              150.0       );
    bulwers.max_fun_hpp_bul             = cfg->lookupValue (".core.bulwers.max_hpp_fun_evoke_bul",       5           );
    bulwers.autosave_interval           = cfg->lookupValue (".core.bulwers.autosave_interval",           300         );
    bulwers.quickcalm                   = cfg->lookupValue (".core.bulwers.quickcalm_perc",              0.1         );
    bulwers.quickcalm_bulwers           = cfg->lookupValue (".core.bulwers.quickcalm_min_bulwers",       5           );

    con->index->registerVariable((long long*)&bulwers.step, varIndex::Tuint, QString ("BUL:STEP"), false);
    con->index->registerVariable(&bulwers.quickcalm, varIndex::Tdouble, QString ("BUL:CALM"), false);
    con->index->registerVariable((long long*)&bulwers.quickcalm_bulwers, varIndex::Tushort, QString ("BUL:CALMBUL"), false);
    con->index->registerVariable((long long*)&bulwers.current_wkup_delay, varIndex::Tushort, QString ("BUL:WKUP"), false);

    //friendship_sector

    fship.calm_perc_high        = cfg->lookupValue (".core.friendship.calm_percentage_high", 5           );
    fship.calm_perc_low         = cfg->lookupValue (".core.friendship.calm_percentage_low",  10          );
    fship.calm_timer            = cfg->lookupValue (".core.friendship.calm_timer",           60          );
    fship.func_calm_high        = cfg->lookupValue (".core.friendship.func_calm_high",       1.8         );
    fship.func_calm_low         = cfg->lookupValue (".core.friendship.func_calm_low",        1.8         );
    fship.func_mouse_high       = cfg->lookupValue (".core.friendship.func_mouse_high",      1.8         );
    fship.func_mouse_low        = cfg->lookupValue (".core.friendship.func_mouse_low",       1.8         );
    fship.func_mouse_hit        = cfg->lookupValue (".core.friendship.func_mouse_hit",       1.8         );
    fship.func_scale            = cfg->lookupValue (".core.friendship.func_scale",           20          );
    fship.max_below             = cfg->lookupValue (".core.friendship.max_below_0",          5000        );
    fship.max_over              = cfg->lookupValue (".core.friendship.max_over_0",           5000        );
    fship.mouse_bad             = cfg->lookupValue (".core.friendship.mouse_bad",            5           );
    fship.mouse_good            = cfg->lookupValue (".core.friendship.mouse_good",           1           );
    fship.max_bul_reduction     = cfg->lookupValue (".core.friendship.max_bul_reduction",    60          );
    fship.stable                = cfg->lookupValue (".core.friendship.stable",               200         );
    fship.value                 = cfg->lookupValue (".core.friendship.value",                0           );
    fship.funboost              = cfg->lookupValue (".core.friendship.funboost",             0.5         );

    con->index->registerVariable((long long*)&fship.calm_perc_high, varIndex::Tuint, QString ("FSHIP:CALMH"), false);
    con->index->registerVariable((long long*)&fship.calm_perc_low, varIndex::Tuint, QString ("FSHIP:CALML"), false);
    con->index->registerVariable((long long*)&fship.calm_timer, varIndex::Tuint, QString ("FSHIP:CALMTIME"), false);
    con->index->registerVariable(&fship.func_calm_high, varIndex::Tdouble, QString ("FSHIP:FCALMH"), false);
    con->index->registerVariable(&fship.func_calm_low, varIndex::Tdouble, QString ("FSHIP:FCALML"), false);
    con->index->registerVariable(&fship.func_mouse_high, varIndex::Tdouble, QString ("FSHIP:FMOUSEH"), false);
    con->index->registerVariable(&fship.func_mouse_low, varIndex::Tdouble, QString ("FSHIP:FMOUSEL"), false);
    con->index->registerVariable(&fship.func_mouse_hit, varIndex::Tdouble, QString ("FSHIP:FMOUSEHIT"), false);
    con->index->registerVariable(&fship.func_scale, varIndex::Tdouble, QString ("FSHIP:FSCALE"), false);
    con->index->registerVariable((long long*)&fship.max_below, varIndex::Tuint, QString ("FSHIP:MAXBELOW"), false);
    con->index->registerVariable((long long*)&fship.max_over, varIndex::Tuint, QString ("FSHIP:MAXOVER"), false);
    con->index->registerVariable((long long*)&fship.max_bul_reduction, varIndex::Tuint, QString ("FSHIP:MAXRED"), false);
    con->index->registerVariable(&fship.stable, varIndex::Tdouble, QString ("FSHIP:STABLE"), false);
    con->index->registerVariable(&fship.value, varIndex::Tlongdouble, QString ("FSHIP:VAL"), false);
    con->index->registerVariable(&fship.funboost, varIndex::Tdouble, QString ("FSHIP:FUNBOOST"), false);


    //basic_sector

    cdbg_enabled            = cfg->lookupValue(".debug.cdebug.on",                       true        );
    hdbg_enabled            = cfg->lookupValue(".debug.HDBG_enabled",                    false       );

    //autocalc_sector

    autocalc.enabled                    = cfg->lookupValue(".core.autocalc.enabled",                         true        );
    autocalc.save_interval              = cfg->lookupValue(".core.autocalc.interval",                        300         );
    autocalc.start_delay                = cfg->lookupValue(".core.autocalc.delay",                           120         );
    cpu.ac.impact                       = cfg->lookupValue(".core.autocalc.cpu.impact",                      0.1         );
    cpu.ac.enabled                      = cfg->lookupValue(".core.autocalc.cpu.enabled",                     true        );
    cpu.ac.auto_degree                  = cfg->lookupValue(".core.autocalc.cpu.auto_angle",                  true        );
    cpu.ac.swalll                       = cfg->lookupValue(".core.autocalc.cpu.stable_wall_low",             50          );
    cpu.ac.swallh                       = cfg->lookupValue(".core.autocalc.cpu.stable_wall_high",            50          );
    cpu.ac.mult_low                     = cfg->lookupValue(".core.autocalc.cpu.multiple_low",                2           );
    cpu.ac.mult_low_converter           = cfg->lookupValue(".core.autocalc.cpu.low_converter",               1           );
    cpu.ac.mult_high                    = cfg->lookupValue(".core.autocalc.cpu.multiple_high",               2           );
    cpu.ac.mult_high_converter          = cfg->lookupValue(".core.autocalc.cpu.high_converter",              1           );
    memory.ac.impact                    = cfg->lookupValue(".core.autocalc.memory.impact",                   0.1         );
    memory.ac.enabled                   = cfg->lookupValue(".core.autocalc.memory.enabled",                  true        );
    memory.ac.auto_degree               = cfg->lookupValue(".core.autocalc.memory.auto_angle",               true        );
    memory.ac.swalll                    = cfg->lookupValue(".core.autocalc.memory.stable_wall_low",          50          );
    memory.ac.swallh                    = cfg->lookupValue(".core.autocalc.memory.stable_wall_high",         50          );
    memory.ac.mult_low                  = cfg->lookupValue(".core.autocalc.memory.multiple_low",             2           );
    memory.ac.mult_low_converter        = cfg->lookupValue(".core.autocalc.memory.low_converter",            1           );
    memory.ac.mult_high                 = cfg->lookupValue(".core.autocalc.memory.multiple_high",            2           );
    memory.ac.mult_high_converter       = cfg->lookupValue(".core.autocalc.memory.high_converter",           1           );
    battery.ac.impact                   = cfg->lookupValue(".core.autocalc.battery.impact",                  0.1         );
    battery.ac.enabled                  = cfg->lookupValue(".core.autocalc.battery.enabled",                 true        );
    battery.ac.auto_degree              = cfg->lookupValue(".core.autocalc.battery.auto_angle",              true        );
    battery.ac.swalll                   = cfg->lookupValue(".core.autocalc.battery.stable_wall_low",         50          );
    battery.ac.swallh                   = cfg->lookupValue(".core.autocalc.battery.stable_wall_high",        50          );
    battery.ac.mult_low                 = cfg->lookupValue(".core.autocalc.battery.multiple_low",            2           );
    battery.ac.mult_low_converter       = cfg->lookupValue(".core.autocalc.battery.low_converter",           1           );
    battery.ac.mult_high                = cfg->lookupValue(".core.autocalc.battery.multiple_high",           2           );
    battery.ac.mult_high_converter      = cfg->lookupValue(".core.autocalc.battery.high_converter",          1           );
    temperature.ac.impact               = cfg->lookupValue(".core.autocalc.temperature.impact",              0.1         );
    temperature.ac.enabled              = cfg->lookupValue(".core.autocalc.temperature.enabled",             true        );
    temperature.ac.auto_degree          = cfg->lookupValue(".core.autocalc.temperature.auto_angle",          true        );
    temperature.ac.swalll               = cfg->lookupValue(".core.autocalc.temperature.stable_wall_low",     50          );
    temperature.ac.swallh               = cfg->lookupValue(".core.autocalc.temperature.stable_wall_high",    50          );
    temperature.ac.mult_low             = cfg->lookupValue(".core.autocalc.temperature.multiple_low",        2           );
    temperature.ac.mult_low_converter   = cfg->lookupValue(".core.autocalc.temperature.low_converter",       1           );
    temperature.ac.mult_high            = cfg->lookupValue(".core.autocalc.temperature.multiple_high",       2           );
    temperature.ac.mult_high_converter  = cfg->lookupValue(".core.autocalc.temperature.high_converter",      1           );
    if (!autocalc.enabled)
    {
        cpu.ac.enabled = false;
        memory.ac.enabled = false;
        battery.ac.enabled = false;
        temperature.ac.enabled = false;
    }

    //eMu_sector

    eMu.cpu                 = cfg->lookupValue (".core.eMu_zone.cpu",                    false       );
    eMu.cpu_val             = cfg->lookupValue (".core.eMu_zone.cpu_val",                0           );
    eMu.mem                 = cfg->lookupValue (".core.eMu_zone.mem",                    false       );
    eMu.mem_val             = cfg->lookupValue (".core.eMu_zone.mem_val",                0           );
    eMu.temp                = cfg->lookupValue (".core.eMu_zone.temp",                   false       );
    eMu.temp_val            = cfg->lookupValue (".core.eMu_zone.temp_val",               0           );
    eMu.batt                = cfg->lookupValue (".core.eMu_zone.batt",                   false       );
    eMu.batt_val            = cfg->lookupValue (".core.eMu_zone.batt_val",               0           );
    eMu.batt_s              = cfg->lookupValue (".core.eMu_zone.batt_s",                 false       );
    eMu.batt_s_val          = cfg->lookupValue (".core.eMu_zone.batt_s_val",             0           );
    eMu.time                = cfg->lookupValue (".core.eMu_zone.time",                   false       );
    eMu.time_val            = cfg->lookupValue (".core.eMu_zone.time_val",               0           );
    eMu.energy              = cfg->lookupValue (".core.eMu_zone.energy",                 false       );
    eMu.energy_val          = cfg->lookupValue (".core.eMu_zone.energy_val",             0           );
    eMu.bulwers             = cfg->lookupValue (".core.eMu_zone.bulwers",                false       );
    eMu.bulwers_val         = cfg->lookupValue (".core.eMu_zone.bulwers_val",            0           );

    con->index->registerVariable(&eMu.cpu, varIndex::Tbool, QString ("3MU:CPU"), false);
    con->index->registerVariable((long long*)&eMu.cpu_val, varIndex::Tushort, QString ("3MU:CPUV"), false);
    con->index->registerVariable(&eMu.mem, varIndex::Tbool, QString ("3MU:MEM"), false);
    con->index->registerVariable((long long*)&eMu.mem_val, varIndex::Tushort, QString ("3MU:MEMV"), false);
    con->index->registerVariable(&eMu.temp, varIndex::Tbool, QString ("3MU:TEMP"), false);
    con->index->registerVariable((long long*)&eMu.temp_val, varIndex::Tushort, QString ("3MU:TEMPV"), false);
    con->index->registerVariable(&eMu.batt, varIndex::Tbool, QString ("3MU:BAT"), false);
    con->index->registerVariable((long long*)&eMu.batt_val, varIndex::Tushort, QString ("3MU:BATV"), false);
    con->index->registerVariable(&eMu.batt_s, varIndex::Tbool, QString ("3MU:BATS"), false);
    con->index->registerVariable((long long*)&eMu.batt_s_val, varIndex::Tushort, QString ("3MU:BATSV"), false);
    con->index->registerVariable(&eMu.time, varIndex::Tbool, QString ("3MU:TIME"), false);
    con->index->registerVariable((long long*)&eMu.time_val, varIndex::Tushort, QString ("3MU:TIMEV"), false);
    con->index->registerVariable(&eMu.energy, varIndex::Tbool, QString ("3MU:NRG"), false);
    con->index->registerVariable((long long*)&eMu.energy_val, varIndex::Tushort, QString ("3MU:NRGV"), false);
    con->index->registerVariable(&eMu.bulwers, varIndex::Tbool, QString ("3MU:FACE"), false);
    con->index->registerVariable((long long*)&eMu.bulwers_val, varIndex::Tushort, QString ("3MU:FACEV"), false);

    //mousea_actions_sector

    mousea.buff_size        = cfg->lookupValue (".core.mouse_actions.buff_size",         120         );
    mousea.scale            = cfg->lookupValue (".core.mouse_actions.scale",             8000        );
    mousea.wall             = cfg->lookupValue (".core.mouse_actions.wall",              500         );
    mousea.force_wall       = cfg->lookupValue (".core.mouse_actions.force_wall",        3000        );
    mousea.opt_speed        = cfg->lookupValue (".core.mouse_actions.opt_speed",         250         );
    mousea.impact           = cfg->lookupValue (".core.mouse_actions.impact",            10          );
    mousea.heavycalm        = cfg->lookupValue (".core.mouse_actions.heavycalm",         100         );
    mousea.max_delay        = cfg->lookupValue (".core.mouse_actions.max_delay",         400         );
    mousea.max_hpp_bul      = cfg->lookupValue (".core.mouse_actions.max_hpp_bul",       5           );

    // hardware_section

    HRDWR.battery_capacity  = cfg->lookupValue ( ".core.hardware.batt_capacity",         4700        );
    HRDWR.special_batname   = cfg->lookupValue ( ".core.hardware.use_batname",           false       );
    HRDWR.special_thername  = cfg->lookupValue ( ".core.hardware.use_thername",          false       );
    HRDWR.cfg_battname      = cfg->lookupValue ( ".core.hardware.battname",              "BAT0"      );
    HRDWR.ignore_bat_state  = cfg->lookupValue ( ".core.hardware.ignore_bat_state",      false       );
    HRDWR.cfg_thername      = cfg->lookupValue ( ".core.hardware.thername",              "thermal_zone0"        );
    HRDWR.backlight_path    = cfg->lookupValue ( ".core.rootcontrol.backlight_device",   "intel_backlight"      );

    //rootcontrol

    rtctrl.screenctrl       = cfg->lookupValue ( ".core.rootcontrol.screen_management",  true        );
    rtctrl.batt_min_backl   = cfg->lookupValue ( ".core.rootcontrol.batt_min_backl",     20          );
    rtctrl.batt_start_perc  = cfg->lookupValue ( ".core.rootcontrol.batt_start_perc",    25          );
    rtctrl.batt_suspend_perc= cfg->lookupValue ( ".core.rootcontrol.batt_suspend_perc",  5           );
    rtctrl.roottype         = cfg->lookupValue ( ".core.rootcontrol.roottype",           false       );
    rtctrl.suspendtohdd     = cfg->lookupValue ( ".core.rootcontrol.suspendtohdd",       false       );
    rtctrl.temp_halt_enabled= cfg->lookupValue ( ".core.rootcontrol.temp_halt_enabled",  true        );
    rtctrl.temp_halt_start  = cfg->lookupValue ( ".core.rootcontrol.temp_halt_start",    85          );
    rtctrl.scrnsaver_management  = cfg->lookupValue ( ".core.rootcontrol.screensaver_management",  true        );
    rtctrl.customshell      = cfg->lookupValue ( ".core.rootcontrol.custom_shell",    false          );
    rtctrl.shellname        = cfg->lookupValue ( ".core.rootcontrol.shell_name",      "sh -c "       );
    rtctrl.scrnsav_X        = cfg->lookupValue ( ".core.rootcontrol.manage_xscreensaver",          true        );
    rtctrl.scrnsav_kde      = cfg->lookupValue ( ".core.rootcontrol.manage_kde_screensaver",       true        );
    rtctrl.scrnsav_gnome    = cfg->lookupValue ( ".core.rootcontrol.manage_gnome_screensaver",     true        );
    rtctrl.scrnsav_mac      = cfg->lookupValue ( ".core.rootcontrol.manage_mac_screensaver",       true        );
    rtctrl.scrnsav_custom   = cfg->lookupValue ( ".core.rootcontrol.manage_custom_screensaver",   false        );
    rtctrl.scrnsav_custom_on     = cfg->lookupValue ( ".core.rootcontrol.manage_custom_screensaver_on",   "xscreensaver-command -activate"        );
    rtctrl.scrnsav_custom_off    = cfg->lookupValue ( ".core.rootcontrol.manage_custom_screensaver_off",  "xscreensaver-command -deactivate"      );

    //flue

    flue.active             = cfg->lookupValue (".core.flue.active",                     false       );
    flue.last_date.day      = cfg->lookupValue (".core.flue.last_date.day",              0           );
    flue.last_date.month    = cfg->lookupValue (".core.flue.last_date.month",            0           );
    flue.last_date.year     = cfg->lookupValue (".core.flue.last_date.year",             0           );
    flue.last_date.lenght   = cfg->lookupValue (".core.flue.last_date.lenght",           3           );
    flue.last_date.minute_perc = cfg->lookupValue (".core.flue.last_date.perc_per_min",  0.034       );
    flue.last_date.invertion_step = cfg->lookupValue (".core.flue.last_date.invertion_per_min",  0.005       );
    flue.last_date.progress = cfg->lookupValue (".core.flue.last_date.progress",         0.0         );
    flue.amplitude          = cfg->lookupValue (".core.flue.temperature_max_amplitude",  20          );
    flue.highval            = cfg->lookupValue (".core.flue.temperature_highest",        (double)(temperature.stable+1));
    flue.lowval             = cfg->lookupValue (".core.flue.temperature_lowest",         (double)(temperature.stable-1));
    flue.update_impact      = cfg->lookupValue (".core.flue.update_impact",              0.2         );
    flue.bul_impact         = cfg->lookupValue (".core.flue.bulwers_impact",             0.001       );
    flue.fun_impact         = cfg->lookupValue (".core.flue.fun_impact",                 0.2         );
    flue.pet_impact         = cfg->lookupValue (".core.flue.pet_impact",                 -0.5        );
    flue.hit_impact         = cfg->lookupValue (".core.flue.hit_impact",                 100.0       );
    flue.max_bul_booster    = cfg->lookupValue (".core.flue.max_bulwers_booster",        10.0        );
    flue.invertion_perc     = cfg->lookupValue (".core.flue.invertion_perc",             0.01        );
    flue.visual_impact_probability_1     = cfg->lookupValue (".core.flue.visual_impact_probability_1",             5        );
    flue.visual_impact_probability_2     = cfg->lookupValue (".core.flue.visual_impact_probability_2",             4        );
    flue.visual_impact_probability_3     = cfg->lookupValue (".core.flue.visual_impact_probability_3",             3        );
    flue.visual_impact_probability_4     = cfg->lookupValue (".core.flue.visual_impact_probability_4",             2        );
    flue.visual_impact_probability_5     = cfg->lookupValue (".core.flue.visual_impact_probability_5",             1        );
    flue.step_perc_1        = cfg->lookupValue (".core.flue.step_perc.lvl_1",            50          );
    flue.step_perc_2        = cfg->lookupValue (".core.flue.step_perc.lvl_2",            70          );
    flue.step_perc_3        = cfg->lookupValue (".core.flue.step_perc.lvl_3",            80          );
    flue.step_perc_4        = cfg->lookupValue (".core.flue.step_perc.lvl_4",            90          );
    flue.step_perc_5        = cfg->lookupValue (".core.flue.step_perc.lvl_5",            100         );
}

void handler (int signal)
{
    info << "signal " << signal << " caught.\n";
    if (signal == 2 || signal == 15 || signal == 3)
    {
        info << "(eyes) close event recived, exiting...\n";
        Configuration::getInstance ()->save ();
        if (ccap.cam != NULL)
            cvReleaseCapture(&ccap.cam);
        cout << "\033[0m";
        exit(1);
    }
    else if (signal == 6)
    {
        cerr << "(eyes) crash!\n";
        if (ccap.cam != NULL)
        {
            info << "Releasing camera...\n";
            cvReleaseCapture(&ccap.cam);
        }
    }
    else
    {
        cerr << "unsupported signal caught! Emergrncy saving...\n";
        Configuration::getInstance ()->save ();
    }
    return ;
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
    s_anim.face_prev = "slp_10";
    info << "s_anim.face_prev set to " << s_anim.face_prev.toStdString() << "\n";
    info << "(core) wake up finished\n";
    if (!core_only_mode)
        eyes->anims_send ("cusual_01", "slp_10_close", "cusual_01_open", 0, 4);
    info << "(core) gui inited\n";
    HRDWR.system_check();
    info << "(core) end of core preparing\n";
    timer->start ( 1000 );
    _cdbg = new cdbg ( this );

    struct sigaction sa;
    sa.sa_handler = handler;
    sigfillset(&(sa.sa_mask));
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);
    sigaction(SIGQUIT, &sa, 0);
    sigaction(SIGABRT, &sa, 0);
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
        QProcess::execute ("kdesu", extendedarg);
    }
}

void rootcontrol::shell(string command)
{
    string newcommand = shellname;
    newcommand += "\"";
    newcommand += command;
    newcommand += "\"";
    execute(1, QString ( &newcommand[0] ));
}

void rootcontrol::action(string command)
{
    if (command == "halt")
    {
        execute(0, QString ( "halt" ));
        return;
    }

    if (command == "suspend")
    {
        execute(roottype, QString ( "halt" ));
        return;
    }

    if (command == "battery")
    {
        if (battery.load < batt_start_perc && screenctrl && core_step > 1 && battery_state != 0)
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
        return;
    }

    if (command == "temperature" && temp_halt_enabled)
    {
        QStringList args;
        if (temperature.value > temp_halt_start)
        {
            args << "-h" << "now";
            execute(1, "/sbin/shutdown", args);
        }
        return;
    }

    execute(1, QString ( &command[0] ));
}

void rootcontrol::shelldetect()
{
    if (!customshell)
    {
        shellname = "sh -c ";
    }
    info << "shell set to: " << shellname << "\n";
}

bool bul::check_env(bool enabled, Configuration * cfg)
{
    if (!enabled)
        return 0;
    bool retstat = false;
    if (envs.size() == 0)
    {
        environment_data newenv;
        newenv.Rperc = 100*ccap.env.Rperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.Yperc = 100*ccap.env.Yperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.Gperc = 100*ccap.env.Gperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.Bperc = 100*ccap.env.Bperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.Pperc = 100*ccap.env.Pperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.Hperc = 100*ccap.env.Hperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        newenv.spenttime = 0;
        newenv.timer = 0;
        envs.push_back(newenv);
        info << "First environment added:\n"
             << envs[0].Rperc << "% red\n"
             << envs[0].Yperc << "% yellow\n"
             << envs[0].Gperc << "% green\n"
             << envs[0].Bperc << "% blue\n"
             << envs[0].Pperc << "% pink\n"
             << envs[0].Hperc << "% grey\n";
        return 0;
    }
    else
    {
        // ---search for existing environment
        envindex = -1;
        env_max_compability = 0.0;
        long long spenttime = 0;
        curenv.Rperc = 100*ccap.env.Rperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        curenv.Yperc = 100*ccap.env.Yperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        curenv.Gperc = 100*ccap.env.Gperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        curenv.Bperc = 100*ccap.env.Bperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        curenv.Pperc = 100*ccap.env.Pperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        curenv.Hperc = 100*ccap.env.Hperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
        for (int i = 0; i<envs.size();i++)
        {
            double compability = 0;
            int possible = 6;
            spenttime += envs[i].spenttime;
            if ((curenv.Rperc > 10 && envs[i].Rperc > 10) || (curenv.Rperc > 25 || envs[i].Rperc > 25))
            {
                if (curenv.Rperc < envs[i].Rperc)
                {
                    compability+=100.0-(100*(-(curenv.Rperc - envs[i].Rperc))/envs[i].Rperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Rperc - envs[i].Rperc)/curenv.Rperc);
                //cerr << "R: " << compability << "\n";
            }
            else
                possible--;

            if ((curenv.Yperc > 10 && envs[i].Yperc > 10) || (curenv.Yperc > 25 || envs[i].Yperc > 25))
            {
                if (curenv.Yperc < envs[i].Yperc)
                {
                    compability+=100.0-(100*(-(curenv.Yperc - envs[i].Yperc))/envs[i].Yperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Yperc - envs[i].Yperc)/curenv.Yperc);
                //cerr << "Y: " << compability << "\n";
            }
            else
                possible--;

            if ((curenv.Gperc > 10 && envs[i].Gperc > 10) || (curenv.Gperc > 25 || envs[i].Gperc > 25))
            {
                if (curenv.Gperc < envs[i].Gperc)
                {
                    compability+=100.0-(100*(-(curenv.Gperc - envs[i].Gperc))/envs[i].Gperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Gperc - envs[i].Gperc)/curenv.Gperc);
                //cerr << "G: " << compability << "\n";
            }
            else
                possible--;

            if ((curenv.Bperc > 10 && envs[i].Bperc > 10) || (curenv.Bperc > 25 || envs[i].Bperc > 25))
            {
                if (curenv.Bperc < envs[i].Bperc)
                {
                    compability+=100.0-(100*(-(curenv.Bperc - envs[i].Bperc))/envs[i].Bperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Bperc - envs[i].Bperc)/curenv.Bperc);
                //cerr << "B: " << compability << "\n";
            }
            else
                possible--;

            if ((curenv.Pperc > 10 && envs[i].Pperc > 10) || (curenv.Pperc > 25 || envs[i].Pperc > 25))
            {
                if (curenv.Pperc < envs[i].Pperc)
                {
                    compability+=100.0-(100*(-(curenv.Pperc - envs[i].Pperc))/envs[i].Pperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Pperc - envs[i].Pperc)/curenv.Pperc);
                //cerr << "P: " << compability << "\n";
            }
            else
                possible--;

            if (curenv.Hperc > 10 && envs[i].Hperc > 10)
            {
                if (curenv.Hperc < envs[i].Hperc)
                {
                    compability+=100.0-(100*(-(curenv.Hperc - envs[i].Hperc))/envs[i].Hperc);
                }
                else
                    compability+=100.0-(100.0*(curenv.Hperc - envs[i].Hperc)/curenv.Hperc);
                //cerr << "H: " << compability << "\n";
            }
            else
                possible--;

            if (possible > 0)
                compability/=(double)possible;
            //cerr << compability << "\n";
            //cerr << envs[i].Rperc << "->" << curenv.Rperc << "\n"
            //     << envs[i].Yperc << "->" << curenv.Yperc << "\n"
            //     << envs[i].Gperc << "->" << curenv.Gperc << "\n"
            //     << envs[i].Bperc << "->" << curenv.Bperc << "\n"
            //     << envs[i].Pperc << "->" << curenv.Pperc << "\n"
            //     << envs[i].Hperc << "->" << curenv.Hperc << "\n";
            if (compability > env_max_compability && compability > env_min_compability)
            {
                env_max_compability = compability;
                envindex = i;
            }
        }
        if (envindex == -1)
        {
            info << "New environment mapped - adding to catalogue...\n";
            environment_data newenv;
            newenv.Rperc = 100*ccap.env.Rperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.Yperc = 100*ccap.env.Yperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.Gperc = 100*ccap.env.Gperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.Bperc = 100*ccap.env.Bperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.Pperc = 100*ccap.env.Pperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.Hperc = 100*ccap.env.Hperc/(100.0-ccap.env.Lperc-ccap.env.Dperc);
            newenv.spenttime = 0;
            newenv.timer = 0;
            envs.push_back(newenv);
            info << "New environment added:\n";
            retstat = 1;
            envindex = envs.size()-1;
        }
        else
        {
            //info << "Mapped to environment " << envindex << "\n";
            envs[envindex].Rperc=(double)envs[envindex].Rperc*(100.0-env_update_impact)/100.0 + (double)curenv.Rperc*env_update_impact/100.0;
            envs[envindex].Yperc=(double)envs[envindex].Yperc*(100.0-env_update_impact)/100.0 + (double)curenv.Yperc*env_update_impact/100.0;
            envs[envindex].Gperc=(double)envs[envindex].Gperc*(100.0-env_update_impact)/100.0 + (double)curenv.Gperc*env_update_impact/100.0;
            envs[envindex].Bperc=(double)envs[envindex].Bperc*(100.0-env_update_impact)/100.0 + (double)curenv.Bperc*env_update_impact/100.0;
            envs[envindex].Pperc=(double)envs[envindex].Pperc*(100.0-env_update_impact)/100.0 + (double)curenv.Pperc*env_update_impact/100.0;
            envs[envindex].Hperc=(double)envs[envindex].Hperc*(100.0-env_update_impact)/100.0 + (double)curenv.Hperc*env_update_impact/100.0;
            envs[envindex].timer++;
            if (envs[envindex].timer > 3599)
            {
                envs[envindex].timer = 0;
                envs[envindex].spenttime++;
            }
            if (envs[envindex].spenttime < spenttime*env_max_exotic_spenttime/100.0)
                retstat = 1;
            else
                retstat = 0;
        }
        if (core_step%env_saveinterval == 0)
        {
            cerr << "Dropping environment...";
            for (int i = 0; i < envs.size(); i++)
            {
                stringstream ss;
                ss << i;
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Rperc")[0], (int)envs[i].Rperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Yperc")[0], (int)envs[i].Yperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Gperc")[0], (int)envs[i].Gperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Bperc")[0], (int)envs[i].Bperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Pperc")[0], (int)envs[i].Pperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".Hperc")[0], (int)envs[i].Hperc );
                cfg->setValue ( &(".core.environment.env"+ss.str()+".spenttime")[0], (int)envs[i].spenttime );
            }
            cfg->setValue ( ".core.bulwers.envs_number", (int)envs.size() );
            bulwers.force_autosave = true;
        }
        return retstat;
    }
}

void bul::fun_check()
{
    if (!ccap.enabled)
        return;
    if (ccap.fun.fun > hpp_fun_perc_2)
        hpp = 2;
    else if (ccap.fun.fun > hpp_fun_perc_1)
        hpp = 1;
    else
        hpp = 0;
    if (hpp > 0)
    {
        fship.value+=fship.value*fship.funboost/100.0;
    }
}
