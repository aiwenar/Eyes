
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
#include <fstream>
#include <cmath>
#include <glibtop.h>
#include <glibtop/cpu.h>
#include <glibtop/mem.h>
#include <glibtop/proclist.h>
#include <glibtop/uptime.h>
#include "core_new.hxx"
#include "eyes_view.h"

using namespace std;

unsigned short  battery_state;
unsigned short  mod_bat_plug;
unsigned int    temp_t;
unsigned int    flu_timer;
unsigned int    prev_bat_plug;
unsigned int    battery_capacity;
bool            once_plugged;
bool            get_flu;
int             core_step;
percental       cpu;
percental       memory;
percental       battery;
unital          temperature;
timal           times;
timal           energy;
bul             bulwers;
sended_anims    s_anim;
hard_dbg        HDBG;
QString         face_send;

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
    cerr << "[info :] anims_reload begin\n";

    if (s_anim.close == "" || s_anim.open == "")
        cerr << "\033[32mOops! My bad :]\n\n";

    set_animation(s_anim.open, s_anim.close, s_anim.start, s_anim.end);

    cerr << "[info :] sended pics:\n" << s_anim.open.toStdString() << " - open anim\n"
         << s_anim.close.toStdString() << " - close anim\n"
         << "start/end " << s_anim.start << " " << s_anim.end
         << "\nface_next = " << face_next.toStdString() << "\n";
}

void eyes_view::reload_eyes()
{
    set_eyes ( s_anim.eyes );
}

void eyes_view::send_eyes( QString neyes )
{
    if ( neyes == "" )
    {
        cerr << "033[32mOops! My bad :]";
        neyes = "eye_06";
    }
    s_anim.eyes = neyes;
    cerr << "core sets \"" << s_anim.eyes.toStdString() << "\" eye\n";
}

double C_LOAD ()
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
//-------------------------
double M_LOAD ()
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
//-------------------------
unsigned short P_LIST ()
{
int which,arg;

glibtop_init();
glibtop_proclist proclist;
glibtop_get_proclist(&proclist,which,arg);

return proclist.number;
}
//-------------------------
int U_TIME ()
{
glibtop_init();
glibtop_uptime uptime;
glibtop_get_uptime(&uptime);

return uptime.uptime;
}

//-------------------------

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

//--------------------------

unsigned short temperatura ()
{
        fstream calosc ("/proc/acpi/thermal_zone/TZ00/temperature", fstream::in);

        string texcik;
        while (calosc.good())
                texcik+=calosc.get();

        unsigned short tempjuczer=texcik.find_first_of ("temperature");

        unsigned short i=0;
        unsigned short a=0;
        char temp[27];

        for (;i<27;i++) {
                temp[a] = texcik[25+a];
                a++;
                }

        unsigned short temperature;
        temperature = atoi (temp);

        return temperature;
}

//--------------------------

int bat_plugged ()
{
        fstream calosc ("/proc/acpi/battery/BAT1/state", fstream::in);

        string texcik;
        while (calosc.good()){
        texcik+=calosc.get();}

        if (texcik[25] == 'n')
        {
                return 0;
        }

        static int pluged = 0;

        if (texcik[82] == 'c' && pluged !=1) //battery has been just pluged
        {
                pluged = 1;
                return 1;
        }
        if (texcik[82] != 'c' && pluged !=0) //battery has been just unpluged
        {
                pluged = 0;
                return 2;
        }
        if (texcik[82] == 'c' && pluged == 1) //battery is still pluged
                return 3;
        if (texcik[82] != 'c' && pluged == 0) //battery is still unpluged
                return 4;

        else return 5;
    }

int bateria ()
{
        fstream calosc ("/proc/acpi/battery/BAT1/state", fstream::in);

        string texcik;
        while (calosc.good()){
        texcik+=calosc.get();}

        int baterry=texcik.find_first_of ("capacity:");


        int i=0;
        int a=0;
        char bat[155];

        for (;i<155;i++)
        {
                bat[a] = texcik[151+a];
                a++;
        }

        static int powr;
        powr = atoi (bat);

        return 100*powr/battery_capacity;
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
            mod_bat_plug    ;
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
        if (times.value < 7 || times.value >= 19)
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
        if (times.value < 6 || times.value >= 19)
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

unsigned int percental::calculate ()
{
    mod_prev = 0;
    mod = 0;
    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (load <= stable - loseless - (i*((stable - loseless)/steps)))
        {
            if (frequency == 'l')
                mod = -i*lin_num;
            if (frequency == 'q')
                mod = -i*i;
            if (frequency == 'f')
                mod = -i + mod_prev;
        }
    }

    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (load >= stable + loseless + (i*((100-(stable + loseless))/steps)))
        {
            if (frequency == 'l')
                mod = i*lin_num;
            if (frequency == 'q')
                mod = i*i;
            if (frequency == 'f')
                mod = i + mod_prev;
        }

    }
    return mod;
}

