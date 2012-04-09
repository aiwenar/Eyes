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

#include <fstream>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/proclist.h>
#include <glibtop/uptime.h>
#include "core.hxx"
#include "hardware.hxx"
#include "debug.hxx"

hardware        HRDWR;
sdate           get_time    ();

string hardware::get_file (char* path)
{
        fstream file (path, fstream::in);
                string out;
                while (file.good())
                        out+=file.get();
        return out;
}

unsigned short hardware::proc_temp (string path)
{
    string input = get_file(&path[0]);
        string temp = "";
        int x = input.find ("temperature:");
        x+=9;
        while (input[x] == ' ')
                x++;
        for (int i = 0; i<2; i++)
        {
                if (input[x+i] == ' ')
                        break;
                temp += input[x+i];
        }
        return atoi (&temp[0]);
}

unsigned short hardware::proc_bat_now (string path)
{
    string input = get_file(&path[0]);
        string bat = "";
        int x = input.find ("capacity:");
        x+=9;
        while (input[x] == ' ')
                x++;
        for (int i = 0; i<10; i++)
        {
                if (input[x+i] == ' ')
                        break;
                bat += input[x+i];
        }
        return atoi (&bat[0]);
}

unsigned short hardware::proc_bat_full (string path)
{
    string input = get_file(&path[0]);
        string bat = "";
        int x = input.find ("last full capacity:");
        x+=19;
        while (input[x] == ' ')
                x++;
        for (int i = 0; i<10; i++)
        {
                if (input[x+i] == ' ')
                        break;
                bat += input[x+i];
        }
        return atoi (&bat[0]);
}

unsigned short hardware::proc_bat_state (string path)
{
    string input = get_file(&path[0]);
        int x = input.find ("present:");
        x+=8;
        while (input[x] == ' ')
                x++;
        if (input[x] == 'n')
                return 0;

        x = input.find ("charging state:");
        x+=15;
        while (input[x] == ' ')
                x++;

        static char pluged = 0;

        if (input[x] == 'c' && pluged !=1) //battery has been just pluged
        {
                pluged = 1;
                return 1;
        }
        if (input[x] != 'c' && pluged !=0) //battery has been just unpluged
        {
                pluged = 0;
                return 2;
        }
        if (input[x] == 'c' && pluged == 1) //battery is still pluged
                return 3;
        if (input[x] != 'c' && pluged == 0) //battery is still unpluged
                return 4;

        return 5;
}

unsigned short hardware::sys_temp (string path)
{
    string input = get_file(&path[0]);
        string temp = "";
        for (int i = 0; i<input.size()-2; i++)
                temp+=input[i];
        return atoi (&temp[0])/1000;
}

unsigned short hardware::sys_bat_uni (string path)
{
    string input = get_file(&path[0]);
        string bat = "";
        for (int i = 0; i<input.size()-2; i++)
                bat+=input[i];
        return atoi (&bat[0])/1000;
}

unsigned short hardware::sys_bat_state (string path)
{
    string input = get_file(&path[0]);
        if (input == "")
            return 0;

        static char pluged = 0;

        if (input[0] == 'C' && pluged !=1) //battery has been just pluged
        {
            pluged = 1;
            return 1;
        }
        if (input[0] != 'C' && pluged !=0) //battery has been just unpluged
        {
            pluged = 0;
            return 2;
        }
        if (input[0] == 'C' && pluged == 1) //battery is still pluged
            return 3;
        if (input[0] != 'C' && pluged == 0) //battery is still unpluged
            return 4;

        return 5;
}

