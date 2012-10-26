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
#include <glibtop/proctime.h>
#include <glibtop/procmem.h>
#include <glibtop/uptime.h>
#include <unistd.h>
#include <sys/types.h>
#include "core.hxx"
#include "hardware.hxx"
#include "debug.hxx"
#include <QDir>

hardware        HRDWR;
sdate           get_time    ();
extern int      core_step;

string hardware::get_file (char* path)
{
    fstream file (path, fstream::in);
        string out;
        while (file.good())
            out+=file.get();
    return out;
}

bool hardware::set_file(string path, string value)
{
    ofstream output;
    output.open(path, ofstream::trunc);
    if (output.good())
    {
        output << value;
        output.close();
        return true;
    }
    else
        return false;
}

bool hardware::set_file(string path, long double value)
{
    ofstream output;
    output.open(path, ofstream::trunc);
    if (output.good())
    {
        output << value;
        output.close();
        return true;
    }
    else
        return false;
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

unsigned int hardware::sys_backlight_full (string path)
{
    string input = get_file(&path[0]);
    string back = "";
    for (int i = 0; i<input.size()-2; i++)
        back+=input[i];
    return atoi (&back[0]);
}

unsigned short hardware::sensors_temp (string path)
{
    unsigned short highest = 0;
    for (int i = 0; i<cores_paths.size();i++)
    {
        string input = get_file(&(cores_paths[i])[0]);
        string temp = "";
        for (int i = 0; i<input.size()-2; i++)
                temp+=input[i];
        unsigned short int_temp = atoi (&temp[0])/1000;
        if (int_temp > highest)
            highest = int_temp;
    }
    return highest;
}

double hardware::C_LOAD ()
{
  glibtop_init();
  glibtop_cpu gcpu;
  glibtop_proc_time proctime;
  glibtop_get_cpu (&gcpu);
  glibtop_get_proc_time(&proctime,pid);

  static double cpu_load = 0;
  static unsigned long p_idle = 0;
  static unsigned long p_total = 0;
  static unsigned long p_pidr = 0;
  static unsigned long a_idle = 0;
  static unsigned long a_total = 0;
  static unsigned long a_pidr = 0;
  static unsigned long d_total = 0;
  static unsigned long d_idle = 0;
  static unsigned long d_pidr = 0;
  a_idle = gcpu.idle;
  a_total = gcpu.total;
  a_pidr = proctime.rtime;

  d_total = a_total - p_total;
  d_idle = a_idle - p_idle;
  d_pidr = a_pidr - p_pidr;

  if (d_total == 0)
  {
      error << "dividing by 0 during cpu load calculation! Skipping...\n";
      return 25;
  }
  cpu_load = (100*(d_total - d_idle)) / d_total;
  owncpu = (100* (d_pidr)) / d_total;
  p_idle = a_idle;
  p_total = a_total;
  p_pidr = a_pidr;

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
  if (memory.total == 0)
  {
      error << "dividing by 0 during memory calculation! Skipping...\n";
      return 25;
  }
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
    if (battery_capacity == 0)
    {
        error << "critical - full capacity didn't set correctly! - trying to reload hardware info...";
        system_check();
    }
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



int percental::convert(unsigned short val)
{
    if (val < stable-safezone)
    {
        return -(int)(pow((double)(stable-val-safezone), degree)*mod_correction_pos);
    }
    if (val > stable+safezone)
    {
        return (int)(pow((double)(val-stable-safezone), degree)*mod_correction_neg);
    }
    return 0;
}



unsigned int percental::calculate ()
{
    if (EQsize == 1)
        return load;
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



bool percental::ready()
{
    if (!buffered)
        return true;
    if (core_step > buff_size*buff_size)
        return true;
    else
        return false;
}

bool percental::ac_init( string name )
{
    if (ac.enabled)
    {
        ac.name = name;
        ac.forcesave = false;
        ac.current = stable;
        if (EQsize > 1)
        {
            for (uint i = 0; i<=EQsize; i++)
            {
                ac.freq.push_back(0);
                ac.curve.push_back(0);
                ac.virtualEQ.push_back((long double)EQ[i]);
            }
            return 1;
        }
        else
        {
            ac.enabled = false;
            warning << "cpu EQ smaller than 3 brands - disabling cpu autocalc\n";
            return 0;
        }
    }
}

bool percental::autocalc()
{
    ac.curstep = 0;
    ac.perc = 0;
    ac.common = 0.0;
    ac.exoticlow = 100.0;
    ac.exotichigh = 100.0;
    ac.curmid = 0;
    ac.stablepointlow = 0;
    ac.stablepointhigh = 0;

    for (int i = 0; i<=EQsize; i++)
    {
        if (load >= (100/EQsize)*i)
            ac.curstep = i;
        else
            break;
    }
    if (ac.curstep == EQsize)
        ac.freq[EQsize]++;
    else
    {
        ac.perc = 100*(load-(100/EQsize)*ac.curstep)/(100/EQsize);
        ac.freq[ac.curstep]+=((long double)ac.perc)/100.0;
        ac.freq[ac.curstep]+=(long double)(100-ac.perc)/100.0;
    }
    for (int i = 0; i<=EQsize; i++)
    {
        if (ac.freq[i] > ac.common)
        {
            ac.common =ac.freq[i];
            ac.curmid = i;
        }
    }
    if (ac.curmid == 0)
        ac.curmid = 1;
    else if (ac.curmid == EQsize)
        ac.curmid = EQsize-1;

    for (int i = 0; i<ac.curmid; i++)
    {
        if (ac.freq[i] < ac.exoticlow )
            ac.exoticlow = ac.freq[i];
    }
    for (int i = ac.curmid+1; i<=EQsize; i++)
    {
        if (ac.freq[i] < ac.exotichigh )
            ac.exotichigh = ac.freq[i];
    }
    bool correct = false;
    while(!correct)
    {
        correct = true;
        for (int i = 0; i<ac.curmid; i++)
        {
            if (ac.freq[i] > ac.freq[i+1])
            {
                swap(ac.freq[i], ac.freq[i+1]);
                correct = false;
            }
        }
    }
    correct = false;
    while(!correct)
    {
        correct = true;
        for (int i = ac.curmid; i<EQsize; i++)
        {
            if (ac.freq[i] < ac.freq[i+1])
            {
                swap(ac.freq[i], ac.freq[i+1]);
                correct = false;
            }
        }
    }
    for (int i = 0; i<ac.curmid; i++)
    {
        if (ac.freq[i] < ac.exoticlow+(long double)(ac.swalll*(ac.common-ac.exoticlow))/100 )
            ac.stablepointlow = i;
    }
    for (int i = ac.curmid+1; i<=EQsize; i++)
    {
        if (ac.freq[i] > ac.exotichigh+(long double)(ac.swallh*(ac.common-ac.exotichigh))/100 )
            ac.stablepointhigh = i;
    }
    if (ac.stablepointhigh == 0)
        ac.stablepointhigh = ac.curmid+1;
    if (ac.auto_degree)
    {
        ac.freq_angle_low = 1;
        ac.freq_angle_high = 1;
        for (int i = 0; i<ac.curmid; i++)
        {
            if (ac.freq[i+1]-ac.freq[i] > ac.freq_angle_low)
                ac.freq_angle_low = ac.freq[i+1]-ac.freq[i];
        }
        ac.freq_angle_low*=(100.0/(ac.common));
        ac.mult_low = (ac.mult_low_converter/ac.freq_angle_low)*100;
        if (ac.mult_low > 6)
            ac.mult_low = 6;
        for (int i = ac.curmid; i<EQsize; i++)
        {
            if (ac.freq[i]-ac.freq[i+1] > ac.freq_angle_high)
                ac.freq_angle_high = ac.freq[i]-ac.freq[i+1];
        }
        ac.freq_angle_high*=(100.0/(ac.common));
        ac.mult_high = (ac.mult_high_converter/ac.freq_angle_high)*100;
        if (ac.mult_high > 6)
            ac.mult_high = 6;
    }

    for (int i = ac.stablepointlow; i >= 0 ; i--)
    {
        ac.curve[i] = pow((long double)(ac.stablepointlow-i), ac.mult_low);
    }
    if (ac.curve[0] != 0)
        ac.curve_correct = (long double)stable/ac.curve[0];
    else
        ac.curve_correct = 1;
    for (int i = 0; i <= ac.stablepointlow ; i++)
    {
        ac.curve[i] *= ac.curve_correct;
        if (ac.curve[i] > (long double)stable)
            ac.curve[i] = (long double)stable;
        ac.curve[i] = (long double)stable - ac.curve[i];
    }
    for (int i = ac.stablepointlow; i <= ac.stablepointhigh ; i++)
    {
        ac.curve[i] = (long double)stable;
    }
    for (int i = 0; i <= EQsize-ac.stablepointhigh; i++)
    {
        ac.curve[i+ac.stablepointhigh] = pow((long double)i, ac.mult_high);
    }
    if (ac.curve[EQsize] != 0)
        ac.curve_correct = (long double)(100.0-stable)/ac.curve[EQsize];
    else
        ac.curve_correct = 1;
    for (int i = 0; i <= EQsize-ac.stablepointhigh; i++)
    {
        ac.curve[i+ac.stablepointhigh] *= ac.curve_correct;
        if (ac.curve[i+ac.stablepointhigh] > 100.0-(long double)stable)
            ac.curve[i+ac.stablepointhigh] = 100.0-(long double)stable;
        ac.curve[i+ac.stablepointhigh] += (long double)stable;
    }
    for (int i = 0; i <= EQsize; i++)
    {
        ac.virtualEQ[i] = (ac.virtualEQ[i]*(100.0-ac.impact)+ac.curve[i]*ac.impact)/100.0;
    }

}

bool percental::ac_save(Configuration *cfg)
{
    for (unsigned short i = 0; i<=EQsize; i++)
    {
        stringstream ss;
        ss << i;
        cfg->setValue ( &(".core." + ac.name + ".EQ"+ss.str())[0], (int)ac.virtualEQ[i] );
        EQ[i] = (int)ac.virtualEQ[i];
        ac.freq[i]/=2.0;
    }
    ac.forcesave = false;
}

int unital::convert(unsigned short val)
{
    if (val < stable-safezone)
    {
        return -(int)(pow((double)(stable-val-safezone), degree)*mod_correction_pos);
    }
    if (val > stable+safezone)
    {
        return (int)(pow((double)(val-stable-safezone), degree)*mod_correction_neg);
    }
    return 0;
}



unsigned int unital::calculate()
{
    if (EQsize == 1)
        return value;
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



bool unital::ready()
{
    if (!buffered)
        return true;
    if (core_step > buff_size*buff_size)
        return true;
    else
        return false;
}


bool unital::ac_init( string name )
{
    if (ac.enabled)
    {
        ac.name = name;
        ac.forcesave = false;
        ac.current = stable;
        if (EQsize > 1)
        {
            for (uint i = 0; i<=EQsize; i++)
            {
                ac.freq.push_back(0);
                ac.curve.push_back(0);
                ac.virtualEQ.push_back((long double)EQ[i]);
            }
            return 1;
        }
        else
        {
            ac.enabled = false;
            warning << "cpu EQ smaller than 3 brands - disabling cpu autocalc\n";
            return 0;
        }
    }
}


bool unital::autocalc( Configuration * cfg )
{
    ac.curstep = 0;
    ac.perc = 0;
    ac.common = 0.0;
    ac.exoticlow = 100.0;
    ac.exotichigh = 100.0;
    ac.curmid = 0;
    ac.stablepointlow = 0;
    ac.stablepointhigh = 0;

    if (value < EQbegin)
    {
        ac.freq[0]++;
        cfg->setValue(".core.EQbegin", (int)value-1);
        ac.forcesave = true;
        info << "autocalc caught new temperature: " << (int)value << " EQbegin switched to new value";
        warning << "it's recomended to restart Eyes as quick as it's possible!";
    }
    for (int i = 0; i<=EQsize; i++)
    {
        if (value >= EQbegin + ((EQend-EQbegin)/EQsize)*i)
            ac.curstep = i;
        else
            break;
    }
    if (ac.curstep == EQsize)
    {
        ac.freq[EQsize]++;
        cfg->setValue(".core.EQend", (int)value+1);
        ac.forcesave = true;
        info << "autocalc caught new temperature: " << (int)value << " EQend switched to new value";
        warning << "it's recomended to restart Eyes as quick as it's possible!";
    }
    else
    {
        ac.perc = 100*(value-(EQbegin + ((EQend-EQbegin)/EQsize)*ac.curstep))/((EQend-EQbegin)/EQsize);
        ac.freq[ac.curstep]+=((long double)ac.perc)/100.0;
        ac.freq[ac.curstep]+=(long double)(100-ac.perc)/100.0;
    }
    for (int i = 0; i<=EQsize; i++)
    {
        if (ac.freq[i] > ac.common)
        {
            ac.common =ac.freq[i];
            ac.curmid = i;
        }
    }
    if (ac.curmid == 0)
        ac.curmid = 1;
    else if (ac.curmid == EQsize)
        ac.curmid = EQsize-1;

    for (int i = 0; i<ac.curmid; i++)
    {
        if (ac.freq[i] < ac.exoticlow )
            ac.exoticlow = ac.freq[i];
    }
    for (int i = ac.curmid+1; i<=EQsize; i++)
    {
        if (ac.freq[i] < ac.exotichigh )
            ac.exotichigh = ac.freq[i];
    }
    bool correct = false;
    while(!correct)
    {
        correct = true;
        for (int i = 0; i<ac.curmid; i++)
        {
            if (ac.freq[i] > ac.freq[i+1])
            {
                swap(ac.freq[i], ac.freq[i+1]);
                correct = false;
            }
        }
    }
    correct = false;
    while(!correct)
    {
        correct = true;
        for (int i = ac.curmid; i<EQsize; i++)
        {
            if (ac.freq[i] < ac.freq[i+1])
            {
                swap(ac.freq[i], ac.freq[i+1]);
                correct = false;
            }
        }
    }
    for (int i = 0; i<ac.curmid; i++)
    {
        if (ac.freq[i] < ac.exoticlow+(long double)(ac.swalll*(ac.common-ac.exoticlow))/100 )
            ac.stablepointlow = i;
    }
    for (int i = ac.curmid+1; i<=EQsize; i++)
    {
        if (ac.freq[i] > ac.exotichigh+(long double)(ac.swallh*(ac.common-ac.exotichigh))/100 )
            ac.stablepointhigh = i;
    }
    if (ac.stablepointhigh == 0)
        ac.stablepointhigh = ac.curmid+1;
    if (ac.auto_degree)
    {
        ac.freq_angle_low = 1;
        ac.freq_angle_high = 1;
        for (int i = 0; i<ac.curmid; i++)
        {
            if (ac.freq[i+1]-ac.freq[i] > ac.freq_angle_low)
                ac.freq_angle_low = ac.freq[i+1]-ac.freq[i];
        }
        ac.freq_angle_low*=(100.0/(ac.common));
        ac.mult_low = (ac.mult_low_converter/ac.freq_angle_low)*100;
        if (ac.mult_low > 6)
            ac.mult_low = 6;
        for (int i = ac.curmid; i<EQsize; i++)
        {
            if (ac.freq[i]-ac.freq[i+1] > ac.freq_angle_high)
                ac.freq_angle_high = ac.freq[i]-ac.freq[i+1];
        }
        ac.freq_angle_high*=(100.0/(ac.common));
        ac.mult_high = (ac.mult_high_converter/ac.freq_angle_high)*100;
        if (ac.mult_high > 6)
            ac.mult_high = 6;
    }

    for (int i = ac.stablepointlow; i >= 0 ; i--)
    {
        ac.curve[i] = pow((long double)(ac.stablepointlow-i), ac.mult_low);
    }
    if (ac.curve[0] != 0)
        ac.curve_correct = (long double)stable/ac.curve[0];
    else
        ac.curve_correct = 1;
    for (int i = 0; i <= ac.stablepointlow ; i++)
    {
        ac.curve[i] *= ac.curve_correct;
        if (ac.curve[i] > (long double)stable)
            ac.curve[i] = (long double)stable;
        ac.curve[i] = (long double)stable - ac.curve[i];
    }
    for (int i = ac.stablepointlow; i <= ac.stablepointhigh ; i++)
    {
        ac.curve[i] = (long double)stable;
    }
    for (int i = 0; i <= EQsize-ac.stablepointhigh; i++)
    {
        ac.curve[i+ac.stablepointhigh] = pow((long double)i, ac.mult_high);
    }
    if (ac.curve[EQsize] != 0)
        ac.curve_correct = (long double)(100.0-stable)/ac.curve[EQsize];
    else
        ac.curve_correct = 1;
    for (int i = 0; i <= EQsize-ac.stablepointhigh; i++)
    {
        ac.curve[i+ac.stablepointhigh] *= ac.curve_correct;
        if (ac.curve[i+ac.stablepointhigh] > 100.0-(long double)stable)
            ac.curve[i+ac.stablepointhigh] = 100.0-(long double)stable;
        ac.curve[i+ac.stablepointhigh] += (long double)stable;
    }
    for (int i = 0; i <= EQsize; i++)
    {
        ac.virtualEQ[i] = (ac.virtualEQ[i]*(100.0-ac.impact)+ac.curve[i]*ac.impact)/100.0;
    }

}


bool unital::ac_save(Configuration *cfg)
{
    for (unsigned short i = 0; i<=EQsize; i++)
    {
        stringstream ss;
        ss << i;
        cfg->setValue ( &(".core." + ac.name + ".EQ"+ss.str())[0], (int)ac.virtualEQ[i] );
        EQ[i] = (int)ac.virtualEQ[i];
        ac.freq[i]/=2.0;
    }
    ac.forcesave = false;
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

bool hardware::set_backlight (unsigned short percentage)
{
    unsigned int full = sys_backlight_full(backlight_fpath);
    if (set_file(backlight_npath, (int) full*percentage/100))
        return 1;
    else
        return 0;
}

void hardware::system_check()
{
    info << "System checking...\n";
        final_now_solution = 0;
        final_full_solution = 0;
        final_state_solution = 0;
        final_temp_solution = 0;
        screen_support = 0;

        QDir * cDir;
        cDir = new QDir("/proc/acpi/battery");
        QStringList dirs = cDir->entryList();
        delete (cDir);
        for (int i = 0; i < dirs.size(); i++)
        {
            if (!dirs[i].contains("BAT"))
            {
                dirs.erase(dirs.begin() + i);
                i--;
            }
        }
        //for (int i = 0; i < dirs.size(); i++)
        //{
        //    cerr << dirs[i].toStdString() << "\n";
        //}

        for (int i = 0; i < dirs.size(); i++)
        {
                string input = "";
                string path = "/proc/acpi/battery/";
                path += dirs[i].toStdString();
                path += "/state";
                input = get_file (&path[0]);
                if (input == "");
                        //info << "proc(BAT" << i << ") - failed\n";
                else
                {
                        final_now_solution = 1;
                        final_state_solution = 1;
                        final_path_now = path;
                        final_path_full = "/proc/acpi/battery/";
                        final_path_full += dirs[i].toStdString();
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
                if (input == "");
                        //info << "searching for custom battery path - failed\n";
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
        if (final_now_solution == 1)
        {
            final_path_state = final_path_full;
            string input = "";
            input = get_file (&final_path_full[0]);
            if (input == "" || proc_bat_full(&final_path_state[0]) == 0)
                    warning << "battery capacity searching - failed\n";
            else
            {
                    final_full_solution = 1;
                    info << "battery capacity searching - success\n";
            }
        }

        /*
        if (final_now_solution == 1)
                cout << proc_bat_now (get_file (&final_path_now[0])) << "\n";
        if (final_full_solution == 1)
                cout << proc_bat_full (get_file (&final_path_full[0])) << "\n";
        if (final_state_solution == 1)
                cout << proc_bat_state (get_file (&final_path_state[0])) << "\n";
        */


        cDir = new QDir("/sys/class/power_supply");
        dirs = cDir->entryList();
        delete (cDir);
        for (int i = 0; i < dirs.size(); i++)
        {
            if (!dirs[i].contains("BAT"))
            {
                dirs.erase(dirs.begin() + i);
                i--;
            }
        }
        //for (int i = 0; i < dirs.size(); i++)
        //{
        //    cerr << dirs[i].toStdString() << "\n";
        //}

        for (int i = 0; i < dirs.size(); i++)
        {
                string input = "";
                string path = "/sys/class/power_supply/";
                path += dirs[i].toStdString();
                path += "/charge_now";
                input = get_file (&path[0]);
                if (input == "");
                        //info << "sys(BAT" << i << ") - failed\n";
                else
                {
                        final_now_solution = 2;
                        final_path_now = path;
                        final_path_full = "/sys/class/power_supply/";
                        final_path_full += dirs[i].toStdString();
                        final_path_full += "/charge_full";
                        final_path_state = "/sys/class/power_supply/";
                        final_path_state += dirs[i].toStdString();
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
                if (input == "");
                        //info << "searching for custom battery path - failed\n";
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
        if (final_now_solution == 2)
        {
            string input = "";
            input = get_file (&final_path_full[0]);
            if (input == "")
                    warning << "battery capacity searching - failed\n";
            else
            {
                    final_full_solution = 2;
                    info << "battery capacity searching - success\n";
            }
            input = "";
            input = get_file (&final_path_state[0]);
            if (input == "");
                    //info << "battery status searching - failed\n";
            else
            {
                    final_state_solution = 2;
                    info << "battery status searching - success\n";
            }
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
            warning << "No battery found!\n";

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

        cDir = new QDir("/sys/class/power_supply");
        dirs = cDir->entryList();
        delete (cDir);
        for (int i = 0; i < dirs.size(); i++)
        {
            if (!dirs[i].contains("TZ"))
            {
                dirs.erase(dirs.begin() + i);
                i--;
            }
        }
        //for (int i = 0; i < dirs.size(); i++)
        //{
        //    cerr << dirs[i].toStdString() << "\n";
        //}

        for (int i = 0; i < dirs.size(); i++)
        {
                string input = "";
                string path = "/proc/acpi/thermal_zone/";
                path += dirs[i].toStdString();
                path += "/temperature";
                input = get_file (&path[0]);
                if (input == "");
                        //info << "proc(TZ0" << i << ") - failed\n";
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
                if (input == "");
                        //info << "searching for custom battery path - failed\n";
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

        cDir = new QDir("/sys/class/thermal");
        dirs = cDir->entryList();
        delete (cDir);
        for (int i = 0; i < dirs.size(); i++)
        {
            if (!dirs[i].contains("thermal_zone"))
            {
                dirs.erase(dirs.begin() + i);
                i--;
            }
        }
        //for (int i = 0; i < dirs.size(); i++)
        //{
        //    cerr << dirs[i].toStdString() << "\n";
        //}

        for (int i = 0; i < dirs.size(); i++)
        {
                string input = "";
                string path = "/sys/class/thermal/";
                path += dirs[i].toStdString();
                path += "/temp";
                input = get_file (&path[0]);
                if (input == "");
                        //info << "sys(thermal_zone" << i << ") - failed\n";
                else
                {
                        final_temp_solution = 2;
                        final_path_temp = path;
                        info << "thermal sensor found on: " << path << "\n";
                        break;
                }
        }
        if (special_thername)
        {
                string input = "";
                string path = "/sys/class/thermal/";
                path += cfg_thername;
                path += "/temp";
                input = get_file (&path[0]);
                if (input == "");
                        //info << "searching for custom thermal sensor path - failed\n";
                else
                {
                        final_temp_solution = 2;
                        final_path_temp = path;
                        info << "searching for custom thermal sensor path - success\n";
                }
        }


        cDir = new QDir("/sys/devices/platform");
        dirs = cDir->entryList();
        delete (cDir);
        for (int i = 0; i < dirs.size(); i++)
        {
            if (!dirs[i].contains("coretemp"))
            {
                dirs.erase(dirs.begin() + i);
                i--;
            }
        }
        //for (int i = 0; i < dirs.size(); i++)
        //{
        //    cerr << dirs[i].toStdString() << "\n";
        //}


        for (int i = 0; i < dirs.size(); i++)
        {
                string input = "";
                string path = "/sys/devices/platform/";
                path += dirs[i].toStdString();

                cDir = new QDir(&path[0]);
                QStringList sdirs = cDir->entryList();
                delete (cDir);
                for (int j = 0; j < sdirs.size(); j++)
                {
                    if (!sdirs[j].contains("input"))
                    {
                        sdirs.erase(sdirs.begin() + j);
                        j--;
                    }
                }
                //for (int j = 0; j < sdirs.size(); j++)
                //{
                //    cerr << sdirs[j].toStdString() << "\n";
                //}

                if (dirs.size() > 1)
                    info << "multi cpu detected\n";
                if (sdirs.size() > 1)
                    info << "multi cores detected\n";

                for (int j = 0; j < sdirs.size(); j++)
                {
                    cores_paths.clear();
                    string tmp = path;
                    tmp += "/";
                    tmp += sdirs[j].toStdString();
                    input = get_file (&path[0]);
                    if (input == "");
                    else
                    {
                            final_temp_solution = 3;
                            final_path_temp = "/sys/devices/platform/";
                            final_path_temp += sdirs[j].toStdString();
                            info << "thermal sensor for cpu: " << i+1 << " found on: " << final_path_temp << "\n";
                            cores_paths.push_back(tmp);
                    }

                }
        }
        if (special_thername)
        {
                string input = "";
                string path = "/sys/devices/platform/";
                QStringList cuspaths = QString (&cfg_thername[0]).split("|");
                if (cuspaths.size() < 2 && final_temp_solution == 0)
                {
                    error << "wrong custom path - please give \"cpu\|core\" notation: eg. \"coretemp\.0\|temp2_input\" for lm-sensors - will exit now...\n";
                    exit(1);
                }
                else if (cuspaths.size() < 2);
                else
                {
                    path += cuspaths[0].toStdString();
                    cores_paths.clear();
                    cDir = new QDir(&path[0]);
                    dirs = cDir->entryList();
                    delete (cDir);
                    for (int i = 0; i < dirs.size(); i++)
                    {
                        if (!dirs[i].contains(&cuspaths[1].toStdString()[0]))
                        {
                            dirs.erase(dirs.begin() + i);
                            i--;
                        }
                    }
                    //for (int i = 0; i < dirs.size(); i++)
                    //{
                    //    cerr << dirs[i].toStdString() << "\n";
                    //}
                    if (dirs.size() > 0)
                    {
                        final_temp_solution = 3;
                        final_path_temp = "/sys/devices/platform/";
                        final_path_temp += cuspaths[0].toStdString();
                        info << "thermal sensor for custom cpu found on: " << final_path_temp << "\n";
                    }
                    for (int j = 0; j < dirs.size(); j++)
                    {
                        string corepath = final_path_temp;
                        corepath += dirs[j].toStdString();
                        input = get_file (&corepath[0]);
                        if (input == "");
                                //info << "sys(thermal_zone" << i << ") - failed\n";
                        else
                        {
                            info << "core " << j-1 << "thermal sensor found on: " << corepath << "\n";
                            cores_paths.push_back(corepath);
                        }
                    }
                }
        }


        if (final_temp_solution == 0)
            warning << "No thermal sensors found!\n";

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
        case 3:
            src_temp = &hardware::sensors_temp;
            info << "thermal sensor: lm-sensors\n";
            break;
        }

            string input = "";
            string path = "/sys/class/backlight/";
            path += backlight_path;
            path += "/brightness";
            input = get_file (&path[0]);
            if (input == "")
            {
                cDir = new QDir("/sys/class/backlight/");
                QStringList dirs = cDir->entryList();
                delete (cDir);

                int biggest_index = -1;
                int biggest_range = 0;
                for (int i = 0; i < dirs.size(); i++)
                {
                    input = get_file (&("/sys/class/backlight/" + dirs[i].toStdString() + "/max_brightness")[0]);
                    if (input != "")
                    {
                        if (atoi(&input[0]) > biggest_range)
                        {
                            biggest_index = i;
                            biggest_range = atoi(&input[0]);
                        }
                    }
                    //cerr << dirs[i].toStdString() << "\n";
                }
                if (biggest_index == -1)
                    info << "backlight searching failed\n";
                else
                {
                    backlight_path = dirs[biggest_index].toStdString();
                    path = "/sys/class/backlight/";
                    path += backlight_path;
                    path += "/brightness";
                    input = get_file (&path[0]);
                    info << "backlight found on: " << path << "\n";
                }
            }
            if (input == "");
            else
            {
                backlight_npath = path;
                if (!set_file(path, input))
                    info << "backlight changing - permission denied\n";
                input = "";
                path = "/sys/class/backlight/";
                path += backlight_path;
                path += "/max_brightness";
                input = get_file (&path[0]);
                if (input == "")
                    info << "max backlight value reading - failed\n";
                else
                {
                    screen_support = 1;
                    backlight_fpath = path;
                    info << "backlight support possible:\n+ full: " << backlight_fpath << "\n+ now: " << backlight_npath << "\n";
                }
            }

}


double hardware::equalize(double min, double max, double input, double degree)
{
    return pow(input-min, degree)*((max-min)/pow((max-min), degree))+min;
}
