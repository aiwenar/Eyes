/*  copyrights © 2010 2011 Damian Chilinski and Krzysztof Mędrzycki

   _______         ______      ______________      _______________
  /       \\     _/      \\_   \     ____    \\   /              //
 /    __   \\   /   ____   \\   |   ||   \    || |     _________//
|    // \___|| |   //   \   ||  |   ||    |   || |    ||
|   ||        /   ||     |  \\  |   ||___/   //  |    ||_______
|   ||        |   ||     |   || |        ___//   |            ||
|   ||   ___  \   ||     |   // |   |\   \\      |      ______||
|    \\_/   || |   \\___/   ||  |   ||\   \\     |    ||_______
 \         //   \_        _//   |   || \   \\    |             \\
  \_______//      \______//     |___|| /____\\    \_____________\\  By Chiliński Damian


Art by Chiliński Damian
GBS copyright 2010 *all rights reserved.


*/
//-------------------------------------------------------------------

#include <iostream>
#include <string.h>
#include <time.h>
#include </usr/include/qt4/Qt/qstring.h>
#include </usr/include/qt4/Qt/qvector.h>
#include </usr/include/qt4/Qt/qregexp.h>
#include </usr/include/qt4/Qt/qstringlist.h>
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
#include "bulwers.h"
#include "eyes_view.h"
#include "defines.hxx"


using namespace std;


struct sdate
{
    int day;
    int month;
    int day_num;
    int hour;
    int year;
};


struct bulwers_core
{
int special;
int longev;
int happy;
int step;
};


sdate       get_time    ();
static void print       ( char * str );
static void print       ( char * str, int pos );
static void print_gui   ();
int         temperatura ();
int         bat_plugged ();
int         bateria     ();
int         C_LOAD      ();
int         M_LOAD      ();
int         P_LIST      ();
int         U_TIME      ();
int         about       ();
int         naglowek_in (char* version);
int         naglowek_out(char* version);
int         felltext    (char*downgrade);
int pulsetext (char*text, int delay, int repeat, int position);
pict_layers bulwers_init ();

static bool event_now,
            get_flu,
            wake_up;

//------------------





void core_main ()
{
    event_now = false;
    wake_up = false;
    int f=0;
    int a;
    cout << "\033[40m" << endl;
    cout << "\033[2J\033[0;0H";
    cout << "\033[32m" << endl;
    naglowek_in ("v.0.0.1a-01");
    print ( " Welcome in eyes project!\nPlease give number of stages (0 for infinitive): " );
    cin >> a;
    naglowek_out ("v.0.0.1a-01");
    cout << "\033[2J\033[0;0H";
    cout << "\033[90A" << endl;
    cout << "\033[2J";
    print_gui ();
    cout << "\033[1;33m";

    //Event ev;
    if (a != 0)
    {
        while (f<a)
        {
            //if ( evs->poll ( &ev ) and not event_now)
            //{
                cout << "\033[0;22H" << f << " (" << (100*f)/a << "%)" << '\n';
                bulwers_init ();
                event_now = true;
                usleep (200000);
            //}
            //if ( not evs->poll ( &ev ) )
            //{
            //    event_now = false;
            //}
            //if (!event_now)
            //{
            //    cout << "\033[0;22H" << f << " (" << (100*f)/a << "%)" << '\n';
            //    bulwers_init ();
            //    sleep (1);
            //    f++;
            //}
        }
    }
    else
    {
        while (true)
        {
            cout << "\033[0;22H" << f << '\n';
            bulwers_init ();
            sleep(1);
            f++;
        }
    }
    about();
    cout << "\033[0m \033[2J \033[0;0H";
}

//===========================

inline static void print ( char * str )
{
  print ( str, 0 );
}

static void print ( char * str, int pos )
{
  cout << "\033[1;32m";
  int plus = pos+1;
  for ( int i=0 ; i<strlen(str) ; i++ )
  {
    cout << str[i] << '\n';
    if ( str[i] == '\n' )
    {
      usleep ( 50000 );
      plus = pos+1;
    }
    else if ( str[i] == '.' or str[i] == '!' or str[i] == '?' )
      usleep ( 30000 );
    else
      usleep ( 20000 );
    cout << "\033[1A \033[" << plus << "C";
    plus++;
  }
}

inline void print_gui ()
{
  cout  << "\033[2J \033[0;0H";
  print ( " stage:\ncurrent cpu probe:" );
  cout  << "\033[0;35H";
  print ( " cpu probes table:", 35 );
  cout << "\033[3;40H";
  print ( " cpu:\nmemory:\nproclist:\nuptime:", 40 );
  cout << "\033[4;75H";
  print ( " day of week:\nday of month:\nmonth:\nyear:\ntime:\n", 75 );
  cout << "\033[10;0H";
  print ( " batery state:\nbatery power:\ntemperature:\nenergy:" );
  cout << "\033[10;35H";
  print ( " specjal:\nlongev:\nhappy:\nbulwers:", 35 );
  cout << "\033[10;60H";
  print ( " o\nt\nh\ne\nr\ns", 60 );
  cout << "\033[8m";
}