double hardware::C_LOAD ()
{
  glibtop_init();
  glibtop_cpu gcpu;
  glibtop_get_cpu (&gcpu);

  static double cpu_load = 0;
  static unsigned long p_idle = 0;
  static unsigned long p_total = 0;
  static unsigned long a_idle = 0;
  static unsigned long a_total = 0;
  static unsigned long d_total = 0;
  static unsigned long d_idle = 0;
  a_idle = gcpu.idle;
  a_total = gcpu.total;

  d_total = a_total - p_total;
  d_idle = a_idle - p_idle;
  cpu_load = (100*(d_total - d_idle)) / d_total;
  p_idle = a_idle;
  p_total = a_total;

  return cpu_load;
}

double hardware::M_LOAD ()
{
  glibtop_init();
  glibtop_mem memory;
  glibtop_get_mem(&memory);

  /*unsigned int mem_total = memory.total;
  unsigned int mem_used = memory.user;
  unsigned int mem_load = mem_used/mem_total;
  */
  return 100*( (double)memory.user / (double)memory.total );
}

unsigned short hardware::P_LIST ()
{
  int which,arg;

  glibtop_init();
  glibtop_proclist proclist;
  glibtop_get_proclist(&proclist,which,arg);

  return proclist.number;
}

int hardware::U_TIME ()
{
  glibtop_init();
  glibtop_uptime uptime;
  glibtop_get_uptime(&uptime);

  return uptime.uptime;
}

sdate get_time ()
{
    //static int initialized = 0;
    //if (initialized == 0){
    time_t timer;

    timer=time(NULL);

    //initialized = 1;

    sdate get_date;
    QString tmp1 = asctime ( localtime ( &timer ) );
    QStringList l_date ( tmp1.split ( ' ' ) );
    if ( l_date[0] == "Mon" )get_date.day = 1;
    if ( l_date[0] == "Tue" )get_date.day = 2;
    if ( l_date[0] == "Wed" )get_date.day = 3;
    if ( l_date[0] == "Thu" )get_date.day = 4;
    if ( l_date[0] == "Fri" )get_date.day = 5;
    if ( l_date[0] == "Sat" )get_date.day = 6;
    if ( l_date[0] == "Sun" )get_date.day = 7;

    if ( l_date[1] == "Jan" )get_date.month = 1;
    if ( l_date[1] == "Feb" )get_date.month = 2;
    if ( l_date[1] == "Mar" )get_date.month = 3;
    if ( l_date[1] == "Apr" )get_date.month = 4;
    if ( l_date[1] == "May" )get_date.month = 5;
    if ( l_date[1] == "Jun" )get_date.month = 6;
    if ( l_date[1] == "Jul" )get_date.month = 7;
    if ( l_date[1] == "Aug" )get_date.month = 8;
    if ( l_date[1] == "Sep" )get_date.month = 9;
    if ( l_date[1] == "Oct" )get_date.month = 10;
    if ( l_date[1] == "Nov" )get_date.month = 11;
    if ( l_date[1] == "Dec" )get_date.month = 12;

    if (l_date[2] == 0)
    {
        get_date.day_num = l_date[3].toInt ();
        QStringList tmp2 ( l_date[4].split ( ':' ) );
        get_date.hour = ( 3600 * ( tmp2[0].toInt () ) + ( 60 * ( tmp2[1].toInt() ) ) + tmp2[2].toInt () );
        get_date.year = l_date[5].toInt ();
    }
    else
    {
        get_date.day_num = l_date[2].toInt ();
        QStringList tmp2 ( l_date[3].split ( ':' ) );
        get_date.hour = ( 3600 * ( tmp2[0].toInt () ) + ( 60 * ( tmp2[1].toInt() ) ) + tmp2[2].toInt () );
        get_date.year = l_date[4].toInt ();
    }

    return get_date;
}



unsigned short hardware::temperatura ()
{
    return (this->*src_temp)(final_path_temp);
}

int hardware::bat_plugged ()
{
    return (this->*src_batt_state)(final_path_state);
}

int hardware::bateria ()
{
    return 100*(this->*src_batt_now)(final_path_now)/battery_capacity;
}

unsigned short hardware::emubat(string path)
{
    return battery_capacity*battery.stable/100;
}