unsigned int unital::calculate()
{
    mod = 0;
    mod_prev = 0;
    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (value <= stable - loseless - (i*unit))
        {
            if (frequency == 'l')
                mod = -i*lin_num;
            if (frequency == 'q')
                mod = -i*i;
            if (frequency == 'f')
                mod = -i + mod_prev;
        }
    }

    for (unsigned short i = 0; i<=steps; i++)
    {
        mod_prev = mod;
        if (value >= stable + loseless + (i*unit))
        {
            if (frequency == 'l')
                mod = i*lin_num;
            if (frequency == 'q')
                mod = i*i;
            if (frequency == 'f')
                mod = i + mod_prev;
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
    if (frequency == 'l')
    {
        for (unsigned short i = steps; i>=0; i--)
        {
            if (value >= (end - (i*(wide/steps))))
                mod = i;


        }
    }
    if (frequency == 'q')
    {
        unsigned short wall = end;
        unsigned short sector = wide;

        for (unsigned short i = steps; i>= 0; i--)
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
return mod;
}

void Core::bulwers_init ()
{

if (cpu.buffered)
{
    cpu.current_probe        = 0  ;
    cpu.current_probe_small  = 0  ;

    for (unsigned short i = 0; i<=cpu.buff_size;i++)
    {
        cpu.probes.push_back (cpu.stable);
    }
    for (unsigned short i = 0; i<=cpu.buff_size;i++)
    {
        cpu.sector_small.push_back (cpu.stable);
    }
}

if (memory.buffered)
{
    memory.current_probe        = 0  ;
    memory.current_probe_small  = 0  ;

    for (unsigned short i = 0; i<=memory.buff_size;i++)
    {
        memory.probes.push_back (memory.stable);
    }
    for (unsigned short i = 0; i<=memory.buff_size;i++)
    {
        memory.sector_small.push_back (memory.stable);
    }
}

if (battery.buffered)
{
    battery.current_probe       = 0  ;
    battery.current_probe_small = 0  ;

    for (unsigned short i = 0; i<=battery.buff_size;i++)
    {
        battery.probes.push_back (battery.stable);
    }
    for (unsigned short i = 0; i<=battery.buff_size;i++)
    {
        battery.sector_small.push_back (battery.stable);
    }
}

if (temperature.buffered)
{
    temperature.current_probe       = 0  ;
    temperature.current_probe_small = 0  ;

    for (unsigned short i = 0; i<=temperature.buff_size;i++)
    {
        temperature.probes.push_back (temperature.stable);
    }
    for (unsigned short i = 0; i<=temperature.buff_size;i++)
    {
        temperature.sector_small.push_back (temperature.stable);
    }
}

energy.value                      = 0    ;
energy.start                     *= 3600 ;
energy.wide                      *= 3600 ;
once_plugged                      = false;
mod_bat_plug                      = 0    ;
bulwers.step                      = 0    ;


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
            sector_small[current_probe_small] = 0;
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
            sector_small[current_probe_small] = 0;
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


            cerr << "[info :] core pics settings begin\n";

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
cpu.get_load(C_LOAD());
memory.get_load(M_LOAD ());
if (battery_state != 1 && battery_state != 3 && battery_state != 0)
    battery.get_load(bateria());
else if (battery_state == 0)
    battery.load = 0;
else
    battery.load = 100;
temperature.get_load(temperatura());
times.value = get_time ().hour/3600;
energy.value ++;

cpu.mod = cpu.calculate();
memory.mod = memory.calculate();
battery.mod = battery.calculate();
temperature.mod = temperature.calculate();
times.mod = times.calculate();
energy.mod = energy.calculate();
battery_state = bat_plugged ();
bulwers.update();
}

void Core::gui_init()
{
cout << "\033[40m" << "\033[37m" << "\n";
HDBG.max_s = 10;
if (cpu.buffered && cpu.buff_size > HDBG.max_s)
    HDBG.max_s = cpu.buff_size;
if (memory.buffered && memory.buff_size > HDBG.max_s)
    HDBG.max_s = memory.buff_size;
if (temperature.buffered && temperature.buff_size > HDBG.max_s)
    HDBG.max_s = temperature.buff_size;
for (unsigned short i = HDBG.max_s+6; i>1; i--)
{
    cout << "\n";
}
for (unsigned short i = HDBG.max_s+7; i>1; i--)
{
    cout << "\033[1A";
}
for (unsigned short i = 37; i>1; i--)
{
    cout << "-";
}
cout << "\033[12D" << "\033[3B";
for (unsigned short i = 12; i>1; i--)
{
    cout << "-";
}
cout << "\033[12D" << "\033[3B";
for (unsigned short i = 12; i>=1; i--)
{
    cout << "-";
}
cout << "\033[12D" << "\033[3B";
for (unsigned short i = 12; i>=1; i--)
{
    cout << "-";
}
cout << "\033[27D" << "\033[9A";
for (unsigned short i = HDBG.max_s + 4; i>0; i--)
{
    cout << "\033[1B" << "\033[1D" << "|";
}
for (unsigned short i = HDBG.max_s + 4; i>0; i--)
{
    cout << "\033[1A";
}

cout << "\033[8C";
for (unsigned short i = HDBG.max_s+4; i>0; i--)
{
    cout << "\033[1B" << "\033[1D" << "|";
}
for (unsigned short i = HDBG.max_s+4; i>0; i--)
{
    cout << "\033[1A";
}
cout << "\033[8C";
for (unsigned short i = HDBG.max_s+4; i>0; i--)
{
    cout << "\033[1B" << "\033[1D" << "|";
}
for (unsigned short i = HDBG.max_s+4; i>0; i--)
{
    cout << "\033[1A";
}
cout << "\033[12C" << "\033[1A";

for (unsigned short i = HDBG.max_s+6; i>0; i--)
{
    cout << "|" << "\033[1B" << "\033[1D";
}
for (unsigned short i = HDBG.max_s+6; i>0; i--)
{
    cout << "\033[1A";
}
cout << "\033[14C";
for (unsigned short i = HDBG.max_s+6; i>0; i--)
{
    cout << "|" << "\033[1B" << "\033[1D";
}
for (unsigned short i = HDBG.max_s+6; i>0; i--)
{
    cout << "\033[1A";
}
cout << "\n" << "\033[1A" << " " << "step:" << "        " << "special:";
cout << "\033[2B" << "\033[21D" << "cpu:";
cout << "\033[4C" << "mem:";
cout << "\033[4C" << "temp:";
cout << "\033[3C" << "battery:";
cout << "\033[8D" << "\033[3B" << "time:";
cout << "\033[5D" << "\033[3B" << "energy:";
cout << "\033[7D" << "\033[6B" << "next_wall:";
cout << "\n" << "\033[15A" << "\033[22C              | " << "mods:       ";
if (1)
    cout << "\033[1C HARDLY DEBUG MODE:";
cout << "\n\n" << "\033[38C" << "cpu:\n";
cout << "\033[1A" << "\033[44C" << "load:\n";
cout << "\n" << "\033[38C" << "mem:\n";
cout << "\033[1A" << "\033[44C" << "load:\n";
cout << "\n" << "\033[38C" << "temp:\n";
cout << "\033[1A" << "\033[44C" << "val:\n";
cout << "\n" << "\033[38C" << "batt:\n";
cout << "\033[1A" << "\033[44C" << "perc:\n";
cout << "\n" << "\033[38C" << "time:\n";
cout << "\033[1A" << "\033[44C" << "val:\n";
cout << "\n" << "\033[38C" << "enrg:\n";
cout << "\033[1A" << "\033[44C" << "val:\n";
cout << "\n" << "\033[38C" << "plug:\n";
cout << "\033[1A" << "\033[44C" << "val:\n";
cout << "\033[14A";



cout << "\n";

/*

This is concept debug layout:


#######################################################################################################################
# step:____   special:__             | mods:       | [] [] [] [] [] []        |
# -----------------------------------|             | [] [] [] [] [] [] bul    |
# cpu:  | mem:  | temp: | battery:   | cpu:  load: | [] [] [] [] [] [] [t]    |
#       |       |       | ]]]]]]>><< |             | [] [] [] [] [] [] 10min  |
# p1 b1 | m1 b1 | t1 b1 |------------| mem:  load: | [] [] [] [] [] []        |
# p2 b2 | m2 b2 | t2 b2 | time:__    |             | [] [] [] [] [] []        |
# p3 b3 | m3 b3 | t3 b3 | ========== | temp: val:  | [] [] [] [] [] []        |
# p4 b4 | m4 b4 | t4 b4 |------------|             | [] [] [] [] [] []        |
# p5 b5 | m5 b5 | t5 b5 | energy:__  | batt: perc: | [] [] [] [] [] []        |
# p6 b6 | m6 b6 | t6 b6 | ========== |             | [] [] [] [] [] []        |
# p7 b7 | m7 b7 | t7 b7 |------------| time: val:  | [] [] [] [] [] []        |
# p8 b8 | m8 b8 | t8 b8 | RISE||CALM |             | [] [] [] [] [] []        |
# p9 b9 | m9 b9 | t9 b9 | __________ | enrg: val:  | [] [] [] [] [] []        |
# p0 b0 | m0 b0 | t0 b0 | __________ |             | [] [] [] [] [] []        |
#       |       |       | next_wall: | plug: val:  | [] [] [] [] [] []        |
# ===== | ===== | ===== | __________ |             | [] [] [] [] [] []        |


And for everyone there are terminal instructions for that:


\033 = [Esc] (up in your keyboard top left corner)

 Colors:
 \033[30m set foreground color to black

 \033[31m set foreground color to red

 \033[32m set foreground color to green

 \033[33m set foreground color to yellow

 \033[34m set foreground color to blue

 \033[35m set foreground color to magenta (purple)

 \033[36m set foreground color to cyan

 \033[37m set foreground color to white

 \033[40m set background color to black

 \033[41m set background color to red

 \033[42m set background color to green

 \033[43m set background color to yellow

 \033[44m set background color to blue

 \033[45m set background color to magenta (purple)

 \033[46m set background color to cyan

 \033[47m set background color to white

 \033[1;30m set foreground color to dark gray

 \033[1;31m set foreground color to light red

 \033[1;32m set foreground color to light green

 \033[1;33m set foreground color to yellow

 \033[1;34m set foreground color to light blue

 \033[1;35m set foreground color to light magenta (purple)

 \033[1;36m set foreground color to light cyan

 \033[1;37m set foreground color to white

 \033[1;40m set background color to dark gray

 \033[1;41m set background color to light red

 \033[1;42m set background color to light green

 \033[1;43m set background color to yellow

 \033[1;44m set background color to light blue

 \033[1;45m set background color to light magenta (purple)

 \033[1;46m set background color to light cyan

 \033[1;47m set background color to white

 For other features:
 \033[0m reset; clears all colors and styles (to white on black)

 \033[1m bold on

 \033[3m italics on

 \033[4m underline on

 \033[5m blink on

 \033[7m reverse video on

 \033[8m nondisplayed (invisible)

 \033[x;yH moves cursor to line x, column y

 \033[xA moves cursor up x lines

 \033[xB moves cursor down x lines

 \033[xC moves cursor right x spaces

 \033[xD moves cursor left x spaces

 \033[2J clear screen and home cursor

 END

*/
}
void Core::gui_refresh ()
{
    cout << "\033[1;33m";
    cout << "\033[2A" << "\033[6C" << core_step << "\n";
    cout << "\033[1A" << "\033[22C" << "0 \n";
    cout << "\033[1B" << "\n\n";

    if (cpu.buffered)
    {
        for (unsigned short i = 0; i< cpu.buff_size; i++)
        {
            if (i != cpu.current_probe)
            {
                if (cpu.probes[i] <= cpu.stable - cpu.loseless)
                    cout << "\033[1;32m" << " " << (unsigned short)cpu.probes[i] << "\n";
                else if (cpu.probes[i] >= cpu.stable + cpu.loseless)
                    cout << "\033[1;31m" << " " << (unsigned short)cpu.probes[i] << "\n";
                else
                    cout << "\033[1;30m" << " " << (unsigned short)cpu.probes[i] << "\n";
            }
            else
                cout << "\033[1;33m" << ">" << (unsigned short)cpu.probes[i] << "\n";
        }
        for (unsigned short i = 0; i<= cpu.buff_size; i++)
        {
            cout << "\033[1A";
        }
        cout << "\n";

        for (unsigned short i = 0; i< cpu.buff_size; i++)
        {
            if (i != cpu.current_probe_small)
            {
                if (cpu.sector_small[i] <= cpu.stable - cpu.loseless)
                {
                    if (cpu.sector_small[i] < 10)
                        cout << "\033[1;32m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;32m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
                }
                else if (cpu.sector_small[i] >= cpu.stable + cpu.loseless)
                {
                    if (cpu.sector_small[i] < 10)
                        cout << "\033[1;31m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;31m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
                }
                else
                {
                    if (cpu.sector_small[i] < 10)
                        cout << "\033[1;30m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;30m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << " \n";
                }
            }
            else
            {
                if (cpu.sector_small[i] < 10)
                    cout << "\033[1;33m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << " <\n";
                else
                    cout << "\033[1;33m" << "\033[4C" << (unsigned short)cpu.sector_small[i] << "<\n";
            }
        }
    }
    else
    {
        if (cpu.load <= cpu.stable - cpu.loseless)
            cout << "\033[1;32m" << " " << (unsigned short)cpu.load << " \n";
        else if (cpu.load >= cpu.stable + cpu.loseless)
            cout << "\033[1;31m" << " " << (unsigned short)cpu.load << " \n";
        else
            cout << "\033[1;30m" << " " << (unsigned short)cpu.load << " \n";
    }

    cout << "\n" << "\033[1C";
    if (cpu.frequency == 'l')
    {
        if (cpu.mod <= 0)
        {
            unsigned short s = 5 * (double(-cpu.mod) / double(cpu.lin_num*cpu.steps));
            if (s == 0 && cpu.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(cpu.mod) / double(cpu.lin_num*cpu.steps));
            if (s == 0 && cpu.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }
    if (cpu.frequency == 'q')
    {
        if (cpu.mod <= 0)
        {
            unsigned short s = 5 * (double(-cpu.mod) / double(cpu.steps*cpu.steps));
            if (s == 0 && cpu.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(cpu.mod) / double(cpu.steps*cpu.steps));
            if (s == 0 && cpu.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (cpu.frequency == 'f')
    {
        unsigned short max_mod = 0;
        unsigned short max_mod_prev = 0;
        for (unsigned int i = 0; i <= cpu.steps; i++)
        {
            max_mod_prev = max_mod;
            max_mod = i + max_mod_prev;
        }
        if (cpu.mod <= 0)
        {
            unsigned short s = 5 * (double(-cpu.mod) / double(max_mod));
            if (s == 0 && cpu.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for (unsigned short i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(cpu.mod) / double(max_mod));
            if (s == 0 && cpu.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (cpu.buffered)
    {

        for (unsigned short i = 0; i<= cpu.buff_size+1; i++)
        {
            cout << "\033[1A";
        }
    }
    else
        cout << "\033[3A";

    cout << "\033[1A \n\n";

    if (memory.buffered)
    {

        for (unsigned short i = 0; i< memory.buff_size; i++)
        {
            if (i != memory.current_probe)
            {
                if (memory.probes[i] <= memory.stable - memory.loseless)
                    cout << "\033[1;32m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
                else if (memory.probes[i] >= memory.stable + memory.loseless)
                    cout << "\033[1;31m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
                else
                    cout << "\033[1;30m" << "\033[8C" << " " << (unsigned short)memory.probes[i] << "\n";
            }
            else
                cout << "\033[1;33m" << "\033[8C" << ">" << (unsigned short)memory.probes[i] << "\n";

        }
        for (unsigned short i = 0; i<= memory.buff_size; i++)
        {
            cout << "\033[1A";
        }
        cout << "\n";

        for (unsigned short i = 0; i< memory.buff_size; i++)
        {
            if (i != memory.current_probe_small)
            {
                if (memory.sector_small[i] <= memory.stable - memory.loseless)
                {
                    if (memory.sector_small[i] < 10)
                        cout << "\033[1;32m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;32m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
                }
                else if (memory.sector_small[i] >= memory.stable + memory.loseless)
                {
                    if (memory.sector_small[i] < 10)
                        cout << "\033[1;31m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;31m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
                }
                else
                {
                    if (memory.sector_small[i] < 10)
                        cout << "\033[1;30m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;30m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " \n";
                }
            }
            else
            {
                if (memory.sector_small[i] < 10)
                    cout << "\033[1;33m" << "\033[12C" << (unsigned short)memory.sector_small[i] << " <\n";
                else
                    cout << "\033[1;33m" << "\033[12C" << (unsigned short)memory.sector_small[i] << "<\n";
            }
        }
    }
    else
    {
        if (memory.load <= memory.stable - memory.loseless)
            cout << "\033[1;32m" << "\033[9C" << (unsigned short)memory.load << " \n";
        else if (memory.load >= memory.stable + memory.loseless)
            cout << "\033[1;31m" << "\033[9C" << (unsigned short)memory.load << " \n";
        else
            cout << "\033[1;30m" << "\033[9C" << (unsigned short)memory.load << " \n";
    }

    cout << "\n" << "\033[9C";
    if (memory.frequency == 'l')
    {
        if (memory.mod <= 0)
        {
            unsigned short s = 5 * (double(-memory.mod) / double(memory.lin_num*memory.steps));
            if (s == 0 && memory.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(memory.mod) / double(memory.lin_num*memory.steps));
            if (s == 0 && memory.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }
    if (memory.frequency == 'q')
    {
        if (memory.mod <= 0)
        {
            unsigned short s = 5 * (double(-memory.mod) / double(memory.steps*memory.steps));
            if (s == 0 && memory.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(memory.mod) / double(memory.steps*memory.steps));
            if (s == 0 && memory.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (memory.frequency == 'f')
    {
        unsigned short max_mod = 0;
        unsigned short max_mod_prev = 0;
        for (unsigned int i = 0; i <= memory.steps; i++)
        {
            max_mod_prev = max_mod;
            max_mod = i + max_mod_prev;
        }
        if (memory.mod <= 0)
        {
            unsigned short s = 5 * (double(-memory.mod) / double(max_mod));
            if (s == 0 && memory.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for (unsigned short i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(memory.mod) / double(max_mod));
            if (s == 0 && memory.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (memory.buffered)
    {
        for (unsigned short i = 0; i<= memory.buff_size+1; i++)
        {
            cout << "\033[1A";
        }
    }
    else
        cout << "\033[3A";

    cout << "\033[1A \n\n";

    if (temperature.buffered)
    {
        for (unsigned short i = 0; i< temperature.buff_size; i++)
        {
            if (i != temperature.current_probe)
            {
                if (temperature.probes[i] <= temperature.stable - temperature.loseless)
                    cout << "\033[1;32m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
                else if (temperature.probes[i] >= temperature.stable + temperature.loseless)
                    cout << "\033[1;31m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
                else
                    cout << "\033[1;30m" << "\033[16C" << " " << (unsigned short)temperature.probes[i] << "\n";
            }
            else
                cout << "\033[1;33m" << "\033[16C" << ">" << (unsigned short)temperature.probes[i] << "\n";
        }
    }
    else
    {
        if (temperature.value <= temperature.stable - temperature.loseless)
            cout << "\033[1;32m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
        else if (temperature.value >= temperature.stable + temperature.loseless)
            cout << "\033[1;31m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
        else
            cout << "\033[1;30m" << "\033[16C" << " " << (unsigned short)temperature.value << "\n";
    }


    cout << "\n" << "\033[17C";
    if (temperature.frequency == 'l')
    {
        if (temperature.mod <= 0)
        {
            unsigned short s = 5 * (double(-temperature.mod) / double(temperature.steps*temperature.lin_num));
            if (s == 0 && temperature.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(temperature.mod) / double(temperature.steps*temperature.lin_num));
            if (s == 0 && temperature.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }
    if (temperature.frequency == 'q')
    {
        if (temperature.mod <= 0)
        {
            unsigned short s = 5 * (double(-temperature.mod) / double(temperature.steps*temperature.steps));
            if (s == 0 && temperature.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(temperature.mod) / double(temperature.steps*temperature.steps));
            if (s == 0 && temperature.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for ( unsigned int i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (temperature.frequency == 'f')
    {
        unsigned short max_mod = 0;
        unsigned short max_mod_prev = 0;
        for (unsigned int i = 0; i <= temperature.steps; i++)
        {
            max_mod_prev = max_mod;
            max_mod = i + max_mod_prev;
        }
        if (temperature.mod <= 0)
        {
            unsigned short s = 5 * (double(-temperature.mod) / double(max_mod));
            if (s == 0 && temperature.mod < 0)
                s = 1;
            cout << "\033[1;32m";
            for (unsigned short i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
        else
        {
            unsigned short s = 5 * (double(temperature.mod) / double(max_mod));
            if (s == 0 && temperature.mod > 0)
                s = 1;
            cout << "\033[1;31m";
            for (unsigned short i = s; i > 0; i--)
            {
                cout << "=";
            }
            cout << "\033[1;30m";
            for (unsigned short m = 5 - s; m > 0; m--)
            {
                cout << "=";
            }
        }
    }

    if (temperature.buffered)
    {

        for (unsigned short i = 0; i<= temperature.buff_size+1; i++)
        {
            cout << "\033[1A";
        }
        cout << "\n";

        for (unsigned short i = 0; i< temperature.buff_size; i++)
        {
            if (i != temperature.current_probe_small)
            {
                if (temperature.sector_small[i] <= temperature.stable - temperature.loseless)
                {
                    if (temperature.sector_small[i] < 10)
                        cout << "\033[1;32m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;32m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
                }
                else if (temperature.sector_small[i] >= temperature.stable + temperature.loseless)
                {
                    if (temperature.sector_small[i] < 10)
                        cout << "\033[1;31m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;31m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
                }
                else
                {
                    if (temperature.sector_small[i] < 10)
                        cout << "\033[1;30m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "  \n";
                    else
                        cout << "\033[1;30m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " \n";
                }
            }
            else
            {
                if (temperature.sector_small[i] < 10)
                    cout << "\033[1;33m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << " <\n";
                else
                    cout << "\033[1;33m" << "\033[20C" << (unsigned short)temperature.sector_small[i] << "<\n";
            }
        }
        for (unsigned short i = 0; i<= temperature.buff_size; i++)
        {
            cout << "\033[1A";
        }
    }
    else
        cout << "\033[3A";

    cout << "\n";

    cout << "\033[25C" << "\033[1;33m" << "\033[1A";
    if (battery_state == 0)
    {
        cout << "<< NONE >>";
    }
    for (unsigned short i = battery.load/10; i > 0; i--)
    {
        cout << "]";
    }
    if (battery_state == 1)
    {
        for (unsigned short i =10 - battery.load/10; i > 0; i--)
        {
            cout << "\033[1;31m" << "O";
        }
    }
    if (battery_state == 2)
    {
        for (unsigned short i =10 - battery.load/10; i > 0; i--)
        {
            cout << "\033[1;32m" << "X";
        }
    }
    if (battery_state == 3)
    {
        for (unsigned short i =10 - battery.load/10; i > 0; i--)
        {
            cout << "\033[1;32m" << ">";
        }
    }
    if (battery_state == 4)
    {
        for (unsigned short i =10 - battery.load/10; i > 0; i--)
        {
            cout << "\033[1;31m" << "<";
        }
    }
    cout << "\n \n" << "\033[30C";
    if (times.mod > 0)
        cout << "\033[1;31m";
    else
        cout << "\033[1;33m";
    cout << "\n";
    cout << "\033[25C";
    cout << "\033[1;31m";
    for (unsigned short i =10*(double(times.mod)/double(times.steps)); i > 0; i--)
    {
        cout << "=";
    }
    cout << "\033[1;30m";
    for (unsigned short i =10 - 10*(double(times.mod)/double(times.steps)); i > 0; i--)
    {
        cout << "=";
    }

    cout << "\n \n" << "\033[32C";
    if (energy.mod > 0)
        cout << "\033[1;31m";
    else
        cout << "\033[1;33m";
    cout << "\n";
    cout << "\033[25C";
    cout << "\033[1;31m";
    for (unsigned short i =10*(double(energy.mod)/double(energy.steps)); i > 0; i--)
    {
        cout << "=";
    }
    cout << "\033[1;30m";
    for (unsigned short i =10 - 10*(double(energy.mod)/double(energy.steps)); i > 0; i--)
    {
        cout << "=";
    }


    cout << "\n \n" << "\033[25C";
    if (bulwers.total_mod > 1)
    {
        cout << "\033[1;31m" << "↑↑↑↑";
        cout << "\033[1;30m" << "||↓↓↓↓";
    }
    else if (bulwers.total_mod < 1)
    {
        cout << "\033[1;30m" << "↑↑↑↑||";
        cout << "\033[1;32m" << "↓↓↓↓";
    }
    else
    {
        cout << "\033[1;30m" << "↑↑↑↑";
        cout << "\033[1;34m" << "||";
        cout << "\033[1;30m" << "↓↓↓↓";
    }
    if (bulwers.total_mod > 1)
        cout << "\033[1;31m";
    else if (bulwers.total_mod < 1)
        cout << "\033[1;32m";
    else
        cout << "\033[1;30m";
    cout << "\n" << "\033[25C" << "          " << "\033[10D" << bulwers.total_mod;
    cout << "\033[1;33m";
    cout << "\n" << "\033[25C" << "          " << "\033[10D" << bulwers.step;
    cout << "\n\n" << "\033[25C" << "          " << "\033[10D";
    if (bulwers.value == 0)
        cout << bulwers.wall_01;
    if (bulwers.value == 1)
        cout << bulwers.wall_02;
    if (bulwers.value == 2)
        cout << bulwers.wall_03;
    if (bulwers.value == 3)
        cout << bulwers.wall_04;
    if (bulwers.value == 4)
        cout << bulwers.wall_05;
    if (bulwers.value == 5)
        cout << bulwers.wall_06;
    if (bulwers.value == 6)
        cout << bulwers.wall_07;
    if (bulwers.value == 7)
        cout << bulwers.wall_08;
    if (bulwers.value == 8)
        cout << bulwers.wall_09;
    if (bulwers.value == 9)
        cout << bulwers.wall_10;
    if (bulwers.value == 10)
        cout << bulwers.wall_11;
    if (bulwers.value == 11)
        cout << bulwers.wall_12;
    if (bulwers.value == 12)
        cout << bulwers.wall_13;
    if (bulwers.value == 13)
        cout << bulwers.wall_14;
    if (bulwers.value == 14)
        cout << bulwers.wall_15;

    cout << "\033[14A";
    if (cpu.mod > 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
    }
    else if (cpu.mod < 0)
    {
        cout << "\033[1;32m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << cpu.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)cpu.load << "%";
    }

    ////

    if (memory.mod > 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
    }
    else if (memory.mod < 0)
    {
        cout << "\033[1;32m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << memory.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)memory.load << "%";
    }

    ////

    if (temperature.mod > 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
    }
    else if (temperature.mod < 0)
    {
        cout << "\033[1;32m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << temperature.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << temperature.value << "ºC";
    }

    ////

    if (battery.mod < 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << -battery.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
    }
    else if (battery.mod > 0)
    {
        cout << "\033[1;32m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << -battery.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << battery.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (unsigned short)battery.load << "%";
    }

    ////

    if (times.mod > 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << times.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << times.value << "h";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << times.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << times.value << "h";
    }

    ////

    if (energy.mod > 0)
    {
        cout << "\033[1;31m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << energy.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (energy.start + energy.wide - energy.value)/3600 << "h";
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << energy.mod;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << (energy.start + energy.wide - energy.value)/3600 << "h";
    }

    ////

    if (mod_bat_plug > 0)
    {
        cout << "\033[1;32m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << mod_bat_plug;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << battery_state;
    }
    else
    {
        cout << "\033[1;33m";
        cout << "\n\n" << "\033[38C" << "      " << "\033[6D" << mod_bat_plug;
        cout << "\033[1A" << "\n" << "\033[44C" << "      " << "\033[6D" << battery_state;
    }
    cout << "\033[16A" << "\n\n";

    if (1)
        HDBG.print();
}

void hard_dbg::print()
{


    //------------WARNING-------------//

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
    //   !!! HARDLY DEBUG BEGIN !!!   //
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//.



    spacer = 52;
    line = 0;

    cout << "\033[" << spacer << "C";
    cout << "\033[1;32m";

    cout << "cload" << " " << (unsigned short)cpu.load << " %";
    chck_s ();
    cout << "cstbl" << " " << (unsigned short)cpu.stable << " %";
    chck_s ();
    cout << "closs" << " " << (unsigned short)cpu.loseless;
    chck_s ();
    cout << "clnum" << " " << (unsigned short)cpu.lin_num;
    chck_s ();
    cout << "cfreq" << " " << cpu.frequency;
    chck_s ();
    cout << "mload" << " " << (unsigned short)memory.load << " %";
    chck_s ();
    cout << "mstbl" << " " << (unsigned short)memory.stable << " %";
    chck_s ();
    cout << "mloss" << " " << (unsigned short)memory.loseless;
    chck_s ();
    cout << "mlnum" << " " << (unsigned short)memory.lin_num;
    chck_s ();
    cout << "mfreq" << " " << memory.frequency;
    chck_s ();
    cout << "bload" << " " << (unsigned short)battery.load << " %";
    chck_s ();
    cout << "bstbl" << " " << (unsigned short)battery.stable << " %";
    chck_s ();
    cout << "bloss" << " " << (unsigned short)battery.loseless;
    chck_s ();
    cout << "blnum" << " " << (unsigned short)battery.lin_num;
    chck_s ();
    cout << "bstat" << " " << battery_state;
    chck_s ();
    cout << "bfreq" << " " << battery.frequency;
    chck_s ();
    cout << "pbplg" << " " << prev_bat_plug;
    chck_s ();
    cout << "bcapa" << " " << battery_capacity;
    chck_s ();
    cout << "boplg" << " " << once_plugged;
    chck_s ();
    cout << "tempv" << " " << (unsigned short)temperature.value << "ºC";
    chck_s ();
    cout << "tstbl" << " " << (unsigned short)temperature.stable << "ºC";
    chck_s ();
    cout << "tloss" << " " << (unsigned short)temperature.loseless;
    chck_s ();
    cout << "tlnum" << " " << (unsigned short)temperature.lin_num;
    chck_s ();
    cout << "tunit" << " " << (unsigned short)temperature.unit;
    chck_s ();
    cout << "tfreq" << " " << temperature.frequency;
    chck_s ();
    cout << "tempt" << " " << temp_t;
    chck_s ();
    cout << "flutm" << " " << flu_timer;
    chck_s ();
    cout << "gtflu" << " " << get_flu;
    chck_s ();
    cout << "timev" << " " << (unsigned short)times.value << " h";
    chck_s ();
    cout << "tmstr" << " " << (unsigned short)times.start << " h";
    chck_s ();
    cout << "timew" << " " << (unsigned short)times.wide << " h";
    chck_s ();
    cout << "tmstp" << " " << (unsigned short)times.steps;
    chck_s ();
    cout << "tmlnr" << " " << (unsigned short)times.lin_num;
    chck_s ();
    cout << "timef" << " " << times.frequency;
    chck_s ();
    cout << "nrgvl" << " " << (unsigned short)energy.value;
    chck_s ();
    cout << "nrgst" << " " << (unsigned short)energy.start;
    chck_s ();
    cout << "nrgwd" << " " << (unsigned short)energy.wide;
    chck_s ();
    cout << "nrstp" << " " << (unsigned short)energy.steps;
    chck_s ();
    cout << "nrgln" << " " << (unsigned short)energy.lin_num;
    chck_s ();
    cout << "nrgfq" << " " << energy.frequency;
    chck_s ();



    cout << "\033[" << line+4 << "A\n";



    //-------------WARNING-------------//

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
    //   !!! HARDLY DEBUG ENDING !!!   //
    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//.


}


void hard_dbg::chck_s()
{
    if(line >= HDBG.max_s+3 && line >= 10 )
    {
        line = 0;
        spacer += 14;
        cout << "\033[1;32m\033[" << HDBG.max_s+4 << "A\n";
    }
    else
    {
        if (line % 2 == 1)
            cout << "\033[1;32m";
        else
            cout << "\033[1;36m";
        cout << "\n";
        line++;
    }
    cout << "\033[" << spacer << "C";
}




/*
int main ()
{
    Config set;
    set.readFile ( "./config.cfg" );
    Core * core = new Core ();
    core->load_config ( &set );
    core->gui_init ();
    core->bulwers_init ();
    core->run ();
}
*/
Core::Core ( eyes_view * neyes )
{
    timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout () ), this, SLOT ( on_timer_tick () ) );
    eyes = neyes;
}

void Core::load_config ( eConfig * cfg )
{
    cpu.frequency = cfg->lookupValue ( "core.cpu.frequency", 'f' );
    cpu.lin_num = cfg->lookupValue ( "core.cpu.linear_modifier", 0 );
    cpu.stable = cfg->lookupValue ( "core.cpu.stable", 25 );
    cpu.steps = cfg->lookupValue ( "core.cpu.steps", 10 );
    cpu.loseless = cfg->lookupValue ( "core.cpu.adaptation", 10 );
    cpu.buffered = cfg->lookupValue ( "core.cpu.buffered", true );
    cpu.buff_size = cfg->lookupValue ( "core.cpu.buffer_size", 10 );
    //mem_section
    memory.frequency = cfg->lookupValue ( "core.memory.frequency", 'q' );
    memory.lin_num = cfg->lookupValue ( "core.memory.linear_modifier", 2 );
    memory.stable = cfg->lookupValue ( "core.memory.stable", 25 );
    memory.steps = cfg->lookupValue ( "core.memory.steps", 8 );
    memory.loseless = cfg->lookupValue ( "core.memory.adaptation", 10 );
    memory.buffered = cfg->lookupValue ( "core.memory.buffered", true );
    memory.buff_size = cfg->lookupValue ( "core.memory.buffer_size", 10 );
    //temperature_section
    temperature.frequency = cfg->lookupValue ( "core.temperature.frequency", 'q' );
    temperature.lin_num = cfg->lookupValue ( "core.temperature.linear_modifier", 2 );
    temperature.stable = cfg->lookupValue ( "core.temperature.stable", 56 );
    temperature.steps = cfg->lookupValue ( "core.temperature.steps", 12 );
    temperature.loseless = cfg->lookupValue ( "core.temperature.adaptation", 2 );
    temperature.buffered = cfg->lookupValue ( "core.temperature.buffered", true );
    temperature.buff_size = cfg->lookupValue ( "core.temperature.buffer_size", 10 );
    temperature.unit = cfg->lookupValue ( "core.temperature.unit", 1 );
    //battery_section
    battery_capacity = cfg->lookupValue ( "core.battery.capacity", 4700 );
    battery.frequency = cfg->lookupValue ( "core.battery.frequency", 'l' );
    battery.lin_num = cfg->lookupValue ( "core.battery.linear_modifier", 0 );
    battery.stable = cfg->lookupValue ( "core.battery.stable", 25 );
    battery.steps = cfg->lookupValue ( "core.battery.steps", 8 );
    battery.loseless = cfg->lookupValue ( "core.battery.adaptation", 10 );
    battery.buffered = cfg->lookupValue ( "core.battery.buffered", false );
    battery.buff_size = cfg->lookupValue ( "core.battery.buffer_size", 10 );
    //times_sector
    times.frequency = cfg->lookupValue ( "core.times.frequency", 'q' );
    times.lin_num = cfg->lookupValue ( "core.times.quad_modifier", 2 );
    times.start = cfg->lookupValue ( "core.times.start", 20 );
    times.steps = cfg->lookupValue ( "core.times.steps", 6 );
    times.end = cfg->lookupValue ( "core.times.end", 6 );
    times.wide = cfg->lookupValue ( "core.times.wide", 6 );
    //energy_sector
    energy.frequency = cfg->lookupValue ( "core.energy.frequency", 'q' );
    energy.lin_num = cfg->lookupValue ( "core.energy.quad_modifier", 2 );
    energy.start = cfg->lookupValue ( "core.energy.start", 16 );
    energy.steps = cfg->lookupValue ( "core.energy.steps", 6 );
    energy.end = cfg->lookupValue ( "core.energy.end", 0 );
    energy.wide = cfg->lookupValue ( "core.energy.wide", 6 );
    //bulwers_walls_sector
    bulwers.wall_01 = cfg->lookupValue ("core.bulwers.wall_01", 300 );
    bulwers.wall_02 = cfg->lookupValue ("core.bulwers.wall_02", 500 );
    bulwers.wall_03 = cfg->lookupValue ("core.bulwers.wall_03", 800 );
    bulwers.wall_04 = cfg->lookupValue ("core.bulwers.wall_04", 1300 );
    bulwers.wall_05 = cfg->lookupValue ("core.bulwers.wall_05", 2100 );
    bulwers.wall_06 = cfg->lookupValue ("core.bulwers.wall_06", 3400 );
    bulwers.wall_07 = cfg->lookupValue ("core.bulwers.wall_07", 5500 );
    bulwers.wall_08 = cfg->lookupValue ("core.bulwers.wall_08", 8900 );
    bulwers.wall_09 = cfg->lookupValue ("core.bulwers.wall_09", 14400 );
    bulwers.wall_10 = cfg->lookupValue ("core.bulwers.wall_10", 23300 );
    bulwers.wall_11 = cfg->lookupValue ("core.bulwers.wall_11", 37700 );
    bulwers.wall_12 = cfg->lookupValue ("core.bulwers.wall_12", 61600 );
    bulwers.wall_13 = cfg->lookupValue ("core.bulwers.wall_13", 98700 );
    bulwers.wall_14 = cfg->lookupValue ("core.bulwers.wall_14", 159700 );
    bulwers.wall_15 = cfg->lookupValue ("core.bulwers.wall_15", 258400 );

}

void Core::run ()
{
    cerr << "[info :] starting core.\n";
    bulwers_init ();
    cerr << "[info :] bulwers inited\n";
    Core::gui_init();
    cerr << "[info :] gui inited\n";
    s_anim.face_prev = "slp_10";
    cerr << "[info :] s_anim.face_prev set to " << s_anim.face_prev.toStdString() << "\n";
    do
    {
        cerr << "[info :] Is in wake up\n";
        times.value = get_time ().hour/3600;

        // TODO 03 : It PROPABLY won't work correctly and it should works on config values.

        wake_up_prepare();
        eyes->anims_reload();
    } while (!wake_up);
    cerr << "[info :] wake up ended";
    eyes->anims_send ("cusual_01", "slp_10_close", "cusual_01_open", 0, 4);
    eyes->anims_reload();
    cerr << "[info :] end of core preparing\n";
    timer->start( 1000 );
}

void Core::on_timer_tick ()
{
    core_step ++;
    bulwers_update ();
    eyes->graphics_prepare();
    gui_refresh ();
}