//---------------------------
int C_LOAD ()
{
glibtop_init();
glibtop_cpu cpu;
glibtop_get_cpu (&cpu);

static unsigned short cpu_load = 0;
static unsigned long p_idle = 0;
static unsigned long p_total = 0;
static unsigned long a_idle = 0;
static unsigned long a_total = 0;
static unsigned short d_total = 0;
static unsigned short d_idle = 0;
a_idle = cpu.idle;
a_total = cpu.total;

        d_total = a_total - p_total;
        d_idle = a_idle - p_idle;
        cpu_load = (100*(d_total - d_idle)) / d_total;
        p_idle = a_idle;
        p_total = a_total;

return cpu_load;
}
//-------------------------
int M_LOAD ()
{
glibtop_init();
glibtop_mem memory;
glibtop_get_mem(&memory);

unsigned int mem_total = memory.total;
unsigned int mem_used = memory.used;
unsigned int mem_load = (100*mem_used)/mem_total;

return mem_load;
}
//-------------------------
int P_LIST ()
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

int temperatura ()
{
        fstream calosc ("/proc/acpi/thermal_zone/TZ00/temperature", fstream::in);

        string texcik;
        while (calosc.good())
                texcik+=calosc.get();

        int tempjuczer=texcik.find_first_of ("temperature");

        int i=0;
        int a=0;
        char temp[27];

        for (;i<27;i++) {
                temp[a] = texcik[25+a];
                a++;
                }

        int temperature;
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

        return powr;
}
//-----------------------------

int pulsetext (char * text, int delay, int repeat, int position)
{
    while (repeat>0)
    {
        if (repeat%2 == 1)
        cout << text << endl;
        else
        cout << "                    " << endl;

     cout << "\033[1A" << "\033[" << position << "C";
     usleep (100*delay);
     repeat--;
    }
    return 1;
}


int felltext (char*downgrade)
{
    cout << "\033[40m" << endl;

    int leng = strlen(downgrade);
    cout << endl << endl;
     for ( int i=0 ; i<(leng+2) ; i++ )
    {
        usleep (5000);
        if (i > 0 && i<leng)
        {
            cout << "\033[32m" << downgrade[i] << endl;
            cout << "\033[1;30m" << "\033[2A" << downgrade[i-1] << endl;
            cout << "\033[2A" << " " << endl;
            cout << "\033[1B" << endl;
        }
        if (i==0)
        {
            cout << "\033[1;32m" << downgrade[i] << endl;
        }
        if (i==leng)
        {
            cout << " " << endl;
            cout << "\033[32m" << "\033[2A" << downgrade[i-1] << endl;
            cout << "\033[2A" << " " << endl;
            cout << "\033[1B" << endl;
        }
        if(i==(leng+1))
        {
            cout << " " << endl;
            cout << "\033[2A" << " " << endl;
            cout << "\033[2A" << " " << endl;
            cout << "\033[1B" << endl;
        }
    }
return 1;
}

int about()
{
    cout << "\033[37m" << "\033[40m";
    int i= 95;
    while (i>0)
    {
        usleep (500);
        cout << endl;
        i--;
    }


    cout << "\033[37m";
    cout << "\033[1;1H";
    cout << "\033[1;30m";
    cout << "  ________________     _________________   ___________" << endl
         << " /   _________   ||   |    /   _______// /     ___   \\\\" << endl
         << "|   ||______  \\   \\\\_/   /|   ||______  |    //___\\___||" << endl
         << "|     _____||  \\     ___//|     _____||  \\_______    \\\\" << endl
         << "|   ||______    \\   \\\\    |   ||________|   \\\\___\\    ||" << endl
         << "|           \\\\   \\   \\\\   |             |             ||" << endl
         << " \\___________\\\\   \\___||   \\_____________\\___________//" << endl;
    usleep (10000);
    cout << "\033[37m" << "\033[9A";
    cout << "  ________________     _________________   ___________" << endl
         << " /   _________   ||   |    /   _______// /     ___   \\\\" << endl
         << "|   ||______  \\   \\\\_/   /|   ||______  |    //___\\___||" << endl
         << "|     _____||  \\     ___//|     _____||  \\_______    \\\\" << endl
         << "|   ||______    \\   \\\\    |   ||________|   \\\\___\\    ||" << endl
         << "|           \\\\   \\   \\\\   |             |             ||" << endl
         << " \\___________\\\\   \\___||   \\_____________\\___________//" << endl;



    cout << endl << endl << endl;
    print ( " by Chilinski Damian and Medrzycki Krzysztof" );
    pulsetext ("...", 500, 4, 45);
    cout << "\033[90D";
    print ( "                                                " );
    cout << endl;
    cout << "\033[37m";
    cout << "\033[1;1H";
    cout << "\033[1;30m";
    cout << "  ________________     _________________   ___________" << endl
         << " /   _________   ||   |    /   _______// /     ___   \\\\" << endl
         << "|   ||______  \\   \\\\_/   /|   ||______  |    //___\\___||" << endl
         << "|     _____||  \\     ___//|     _____||  \\_______    \\\\" << endl
         << "|   ||______    \\   \\\\    |   ||________|   \\\\___\\    ||" << endl
         << "|           \\\\   \\   \\\\   |             |             ||" << endl
         << " \\___________\\\\   \\___||   \\_____________\\___________//" << endl;
    usleep (10000);
    cout << "\033[1;33m";
return 1;
}