unsigned short hardware::emustat(string path)
{
    return 4;
}

unsigned short hardware::emutemp(string path)
{
    return temperature.stable;
}



unsigned int percental::convert(unsigned short val)
{
    mod_prev = 0;
    mod = 0;
    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (val <= stable - loseless - (i*((stable - loseless)/steps)))
        {
            switch (frequency)
            {
            case 'l':
                mod = -i*lin_num;
            case 'q':
                mod = -i*i;
            case 'f':
                mod = -i + mod_prev;
            }
        }
    }

    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (val >= stable + loseless + (i*((100-(stable + loseless))/steps)))
        {
            switch (frequency)
            {
            case 'l':
                mod = i*lin_num;
            case 'q':
                mod = i*i;
            case 'f':
                mod = i + mod_prev;
            }
        }

    }
    return mod;
}



unsigned int percental::calculate ()
{
    int curstep = 0;
    int perc = 0;
    for (int i = 0; i<=EQsize; i++)
    {
        if (load >= (100/EQsize)*i)
            curstep = i;
        else
            break;
    }
    if (curstep == EQsize)
        return EQ[EQsize];
    else
    {
        perc = 100*(load-(100/EQsize)*curstep)/(100/EQsize);
        return EQ[curstep] + perc*(EQ[curstep+1]-EQ[curstep])/100;
    }
}

unsigned int unital::calculate()
{
    int curstep = 0;
    int perc = 0;
    if (value < EQbegin)
        return EQ[0];
    for (int i = 0; i<=EQsize; i++)
    {
        if (value >= EQbegin + ((EQend-EQbegin)/EQsize)*i)
            curstep = i;
        else
            break;
    }
    if (curstep == EQsize)
        return EQ[EQsize];
    else
    {
        perc = 100*(value-(EQbegin + ((EQend-EQbegin)/EQsize)*curstep))/((EQend-EQbegin)/EQsize);
        return EQ[curstep] + perc*(EQ[curstep+1]-EQ[curstep])/100;
    }
}



unsigned int unital::convert(unsigned short val)
{
    mod = 0;
    mod_prev = 0;
    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (val <= stable - loseless - (i*unit))
        {
            switch (frequency)
            {
            case 'l':
                mod = -i*lin_num;
            case 'q':
                mod = -i*i;
            case 'f':
                mod = -i + mod_prev;
            }
        }
    }

    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (val >= stable + loseless + (i*unit))
        {
            switch (frequency)
            {
            case 'l':
                mod = i*lin_num;
            case 'q':
                mod = i*i;
            case 'f':
                mod = i + mod_prev;
            }
        }
    }
    return mod;
}



unsigned int timal::calculate()
{

    if (value > start)
    {
        if (frequency == 'l')
        {
            for (unsigned short i = 0; i<=steps; i++)
            {
                if (value >= (start + (i*(wide/steps))))
                    mod = i;
            }
        }
        if (frequency == 'q')
        {
            unsigned short wall = start;
            unsigned short sector = wide;

            for (unsigned short i = 0; i<= steps; i++)
            {
                if (value >= wall + (sector/2))
                {
                    sector /= 2;
                    wall += sector;
                    mod = i;
                }

            }
        }
    }
    if (value <= end)
    {
        switch (frequency)
        {
        case 'l':
            {
                for (unsigned short i = steps; i>=0; i--)
                {
                    if (value >= (end - (i*(wide/steps))))
                        mod = i;
                }
            }
        case 'q':
            {
                unsigned short wall = end;
                unsigned short sector = wide;

                for (unsigned short i = steps+1; i> 0; i--)
                {
                    if (value >= 0 + (sector/2))
                    {
                        sector /= 2;
                        wall += sector;
                        mod = i;
                    }
                    else break;
                }
            }
        }
    }
    return mod;
}

