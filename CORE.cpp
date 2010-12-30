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
#include <SDL/SDL.h>

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
    int happy;
    int state;
    int special;
    int longev;
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
int         intro       ();
bulwers_core bulwers_init ();


int main ()
{
    int f=0;
    int a;
    cout << "\033[2J\033[0;0H";
    print ( " Welcome in eyes project!\nPlease give number of stages: " );
    cin >> a;
    cout << "\033[2J";
    print_gui ();
    intro ();
    cout << "\033[1;33m";
    while (f<a){

        cout << "\033[0;22H" << f << " (" << (100*f)/a << "%)" << '\n';
        bulwers_init ();

f++;
sleep (3);
}
    cout << "\033[0m \033[2J \033[0;0H";
}

//===========================

static void print ( char * str )
{
  print ( str, 0 );
}

static void print ( char * str, int pos )
{
  cout << "\033[0m \033[1;32m";
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
  cout  << "\033[2J \033[0m \033[0;0H";
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

    get_date.day_num = l_date[2].toInt ();
    QStringList tmp2 ( l_date[3].split ( ':' ) );
    get_date.hour = ( 3600 * ( tmp2[0].toInt () ) + ( 60 * ( tmp2[1].toInt() ) ) + tmp2[2].toInt () );
    get_date.year = l_date[4].toInt ();

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

int intro()
{
    cout << "\033[37m" << "\033[40m";
    int i= 95;/*
    cout << "\033[37m";
    cout << "\033[92A";
    SDL_Delay (2000);
    cout << "\033[1;30m";
    cout << "  _______________  ___       ___    _______________   ____________" << endl
            << " /              //|   ||    |   || /              // /            \\\\" << endl
            << "|     _________// |   \\\\    /   |||     _________// |     _____    ||" << endl
            << "|    ||_______     \\   \\\\__/   // |    ||_______    |    //____\\___||" << endl
            << "|            ||     \\         //  |            ||   |             \\\\" << endl
            << "|      ______||      \\_     _//   |      ______||    \\_______      ||" << endl
            << "|    ||_______         |   ||     |    ||________   |   \\\\___\\     ||" << endl
            << "|             \\\\       |   ||     |             \\\\  |              ||" << endl
            << " \\_____________\\\\      |___||      \\_____________\\\\  \\____________//" << endl;
    SDL_Delay (100);
    cout << "\033[37m" << "\033[9A";
    cout << "  _______________  ___       ___    _______________   ____________" << endl
         << " /              //|   ||    |   || /              // /            \\\\" << endl
         << "|     _________// |   \\\\    /   |||     _________// |     _____    ||" << endl
         << "|    ||_______     \\   \\\\__/   // |    ||_______    |    //____\\___||" << endl
         << "|            ||     \\         //  |            ||   |             \\\\" << endl
         << "|      ______||      \\_     _//   |      ______||    \\_______      ||" << endl
         << "|    ||_______         |   ||     |    ||________   |   \\\\___\\     ||" << endl
         << "|             \\\\       |   ||     |             \\\\  |              ||" << endl
         << " \\_____________\\\\      |___||      \\_____________\\\\  \\____________//" << endl;
    cout << endl << endl << "Eyes project by:" << endl;
    SDL_Delay (500);
    cout << "Phoenix |Medrzycki Krzysztof|" << endl;
    SDL_Delay (200);
    cout << "Lapsio  |Chilinski Damian   |" << endl << endl;
    SDL_Delay (500);
    cout << "GBS 2010" << endl;
    SDL_Delay (200);
    cout << "www.eyesproject.pl" << endl << endl;
    cout << "\033[1;31m" << "#####################################################################" << endl << endl;
    SDL_Delay (100);
    cout << "#####################################################################" << endl << endl << endl << endl;
    cout << "\033[1;33m";
    SDL_Delay (5000);*/
return 1;
}

/*

Welcome to Eyes project!

  _______________  ___       ___    _______________   ____________
 /              //|   ||    |   || /              // /            \\
|     _________// |   \\    /   |||     _________// |     _____    ||
|    ||_______     \   \\__/   // |    ||_______    |    //____\___||
|            ||     \         //  |            ||   |             \\
|      ______||      \_     _//   |      ______||    \_______      ||
|    ||_______         |   ||     |    ||________   |   \\___\     ||
|             \\       |   ||     |             \\  |              ||
 \_____________\\      |___||      \_____________\\  \____________//


Eyes project by:
Phoenix |Medrzycki Krzysztof|
Lapsio  |Chilinski Damian   |

GBS 2010
www.eyesproject.pl

#####################################################################

#####################################################################



stage: 18 (90%)
First sector

current cpu probe: 9

cpu probes table:

22
28
22
26
35
28
19
31
28
23

cpu filling buffer... DONE!


Glibtop values:

cpu: 26%
memory: 3%
proclist: 195
uptime: 420 seconds
that is:
0 hours and 7minutes
Time values:

day of week: 3
day of month: 29
month: 12
year: 2010
time: 70504 seconds
that is:
19:35:4

Other values

battery plugged state: 4
temperature: 46ºC
batery power4608MAh
energy: 54000 units (100%)
once plugged: 0
prev battery plug: 4

end of first sector

bulwers.special: 0
bulwers.longev before checking: 0
bulwers.longev: 0
bulwers before cpu: 0
bulwers after cpu (before memory): 0
bulwers after memory: 0
happy before checking: 0
once plugged = 0
happy: 0



###################
Version 0.0.1a-01
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

 *FUCK YEAH*
 NIEpospolicie!

 */
//----------------------------------------------------------------------

int pos;

void print_event ( char * ev_text )
{
  cout << "\033[" << 10+pos << ";66H" << ev_text << '\n';
  pos++;
}

//----------------------------------------

bulwers_core bulwers_init()
{
    //----create bulwers structure

    bulwers_core bulwers;

    //----initializing values

    static bool first_play = true;
    if (first_play)
    {
        bulwers.happy = 0;
        bulwers.longev = 0;
        bulwers.special = 0;
        bulwers.state = 0;
    }
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
    cpu_probes[current_probe] = C_LOAD ();
    if (first_play) cpu_probes[current_probe] = 20;
    current_probe++;
    if (current_probe == 10) current_probe = 0;
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
    first_play = false;
    static bool bajer = true;

    //----first sector

    cout << "\033[2;22H" << current_probe << "\033[1;55H"
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
    if (cpu_probes[9] == 0)
    {
    }
    
    if (cpu_probes[9] == 0)
    {
    }
    
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
         << "\033[11;17H" << core_battery << "MAh"
         << "\033[12;17H" << core_temperature << "ºC"
         << "\033[13;17H" << energy << " units (" << (100*energy)/54000 << "%)"
         << '\n';

    //--Proggramer Manual
    /*
     Bulwers manual:

       Bulwers classes:

        bulwers.special - runs special ocasion mode
        bulwers.state - true bulwers status eg. bulwers = 0 - everything is OK.
        bulwers.longev - long events appears on every bulwers - eg. low battery or high temperature.
        bulwers.happy - happyness modificator it has appear on some types of non-bad bulwers eg. 0-3



        bulwers.special list:
        0 - nothing special
        1 - start of school term
        2 - nicolas day
        3 - christmas
        4 - new year
        5 - wielkanoc
        6 - end of term

        bulwers.state list:
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

    */

    //eves checking

    if (core_day == 13)
    {
        //something stupid
    }
    if (core_month == 9)
    {
        if (core_day == 1)
        {
            bulwers.special = 1;
        }
    }
    if (core_month == 6)
    {
        if (core_day == 14)
        {
            bulwers.special = 6;
        }
    }
    if (core_month == 12)
    {
        if (core_day == 24)
        {
            bulwers.special = 3;
        }
        if (core_day == 31)
        {
            bulwers.special = 4;
        }
    }


    //--space for others events

    cout << "\033[10;46H" << bulwers.special;

    pos = 0;
    //--long events checking

    if (core_battery <= 2500 && bulwers.longev < 1 && core_battery_plugged == 4)
    {
        bulwers.longev = 1;
        print_event ( "ev1 (battery <=2500 MAh)" );
    }

    if (core_battery <= 1500 && bulwers.longev < 2 && core_battery_plugged == 4)
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
    if (core_battery <= 500 && bulwers.longev < 10 && core_battery_plugged == 4)
    {
        bulwers.longev = 10;

        if (bulwers.state < 5)
            bulwers.state = 5;

        print_event ( "ev10 (battery critical low)" );
    }
    if ((core_uptime >= 39600 || energy < 7200 )&& bulwers.longev < 11)
    {
        bulwers.longev = 11;

        if (bulwers.state < 6)
            bulwers.state = 6;

        print_event ( "ev11 (they've just slept...)" );
    }
    if (core_temperature >= 62)
    {
        energy -=3;

        if (bulwers.longev < 12)
            bulwers.longev = 12;

        if (bulwers.state < 10)
            bulwers.state = 10;

        print_event ( "ev12 (temperature goes up to 62ºC)" );
    }
    if (core_temperature >= 64)
    {
        energy -=4;

        if (bulwers.longev < 13)
            bulwers.longev = 13;

        if (bulwers.state < 14)
            bulwers.state = 14;

        print_event ( "ev13 (temperature goes up to 64ºC)" );
    }
    if ((core_uptime >= 50400 || energy < 3600) && bulwers.longev < 14)
    {
        bulwers.longev = 14;

        if (bulwers.state < 16)
            bulwers.state = 16;

        print_event ( "ev14 (they cant' stand)" );
    }
    if (energy <= 0)
    {
        bulwers.state = 17;
        print_event ( "idiot... <.:zzZ:.>" );
    }
    //if (core_battery <= 2500 && bulwers.longev < 1) bulwers.longev = 1;
    //if (core_battery <= 2500 && bulwers.longev < 2) bulwers.longev = 2;

    cout << "\033[11;46H" << bulwers.longev << '\n';


    //--loads checking


    //----CPU----

    //----40

    if (core_cpu_load >= 40)
    {
        if(bulwers.state < 6 && bulwers.longev < 9)
        {
            bulwers.state = 6;
            energy -=1;
        }

        if(bulwers.state < 8 && bulwers.longev >= 9)
        {
            bulwers.state = 8;
            energy -=1;
        }

        if(bulwers.state < 12 && bulwers.longev < 11)
        {
            bulwers.state = 12;
            energy -=2;
        }
    }



    if (core_cpu_load >= 50)
    {
        if (bulwers.state < 8 && bulwers.longev < 8)
        {
            bulwers.state = 8;
            energy -=1;
        }

        if (bulwers.state < 9 && bulwers.longev >= 8)
        {
            bulwers.state = 9;
            energy -=1;
        }

        if (bulwers.state < 13 && bulwers.longev < 10)
        {
            bulwers.state = 13;
            energy -=2;
        }
    }



    if (core_cpu_load >= 60)
    {
        if (bulwers.state < 10 && bulwers.longev < 7)
        {
            bulwers.state = 10;
            energy -=2;
        }

        if (bulwers.state < 11 && bulwers.longev >= 7)
        {
            bulwers.state = 11;
            energy -=2;
        }

        if (bulwers.state < 14 && bulwers.longev < 9)
        {
            bulwers.state = 14;
            energy -=3;
        }
    }



    if (core_cpu_load >= 70)
    {
        if(bulwers.state < 12 && bulwers.longev < 6)
        {
            bulwers.state = 12;
            energy -=2;
        }

        if(bulwers.state < 14 && bulwers.longev >= 6)
        {
            bulwers.state = 14;
            energy -=2;
        }

        if(bulwers.state < 15 && bulwers.longev < 8)
        {
            bulwers.state = 15;
            energy -=3;
        }
    }



    if (core_cpu_load >= 80)
    {
        if(bulwers.state < 14 && bulwers.longev < 5)
        {
            bulwers.state = 14;
            energy -=3;
        }

        if(bulwers.state < 15 && bulwers.longev >= 5)
        {
            bulwers.state = 15;
            energy -=3;
        }

        if(bulwers.state < 16 && bulwers.longev < 7)
        {
            bulwers.state = 16;
            energy -=4;
        }
    }



    if (core_cpu_load >= 90)
    {
        if(bulwers.state < 15 && bulwers.longev < 4)
        {
            bulwers.state = 15;
            energy -=3;
        }

        if(bulwers.state < 16 && bulwers.longev >= 4)
        {
            bulwers.state = 16;
            energy -=3;
        }

        if(bulwers.state < 16 && bulwers.longev < 6)
        {
            bulwers.state = 16;
            energy -=4;
        }
    }


    //----MEM----

    //----40

    if (core_memory >= 40)
    {
        if(bulwers.state < 4 && bulwers.longev < 9)
        {
            bulwers.state = 4;
        }

        if(bulwers.state < 5 && bulwers.longev >= 9)
        {
            bulwers.state = 5;
        }

        if(bulwers.state < 7 && bulwers.longev < 11)
        {
            bulwers.state = 7;
            energy -=1;
        }
    }



    if (core_memory >= 50)
    {
        if(bulwers.state < 5 && bulwers.longev < 8)
        {
            bulwers.state = 5;
        }

        if(bulwers.state < 6 && bulwers.longev >= 8)
        {
            bulwers.state = 6;
        }

        if(bulwers.state < 8 && bulwers.longev < 10)
        {
            bulwers.state = 8;
            energy -=1;
        }
    }



    if (core_memory >= 60)
    {
        if(bulwers.state < 6 && bulwers.longev < 7)
        {
            bulwers.state = 6;
            energy -=1;
        }

        if(bulwers.state < 7 && bulwers.longev >= 7)
        {
            bulwers.state = 7;
            energy -=1;
        }

        if(bulwers.state < 9 && bulwers.longev < 9)
        {
            bulwers.state = 9;
            energy -=2;
        }
    }



    if (core_memory >= 70)
    {
        if(bulwers.state < 6 && bulwers.longev < 6)
        {
            bulwers.state = 7;
            energy -=1;
        }

        if(bulwers.state < 8 && bulwers.longev >= 6)
        {
            bulwers.state = 8;
            energy -=1;
        }

        if(bulwers.state < 10 && bulwers.longev < 8)
        {
            bulwers.state = 10;
            energy -=2;
        }
    }



    if (core_memory >= 80)
    {
        if(bulwers.state < 8 && bulwers.longev < 5)
        {
            bulwers.state = 8;
            energy -=2;
        }

        if(bulwers.state < 9 && bulwers.longev >= 5)
        {
            bulwers.state = 9;
            energy -=2;
        }

        if(bulwers.state < 11 && bulwers.longev < 7)
        {
            bulwers.state = 11;
            energy -=3;
        }
    }

    cout << "\033[13;46H" << bulwers.state << '\n';


    //----events checking


    if (once_plugged)
    {
//        cout << "once plugged = 1" << endl;
        if (core_battery_plugged == 2 && prev_bat_plug != 2)
        {
            if (bulwers.state < 3)
            {
                bulwers.state = 3;
  //              cout << "surpriced" << endl;
            }

            if (prev_bat_plug == 3)
            {
                bulwers.happy--;
    //            cout << "prev bat plug = 3" << endl;
            }

            if (prev_bat_plug == 1)
            {
                bulwers.happy-=2;
      //          cout << "prev bat plug = 1" << endl;
            }
        }
        if (core_battery_plugged == 1 && prev_bat_plug != 1)
        {
            bulwers.happy+=2;
        //    cout << "battery has been just plugged, happy +2" << endl;
        }
        if (core_battery_plugged == 3 && prev_bat_plug != 3)
        {
            bulwers.happy--;
        }
    }
    if (!once_plugged)
    {
        //cout << "once plugged = 0" << endl;
        if (core_battery_plugged == 1)
        {
            once_plugged = true;
            bulwers.happy +=2;
//            cout << "plugged = 1" << endl;
        }
        if (core_battery_plugged == 3)
        {
            once_plugged = true;
            bulwers.happy ++;
  //          cout << "pluged = 3" << endl;
        }
    }
    prev_bat_plug = core_battery_plugged;
    cout << "\033[12;46H" << bulwers.happy << '\n';

return bulwers;
}


//Version 0.0.1a-01
//Chiliński Damian
//GBS