int naglowek_in (char* version)
{
    cout << "\033[37m";
    cout << "\033[1;1H";
    cout << "\033[1;30m";
    cout << "  _______   _______  _____" << endl
         << " |  ___  \\_/    ___|/  ___|" << endl
         << " |  ___|\\   /|  ___|\\___  \\" << endl
         << " |_____| |_| |____________/" << endl
         << " ===============================" << endl
         << " " << version << endl;
    usleep (10000);
    cout << "\033[37m" << "\033[9A";
    cout << "  _______   _______  _____" << endl
         << " |  ___  \\_/    ___|/  ___|" << endl
         << " |  ___|\\   /|  ___|\\___  \\" << endl
         << " |_____| |_| |____________/" << endl
         << " ===============================" << endl
         << " " << version << endl;

    cout << endl << endl << endl;
}

int naglowek_out (char* version)
{
    cout << "\033[37m";
    cout << "\033[1;1H";
    cout << "\033[1;30m";
    cout << "  _______   _______  _____" << endl
         << " |  ___  \\_/    ___|/  ___|" << endl
         << " |  ___|\\   /|  ___|\\___  \\" << endl
         << " |_____| |_| |____________/" << endl
         << " ===============================" << endl
         << " " << version << endl;
    usleep (10000);

}

int pos;

void print_event ( char * ev_text )
{
  cout << "\033[" << 10+pos << ";66H" << ev_text << '\n';
  pos++;
}

/*

Welcome to Eyes project!

  ________________     _________________   ___________
 /   _________   ||   |    /   _______// /     ___   \\
|   ||______  \   \\_/   /|   ||______  |    //___\___||
|     _____||  \     ___//|     _____||  \_______    \\
|   ||______    \   \\    |   ||________|   \\___\    ||
|           \\   \   \\   |             |             ||
 \___________\\   \___||   \_____________\___________//


 _______   _______  _____
|  ___  \_/    ___|/  ___|
|  ___|\   /|  ___|\___  \
|_____| |_| |____________/
===============================
v.0.0.1a-06


 ______   _______  _____
|  ___ \_/    ___|/  ___|
|  ___\   /|  ___|\___  \
|_____||_| |____________/
===============================
v.0.0.1a-06


  _______________  ___       ___    _______________   ____________
 /              //|   ||    |   || /              // /            \\
|     _________// |   \\    /   |||     _________// |     _____    ||
|    ||_______     \   \\__/   // |    ||_______    |    //____\___||
|            ||     \         //  |            ||   |             \\
|      ______||      \_     _//   |      ______||    \_______      ||
|    ||_______         |   ||     |    ||________   |   \\___\     ||
|             \\       |   ||     |             \\  |              ||
 \_____________\\      |___||      \_____________\\  \____________//


*/





//---------------------------------------------------------------------
/*
       ________  _________   ________  __________        ________  _____
      /       ||/        || |   ___//  \_____   ||      /       ||/   //
     /   __   |/   __    || |  ||__          |  ||     /   __   |/   //
    /   // |      // |   || |    __||        |  ||    /   // |      //
   /   //  |     //  |   || |  ||__     __   |  ||   /   //  |     //
  /   //   |    //   |   || |      \\  |  \_/   //  /   //   |    //
 /___//    |___//    |___|| |_______\\  \______//  /___//    |___// Art by Chiliński Damian 2010




 */
//----------------------------------------------------------------------



//----------------------------------------