void hardware::system_check()
{
    info << "System checking...\n";
        final_now_solution = 0;
        final_full_solution = 0;
        final_state_solution = 0;
        final_temp_solution = 0;

        for (int i = 0; i < 10; i++)
        {
                string input = "";
                string path = "/proc/acpi/battery/BAT";
                path += i+48;
                path += "/state";
                input = get_file (&path[0]);
                if (input == "")
                        info << "proc(BAT" << i << ") - failed\n";
                else
                {
                        final_now_solution = 1;
                        final_state_solution = 1;
                        final_path_now = path;
                        final_path_full = "/proc/acpi/battery/BAT";
                        final_path_full += i+48;
                        final_path_full += "/info";
                        info << "battery found on: " << path << "\n";
                        break;
                }
        }
        if (special_batname)
        {
                string input = "";
                string path = "/proc/acpi/battery/";
                path += cfg_battname;
                path += "/state";
                input = get_file (&path[0]);
                if (input == "")
                        info << "searching for custom battery path - failed\n";
                else
                {
                        final_now_solution = 1;
                        final_state_solution = 1;
                        final_path_now = path;
                        final_path_full = "/proc/acpi/battery/";
                        final_path_full += cfg_battname;
                        final_path_full += "/info";
                        info << "searching for custom battery path - success\n";
                }
        }
        final_path_state = final_path_full;
        string input = "";
        input = get_file (&final_path_full[0]);
        if (input == "")
                info << "battery capacity searching - failed\n";
        else
        {
                final_full_solution = 1;
                info << "battery capacity searching - success\n";
        }

        /*
        if (final_now_solution == 1)
                cout << proc_bat_now (get_file (&final_path_now[0])) << "\n";
        if (final_full_solution == 1)
                cout << proc_bat_full (get_file (&final_path_full[0])) << "\n";
        if (final_state_solution == 1)
                cout << proc_bat_state (get_file (&final_path_state[0])) << "\n";
        */


        for (int i = 0; i < 10; i++)
        {
                string input = "";
                string path = "/sys/class/power_supply/BAT";
                path += i+48;
                path += "/charge_now";
                input = get_file (&path[0]);
                if (input == "")
                        info << "sys(BAT" << i << ") - failed\n";
                else
                {
                        final_now_solution = 2;
                        final_path_now = path;
                        final_path_full = "/sys/class/power_supply/BAT";
                        final_path_full += i+48;
                        final_path_full += "/charge_full";
                        final_path_state = "/sys/class/power_supply/BAT";
                        final_path_state += i+48;
                        final_path_state += "/status";
                        info << "battery found on: " << path << "\n";
                        break;
                }
        }
        if (special_batname)
        {
                string input = "";
                string path = "/sys/class/power_supply/";
                path += cfg_battname;
                path += "/charge_now";
                input = get_file (&path[0]);
                if (input == "")
                        info << "searching for custom battery path - failed\n";
                else
                {
                        final_now_solution = 2;
                        final_path_now = path;
                        final_path_full = "/sys/class/power_supply/";
                        final_path_full += cfg_battname;
                        final_path_full += "/charge_full";
                        final_path_state = "/sys/class/power_supply/";
                        final_path_state += cfg_battname;
                        final_path_state += "/status";
                        info << "searching for custom battery path - success\n";
                }
        }
        input = "";
        input = get_file (&final_path_full[0]);
        if (input == "")
                info << "battery capacity searching - failed\n";
        else
        {
                final_full_solution = 2;
                info << "battery capacity searching - success\n";
        }
        input = "";
        input = get_file (&final_path_state[0]);
        if (input == "")
                info << "battery status searching - failed\n";
        else
        {
                final_state_solution = 2;
                info << "battery status searching - success\n";
        }

        /*
        if (final_now_solution == 2)
                cout << sys_bat_uni (get_file (&final_path_now[0])) << "\n";
        if (final_full_solution == 2)
                cout << sys_bat_uni (get_file (&final_path_full[0])) << "\n";
        if (final_state_solution == 2)
                cout << sys_bat_state (get_file (&final_path_state[0])) << "\n";
        */

        if (final_now_solution == 0)
            cerr << "No battery found!\n";

        switch (final_now_solution)
        {
        case 0:
            src_batt_now = &hardware::emubat;
            info << "battery source: emubat\n";
            break;
        case 1:
            src_batt_now = &hardware::proc_bat_now;
            info << "battery source: proc\n";
            break;
        case 2:
            src_batt_now = &hardware::sys_bat_uni;
            info << "battery source: sys\n";
            break;
        }
        switch (final_full_solution)
        {
        case 0:
            battery_capacity = battery_capacity;
            info << "battery full: config capacity\n";
            break;
        case 1:
            battery_capacity = proc_bat_full(final_path_full);
            info << "battery full: proc\n";
            break;
        case 2:
            battery_capacity = sys_bat_uni(final_path_full);
            info << "battery full: sys\n";
            break;
        }
        switch (final_state_solution)
        {
        case 0:
            src_batt_state = &hardware::emustat;
            info << "battery state: emustate\n";
            break;
        case 1:
            src_batt_state = &hardware::proc_bat_state;
            info << "battery state: proc\n";
            break;
        case 2:
            src_batt_state = &hardware::sys_bat_state;
            info << "battery state: sys\n";
            break;
        }

        for (int i = 0; i < 10; i++)
        {
                string input = "";
                string path = "/proc/acpi/thermal_zone/TZ0";
                path += i+48;
                path += "/temperature";
                input = get_file (&path[0]);
                if (input == "")
                        info << "proc(TZ0" << i << ") - failed\n";
                else
                {
                        final_temp_solution = 1;
                        final_path_temp = path;
                        info << "temperature sensor found on: " << path << "\n";
                        break;
                }
        }
        if (special_thername)
        {
                string input = "";
                string path = "/proc/acpi/thermal_zone/";
                path += cfg_thername;
                path += "/temperature";
                input = get_file (&path[0]);
                if (input == "")
                        info << "searching for custom battery path - failed\n";
                else
                {
                        final_temp_solution = 1;
                        final_path_temp = path;
                        info << "searching for custom battery path - success\n";
                }
        }

        /*
        if (final_temp_solution == 1)
                cout << proc_temp (get_file (&final_path_temp[0])) << "\n";
        */

        for (int i = 0; i < 10; i++)
        {
                string input = "";
                string path = "/sys/class/thermal/thermal_zone";
                path += i+48;
                path += "/temp";
                input = get_file (&path[0]);
                if (input == "")
                        info << "sys(thermal_zone" << i << ") - failed\n";
                else
                {
                        final_temp_solution = 2;
                        final_path_temp = path;
                        info << "thermal sensor found on: " << path << "\n";
                        break;
                }
        }
        if (special_batname)
        {
                string input = "";
                string path = "/sys/class/thermal/";
                path += cfg_battname;
                path += "/temp";
                input = get_file (&path[0]);
                if (input == "")
                        info << "searching for custom battery path - failed\n";
                else
                {
                        final_temp_solution = 2;
                        final_path_temp = path;
                        info << "searching for custom battery path - success\n";
                }
        }
        if (final_temp_solution == 0)
            cerr << "No thermal sensors found!\n";

        /*
        if (final_temp_solution == 2)
                cout << sys_temp (get_file (&final_path_temp[0])) << "\n";
        */

        switch (final_temp_solution)
        {
        case 0:
            src_temp = &hardware::emutemp;
            info << "thermal sensor: emutemp\n";
            break;
        case 1:
            src_temp = &hardware::proc_temp;
            info << "thermal sensor: proc\n";
            break;
        case 2:
            src_temp = &hardware::sys_temp;
            info << "thermal sensor: sys\n";
            break;
        }

}