pict_layers bulwers_init()
{
    //----create bulwers structure

    bulwers_core bulwers;
    pict_layers pics;

    //----initializing values
    static int prev_happy = 0;
    static bool first_play = true;
    if (first_play)
    {
        bulwers.happy = 0;
        bulwers.longev = 0;
        bulwers.special = 0;
        pics.bulwers = 0;
        bulwers.step = 0;
    }
    //critical values
    static int core_day = get_time ().day;
    static int core_dnum = get_time ().day_num;
    static int core_month = get_time ().month;
    static int core_year = get_time ().year;
    int core_time = get_time ().hour;
    int core_battery_plugged = bat_plugged ();
    int core_temperature = temperatura ();
    int core_battery = bateria();
    static int cpu_probes[10];
    static int current_probe = 0;

                //Event ev;

    //if ( evs->poll ( &ev ) and not event_now )
    //{
    //    current_probe--;
    //}
    //else
        cpu_probes[current_probe] = C_LOAD ();

    if (first_play)
        cpu_probes[current_probe] = 20;

    current_probe++;

    if (current_probe == 10)
        current_probe = 0;

    int core_cpu_load = (cpu_probes[0] +
                         cpu_probes[1] +
                         cpu_probes[2] +
                         cpu_probes[3] +
                         cpu_probes[4] +
                         cpu_probes[5] +
                         cpu_probes[6] +
                         cpu_probes[7] +
                         cpu_probes[8] +
                         cpu_probes[9]) / 10;
    int core_memory = M_LOAD ();
    int core_proclist = P_LIST ();
    int core_uptime = U_TIME ();
    static int energy = 54000;
    static bool once_plugged = false;
    static int prev_bat_plug = core_battery_plugged;


    //----first sector

    cout << "\033[2;22H" << current_probe+1 << " " << "\033[1;55H"
         << cpu_probes [0] << ' '
         << cpu_probes [1] << ' '
         << cpu_probes [2] << ' '
         << cpu_probes [3] << ' '
         << cpu_probes [4] << ' '
         << cpu_probes [5] << ' '
         << cpu_probes [6] << ' '
         << cpu_probes [7] << ' '
         << cpu_probes [8] << ' '
         << cpu_probes [9] << '\n';
    if (cpu_probes[9] == 0){}
    else cout << "\033[3;53H" << core_cpu_load;
    cout << "\033[4;53H" << core_memory << "%"
         << "\033[5;53H" << core_proclist
         << "\033[6;53H" << core_uptime << " seconds" //<< "that is: " << core_uptime/3600 << " hours" << " and " << (core_uptime/60) - ((core_uptime/3600)*60) << "minutes"
         << "\033[4;92H" << core_day
         << "\033[5;92H" << core_dnum
         << "\033[6;92H" << core_month
         << "\033[7;92H" << core_year
         << "\033[8;92H" << core_time << " seconds that is:" << core_time/3600 << ":" << (core_time/60) - ((core_time/3600)*60) << ":" << core_time - ((core_time/60)*60)
         << "\033[10;17H" << ( core_battery_plugged == 1 ? "just pluged" : ( core_battery_plugged == 2 ? "just unpluged" : ( core_battery_plugged == 3 ? "pluged      " : "unpluged     " ) ) )
         << "\033[11;17H" << core_battery << "MAh   "
         << "\033[12;17H" << core_temperature << "ºC"
         << '\n';

    //--Proggramer Manual
    /*
     Bulwers manual:

       Bulwers classes:

        bulwers.special - runs special ocasion mode
        pics.bulwers - true bulwers status eg. bulwers = 0 - everything is OK.
        bulwers.longev - long events appears on every bulwers - eg. low battery or high temperature.
        bulwers.happy - happyness modificator it has appear on some types of non-bad bulwers eg. 0-3



        bulwers.special list:
        0 - nothing special
        1 - weekand!
        2 - friday thirteenth !
        3 - start of school term
        4 - nicolas day
        5 - christmas
        6 - new year
        7 - wielkanoc
        8 - end of term

        pics.bulwers list:
        0 - everything is OK
        1 - something goes wrong - speed looking
        2 - something goes wrong - shocked
        3 - something happend - surpriced looking
        4+ - hardware evenst eg. cpu load to high, or mem load(experimental), or longev appears

        bulwers.longev list: (in priority order)
        1 - battery middle
        2 - battery low
        3 - it's late
        4 - it's warm
        5 - it's very late
        6 - it's very warm
        7 - they're sleeppy
        8 - it's hot
        9 - ther're very sleeppy
        10 - battery critically low
        11 - they just sleep...
        12 - it's extremally hot
        13 - OMG what a temperature!!!
        14 - they can't stand
        15 - they're sleepping



        animations list:

        to_picture_bulwers x
        to_picture_happy 1-2
        to_picture_sleeppy 1-3

    */

    //eves checking

    if (core_day == 7)
    {
        bulwers.special = 1;
    }
    if (core_day == 5)
    {
        if (core_dnum == 13)
        {
            bulwers.special = 2;
        }
    }
    if (core_month == 9)
    {
        if (core_day == 1)
        {
            bulwers.special = 3;
        }
    }
    if (core_month == 6)
    {
        if (core_day == 14)
        {
            bulwers.special = 8;
        }
    }
    if (core_month == 12)
    {
        if (core_day == 24)
        {
            bulwers.special = 5;
        }
        if (core_day == 31)
        {
            bulwers.special = 6;
        }
    }


    //--space for others events

    cout << "\033[10;46H" << bulwers.special << " ";

    pos = 0;

    //--filling energy status, Wake up Neo!



if (wake_up)
{
    if (energy >= 10000)
        cout << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)  " << endl;
    if (energy < 10000 && energy >= 1000)
        cout << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)   " << endl;
    if (energy < 1000 && energy >= 100)
        cout << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)    " << endl;
    if (energy < 100 && energy >= 10)
        cout << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)     " << endl;
    if (energy < 10)
        cout << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)      " << endl;





    //--long events checking


    static int battery_buffer = 0;
    if (core_battery_plugged == 2) battery_buffer = 0;
    if (core_battery_plugged == 4 && battery_buffer < 15) battery_buffer++;

    if (core_battery <= 2500 && bulwers.longev < 1 && core_battery_plugged == 4 && battery_buffer == 15)
    {
        bulwers.longev = 1;
        print_event ( "ev1 (battery <=2500 MAh)" );
    }

    if (core_battery <= 1500 && bulwers.longev < 2 && core_battery_plugged == 4 && battery_buffer == 15)
    {
        bulwers.longev = 2;
        print_event ( "ev2 (battery <= 1500 MAh)" );
    }

    if (core_time >= 75600 && bulwers.longev < 3)
    {
        bulwers.longev = 3;
        print_event ( "ev3 (it's late)" );
    }

    if (core_temperature >= 56 && bulwers.longev < 4)
    {
        bulwers.longev = 4;
        print_event ( "ev4 (temperature goes up to 56ºC)" );
    }

    if (core_time >= 82800 && bulwers.longev < 5)
    {
        bulwers.longev = 5;
        print_event ( "ev5 (it's very late)" );
    }

    if (core_temperature >= 58)
    {
        energy -=1;

        if(bulwers.longev < 6)
            bulwers.longev = 6;

        print_event ( "ev6 (temperature goes up to 58ºC)" );
    }

    if ((core_uptime >= 21600 || energy < 25200) && bulwers.longev < 7)
    {
        bulwers.longev = 7;
        print_event ( "ev7 (they're tired)" );
    }

    if (core_temperature >= 60)
    {
        energy -=2;

        if (bulwers.longev < 8)
            bulwers.longev = 8;

        print_event ( "ev8 (temperature goes up to 60ºC)" );
    }

    if ((core_uptime >= 28800 || energy < 14400) && bulwers.longev < 9)
    {
        bulwers.longev = 9;
        print_event ( "ev9 (they're very tired)" );
    }
    if (core_battery <= 500 && bulwers.longev < 10 && core_battery_plugged == 4 && battery_buffer == 15)
    {
        bulwers.longev = 10;

        if (pics.bulwers < 5)
            pics.bulwers = 5;

        print_event ( "ev10 (battery critical low)" );
    }
    if ((core_uptime >= 39600 || energy < 7200 )&& bulwers.longev < 11)
    {
        bulwers.longev = 11;

        if (pics.bulwers < 6)
            pics.bulwers = 6;

        print_event ( "ev11 (they've just slept...)" );
    }
    if (core_temperature >= 62)
    {
        energy -=3;

        if (bulwers.longev < 12)
            bulwers.longev = 12;

        if (pics.bulwers < 10)
            pics.bulwers = 10;

        print_event ( "ev12 (temperature goes up to 62ºC)" );
    }
    if (core_temperature >= 64)
    {
        energy -=4;

        if (bulwers.longev < 13)
            bulwers.longev = 13;

        if (pics.bulwers < 14)
            pics.bulwers = 14;

        print_event ( "ev13 (temperature goes up to 64ºC)" );
    }
    if ((core_uptime >= 50400 || energy < 3600) && bulwers.longev < 14)
    {
        bulwers.longev = 14;

        if (pics.bulwers < 16)
            pics.bulwers = 16;

        print_event ( "ev14 (they cant' stand)" );
    }
    if (energy <= 0)
    {
        pics.bulwers = 17;
        print_event ( "idiot... <.:zzZ:.>" );
    }
    //if (core_battery <= 2500 && bulwers.longev < 1) bulwers.longev = 1;
    //if (core_battery <= 2500 && bulwers.longev < 2) bulwers.longev = 2;

    cout << "\033[11;46H" << bulwers.longev << " " << '\n';


    //--loads checking


    //----CPU----

    //----40

    if (core_cpu_load >= 40)
    {
        if(pics.bulwers <= 4 && bulwers.longev < 9)
        {
            pics.bulwers = 4;
            energy -=1;
            bulwers.step = 1;
        }

        if(pics.bulwers <= 6 && bulwers.longev >= 9)
        {
            pics.bulwers = 6;
            energy -=1;
            bulwers.step = 4;
        }

        if(pics.bulwers <= 10 && bulwers.longev > 11)
        {
            pics.bulwers = 10;
            energy -=2;
            bulwers.step = 64;
        }
    }



    if (core_cpu_load >= 50)
    {
        if (pics.bulwers <= 6 && bulwers.longev < 8)
        {
            pics.bulwers = 6;
            energy -=1;
            bulwers.step = 4;
        }

        if (pics.bulwers <= 8 && bulwers.longev >= 8)
        {
            pics.bulwers = 8;
            energy -=1;
            bulwers.step = 16;
        }

        if (pics.bulwers <= 12 && bulwers.longev > 10)
        {
            pics.bulwers = 12;
            energy -=2;
            bulwers.step = 256;
        }
    }



    if (core_cpu_load >= 60)
    {
        if (pics.bulwers <= 8 && bulwers.longev < 7)
        {
            pics.bulwers = 8;
            energy -=2;
            bulwers.step = 16;
        }

        if (pics.bulwers <= 10 && bulwers.longev >= 7)
        {
            pics.bulwers = 10;
            energy -=2;
            bulwers.step = 64;
        }

        if (pics.bulwers <= 14 && bulwers.longev > 9)
        {
            pics.bulwers = 14;
            energy -=3;
            bulwers.step = 1024;
        }
    }



    if (core_cpu_load >= 70)
    {
        if(pics.bulwers <= 10 && bulwers.longev < 6)
        {
            pics.bulwers = 10;
            energy -=2;
            bulwers.step = 64;
        }

        if(pics.bulwers <= 12 && bulwers.longev >= 6)
        {
            pics.bulwers = 12;
            energy -=2;
            bulwers.step = 256;
        }

        if(pics.bulwers <= 15 && bulwers.longev > 8)
        {
            pics.bulwers = 15;
            energy -=3;
            bulwers.step = 2048;
        }
    }



    if (core_cpu_load >= 80)
    {
        if(pics.bulwers <= 12 && bulwers.longev < 5)
        {
            pics.bulwers = 12;
            energy -=3;
            bulwers.step = 256;
        }

        if(pics.bulwers <= 14 && bulwers.longev >= 5)
        {
            pics.bulwers = 14;
            energy -=3;
            bulwers.step = 1024;
        }

        if(pics.bulwers <= 16 && bulwers.longev > 7)
        {
            pics.bulwers = 16;
            energy -=4;
            bulwers.step = 4096;
        }
    }



    if (core_cpu_load >= 90)
    {
        if(pics.bulwers <= 14 && bulwers.longev < 4)
        {
            pics.bulwers = 14;
            energy -=3;
            bulwers.step = 1024;
        }

        if(pics.bulwers <= 15 && bulwers.longev >= 4)
        {
            pics.bulwers = 15;
            energy -=3;
            bulwers.step = 2048;
        }

        if(pics.bulwers <= 16 && bulwers.longev > 6)
        {
            pics.bulwers = 16;
            energy -=4;
            bulwers.step = 4096;
        }
    }


    //----MEM----

    //----40

    if (core_memory >= 40)
    {
        if(pics.bulwers <= 4 && bulwers.longev < 9)
        {
            pics.bulwers = 4;
            bulwers.step = 1;
        }

        if(pics.bulwers <= 5 && bulwers.longev >= 9)
        {
            pics.bulwers = 5;
            bulwers.step = 2;
        }

        if(pics.bulwers <= 7 && bulwers.longev > 11)
        {
            pics.bulwers = 7;
            energy -=1;
            bulwers.step = 16;
        }
    }



    if (core_memory >= 50)
    {
        if(pics.bulwers <= 5 && bulwers.longev < 8)
        {
            pics.bulwers = 5;
            bulwers.step = 2;
        }

        if(pics.bulwers <= 6 && bulwers.longev >= 8)
        {
            pics.bulwers = 6;
            bulwers.step = 4;
        }

        if(pics.bulwers <= 8 && bulwers.longev > 10)
        {
            pics.bulwers = 8;
            energy -=1;
            bulwers.step = 16;
        }
    }



    if (core_memory >= 60)
    {
        if(pics.bulwers <= 6 && bulwers.longev < 7)
        {
            pics.bulwers = 6;
            energy -=1;
            bulwers.step = 4;
        }

        if(pics.bulwers <= 7 && bulwers.longev >= 7)
        {
            pics.bulwers = 7;
            energy -=1;
            bulwers.step = 8;
        }

        if(pics.bulwers <= 9 && bulwers.longev > 9)
        {
            pics.bulwers = 9;
            energy -=2;
            bulwers.step = 32;
        }
    }



    if (core_memory >= 70)
    {
        if(pics.bulwers <= 6 && bulwers.longev < 6)
        {
            pics.bulwers = 7;
            energy -=1;
            bulwers.step = 8;
        }

        if(pics.bulwers <= 8 && bulwers.longev >= 6)
        {
            pics.bulwers = 8;
            energy -=1;
            bulwers.step = 16;
        }

        if(pics.bulwers <= 10 && bulwers.longev > 8)
        {
            pics.bulwers = 10;
            energy -=2;
            bulwers.step = 64;
        }
    }



    if (core_memory >= 80)
    {
        if(pics.bulwers <= 8 && bulwers.longev < 5)
        {
            pics.bulwers = 8;
            energy -=2;
            bulwers.step = 16;
        }

        if(pics.bulwers <= 9 && bulwers.longev >= 5)
        {
            pics.bulwers = 9;
            energy -=2;
            bulwers.step = 32;
        }

        if(pics.bulwers <= 11 && bulwers.longev > 7)
        {
            pics.bulwers = 11;
            energy -=3;
            bulwers.step = 128;
        }
    }

    cout << "\033[13;46H" << pics.bulwers << " " << '\n';


    //----events checking

    if (core_battery_plugged == 0)
    {
        pics.outline = "to_shocked 2";
        print_event ("No battery !");
    }

    bulwers.happy = prev_happy;

    if (once_plugged)
    {
        if (core_battery_plugged == 2 && prev_bat_plug != 2)
        {
            if (pics.bulwers < 3)
            {
                pics.bulwers = 3;
            }

            if (prev_bat_plug == 3)
            {
                bulwers.happy--;
            }

            if (prev_bat_plug == 1)
            {
                bulwers.happy-=2;
            }
        }
        if (core_battery_plugged == 1 && prev_bat_plug != 1)
        {
            bulwers.happy+=2;
        }
        if (core_battery_plugged == 3 && prev_bat_plug != 3)
        {
            bulwers.happy--;
        }
    }
    if (!once_plugged)
    {
        if (core_battery_plugged == 1)
        {
            once_plugged = true;
            bulwers.happy +=2;
        }
        if (core_battery_plugged == 3)
        {
            once_plugged = true;
            bulwers.happy ++;
        }
    }
    prev_bat_plug = core_battery_plugged;
    cout << "\033[12;46H" << bulwers.happy << '\n';
    prev_happy = bulwers.happy;


    //---Calming

    static bool last_kill = false;
    static bool last_plask = false;
    static bool last_pet = false;
    static bool pet_success = false;

                //Event ev;
/*
    if ( evs->poll ( &ev ) and not event_now )
    {
        if ( ev.type == plask )
        {
            if (pics.bulwers <= 10)
                pics.bulwers++;
            bulwers.step += 20;
            pics.hot = 2;
            last_pet = false;
            last_kill = false;
            last_plask = true;
        }
        if (ev.type == pet)
        {

            if (pics.bulwers <= 10)
            {
                if (!last_pet && pics.bulwers <= 10)
                {
                    bulwers.happy++;
                    pet_success = true;
                    if (pics.bulwers <=6)
                        pics.outline = "to_happy 2";
                    else
                        pics.outline = "to_happy 1";
                }
            }

            if (pics.bulwers > 10)
            {
                bulwers.step += 5;
                pics.outline = "to_bulwers 16";
            }


            last_plask = false;
            last_kill = false;
            last_pet = true;
        }
        if (last_pet && ev.type != pet && pet_success)
            bulwers.happy--;
    }

    else
    {

     last_kill = false;
     last_plask = false;
     last_pet = false;
     pet_success = false;
*/

    //---bulwers state

    if (pics.bulwers == 4)
    {
        if ((core_memory < 40 && bulwers.longev < 9) && (core_cpu_load < 40 && bulwers.longev < 9))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers = 0;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 5)
    {
        if (((core_memory < 40 && bulwers.longev >=9) || (core_memory < 50 && bulwers.longev < 8)) && bulwers.longev < 10)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 1;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 6)
    {
        if (((core_cpu_load < 40 && bulwers.longev >= 9) || (core_cpu_load < 50 && bulwers.longev < 8)) && ((core_memory < 50 && bulwers.longev >= 8) || (core_memory < 60 && bulwers.longev < 7)) && bulwers.longev < 11)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 1;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 7)
    {
        if ((core_memory < 40 && bulwers.longev > 11) || (core_memory < 60 && bulwers.longev >= 7) || (core_memory < 70 && bulwers.longev < 6))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 2;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 8)
    {
        if (((core_cpu_load < 50 && bulwers.longev >= 8) || (core_cpu_load < 60 && bulwers.longev < 7)) && ((core_memory < 50 && bulwers.longev > 10) || (core_memory < 70 && bulwers.longev >= 6) || (core_memory < 80 && bulwers.longev < 5)))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 4;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 9)
    {
        if ((core_memory < 60 && bulwers.longev > 9) || (core_memory < 80 && bulwers.longev >= 5))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 8;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 10)
    {
        if (((core_cpu_load < 40 && bulwers.longev > 11) || (core_cpu_load < 60 && bulwers.longev >= 7) || (core_cpu_load < 70 && bulwers.longev < 6)) && (core_memory < 70 && bulwers.longev > 8) && bulwers.longev < 12)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 16;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 11)
    {
        if (core_memory < 80 && bulwers.longev > 7)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 32;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 12)
    {
        if ((core_cpu_load < 50 && bulwers.longev > 10) || (core_cpu_load < 70 && bulwers.longev >= 6) || (core_cpu_load < 80 && bulwers.longev < 5))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 64;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 13)
    {
        if (1)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 128;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 14)
    {
        if (((core_cpu_load < 60 && bulwers.longev > 9) || (core_cpu_load < 80 && bulwers.longev >= 5) || (core_cpu_load < 90 && bulwers.longev < 4)) && bulwers.longev < 13)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 256;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 15)
    {
        if ((core_cpu_load < 70 && bulwers.longev > 8) || (core_cpu_load < 90 && bulwers.longev >= 4))
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 512;
            }
            else
                bulwers.step--;
        }
    }
    if (pics.bulwers == 16)
    {
        if (((core_cpu_load < 80 && bulwers.longev > 7) || (core_cpu_load < 90 && bulwers.longev > 6)) && bulwers.longev < 14)
        {
            if (bulwers.step == 0)
            {
                pics.bulwers--;
                bulwers.step = 1024;
            }
            else
                bulwers.step--;
        }
    }



    //---long events



    if (bulwers.longev == 1)
    {
        if (core_battery > 2500 || core_battery_plugged == 3)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 2)
    {
        if (core_battery > 1500 || core_battery_plugged == 3)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 3)
    {
        if (core_time < 75600)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 4)
    {
        if (core_temperature < 56)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 5)
    {
        if (core_time < 82800)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 6)
    {
        if (core_temperature < 58)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 7)
    {
        if (core_uptime < 21600 && energy >= 25200)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 8)
    {
        if (core_temperature < 60)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 9)
    {
        if (core_uptime < 28800 && energy >= 14400)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 10)
    {
        if (core_battery > 500 || core_battery_plugged == 3)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 11)
    {
        if (core_uptime < 39600 && energy >= 7200 )
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 12)
    {
        if (core_temperature < 62)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 13)
    {
        if (core_temperature < 64)
        {
            bulwers.longev--;
        }
    }
    if (bulwers.longev == 14)
    {
        if (core_uptime < 50400 && energy >= 3600)
        {
            bulwers.longev--;
        }
    }

//---bulwers happyness modifications


    if (bulwers.happy == 1)
        if (bulwers.step >=1)
            bulwers.step--;

    if (bulwers.happy == 2)
        if (bulwers.step >=2)
            bulwers.step-=2;

    if (bulwers.happy == 3)
        if (bulwers.step >=5)
            bulwers.step-=5;

    if (bulwers.happy >= 4)
        if (bulwers.step >=10)
            bulwers.step -=10;


if (energy > 0)
    energy--;
}

//----END OF BULWERS MANAGING




//eye size

//---winter

/*
6<  22>
6-7 18-22
7-8 16-18
8-9 15-16
9-11 14-15
11-14
*/

if (core_month == 11 || core_month == 12 || core_month == 1 || core_month == 2)
{
    if (core_time < 6*3600 || core_time >= 22*3600)
        pics.eye = 1;
    if ((core_time >= 6*3600 && core_time < 7*3600) || (core_time >= 18*3600 && core_time < 22*3600))
        pics.eye = 2;
    if ((core_time >= 7*3600 && core_time < 8*3600) || (core_time >= 16*3600 && core_time < 18*3600))
        pics.eye = 3;
    if ((core_time >= 8*3600 && core_time < 9*3600) || (core_time >= 15*3600 && core_time < 16*3600))
        pics.eye = 4;
    if ((core_time >= 9*3600 && core_time < 11*3600) || (core_time >= 14*3600 && core_time < 15*3600))
        pics.eye = 5;
    if ((core_time >= 11*3600 && core_time < 14*3600))
        pics.eye = 6;
}



if (core_day != 7)
{
    if (core_time < 22800 || core_time > 75600 || energy < 18000)
        pics.tired = 1;
    if (core_time < 21600 || core_time > 79200 || energy < 10800)
        pics.tired = 2;
    if (core_time < 18000 || core_time > 82800 || energy < 3600 )
        pics.tired = 3;
    else
    {
        if (pics.tired > 0)
            pics.tired--;
    }
}
else
{
    if (core_time < 36000 || core_time > 82800 || energy < 18000)
    {
        pics.tired = 1;
        if (pics.bulwers <=6)
            pics.outline = "to_sleeppy 1";
    }
    if ((core_time < 28800 && core_time > 3600) || energy < 10800)
    {
        pics.tired = 2;
        if (pics.bulwers <=6)
            pics.outline = "to_sleeppy 2";
    }
    if ((core_time < 25200 && core_time > 10800) || energy < 3600 )
    {
        pics.tired = 3;
        if (pics.bulwers <=6)
            pics.outline = "to_sleeppy 3";
    }
    else
    {
        if (pics.tired > 0)
            pics.tired--;
    }
}
/*
56ºC
58ºC
60ºC
62ºC
64ºC
*/

static int temp_t = 0;

if (core_temperature >= 56 && core_temperature < 58)
{
    pics.hot = 1;
    if (temp_t < 30)
        temp_t = 30;
    if (temp_t > 60)
        get_flu = true;
}
if (core_temperature >= 58 && core_temperature < 60)
{
    pics.hot = 2;
    if (temp_t < 60)
        temp_t = 60;
    if (temp_t > 80)
        get_flu = true;
}
if (core_temperature >= 60 && core_temperature < 62)
{
    pics.hot = 3;
    if (temp_t < 80)
        temp_t = 80;
    if (temp_t > 120)
        get_flu = true;
}
if (core_temperature >= 62 && core_temperature < 64)
{
    pics.hot = 4;
    if (temp_t < 120)
        temp_t = 120;
}
if (core_temperature >= 64)
{
    pics.hot = 5;
    if (temp_t < 180)
        temp_t = 180;
}

else
{
    if (pics.hot > 0)
        pics.hot--;
}

if (temp_t > 0)
    temp_t--;

if (get_flu)
{
    print_event ("GET FLU!");

    static int flu_timer = 360;

    if (flu_timer > 0)
        flu_timer--;

    if (flu_timer <= 240)
    {
        pics.hot = 1;
        pics.shy = 1;
        pics.tired = 1;
        if (pics.bulwers < 8)
            pics.bulwers = 8;
    }
    if (flu_timer <= 120)
    {
        pics.hot = 2;
        pics.shy = 2;
        pics.tired = 2;
        if (pics.bulwers < 8)
            pics.bulwers = 8;
    }
    if (flu_timer == 0)
    {
        pics.hot = 3;
        pics.shy = 3;
        pics.tired = 3;
        if (pics.bulwers < 10)
            pics.bulwers = 10;
        energy--;
    }
}







//---sleepping

if (!wake_up)
{
    if (core_day != 7)
    {
        if (core_time >= 22800 )
        {
            energy = 54000;
            wake_up = true;
        }
        else
            energy = 43200;
    }
    else
    {

        if (core_time >= 36000 )
        {
            energy = 72000;
            wake_up = true;
        }
        else
            energy = 43200;
    }
    cout << "\033[13;17H" << "Wake up Neo!" << endl;
}

first_play = false;
return pics;
}




//Version 0.0.1a-01
//Chiliński Damian
//GBS
