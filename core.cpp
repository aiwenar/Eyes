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

sdate       get_time        ();
static void print           ( char * str );
static void print           ( char * str, int _pos );
static void print_gui       ();
int         temperatura     ();
int         bat_plugged     ();
int         bateria         ();
int         C_LOAD          ();
int         M_LOAD          ();
int         P_LIST          ();
int         U_TIME          ();
int         about           ();
int         naglowek_in     (char* version);
int         naglowek_out    (char* version);
int         felltext        (char*downgrade);
int         pulsetext       (char*text, int delay, int repeat, int _position);
void        bulwers_init    ();
void        initialization  ( core_stats * input );
void        reload_stats    ( core_stats * input );
void        cleanup         ();
bool        identical       ( pict_layers, pict_layers);

static bool event_now,
            get_flu,
            wake_up;
static int core_step;
int _pos;

//------------------

bulwers_core * bulwers;

void bulwers_init ()
{
    bulwers = new bulwers_core;
    bulwers->happy = 0;
    bulwers->longev = 0;
    bulwers->special = 0;
    bulwers->step = 0;
    bulwers->level = 0;
}

void cleanup ()
{
    // tu musisz dorobić usuwanie zmiennych itp
}

void print_event ( char * ev_text )
{
    cout << "\033[" << 10+_pos << ";66H" << ev_text << '\n';
    _pos++;
}

void eyes_view::update_bulwers ( core_stats * input )
{
    pict_layers pics;
    pict_layers prev_pics;
    //static int input->current_probe = 0;
        //input->cpu_probes[input->current_probe] = C_LOAD ();

    //input->current_probe++;



    /*input->core_cpu_load = (input->cpu_probes[0] +
                         input->cpu_probes[1] +
                         input->cpu_probes[2] +
                         input->cpu_probes[3] +
                         input->cpu_probes[4] +
                         input->cpu_probes[5] +
                         input->cpu_probes[6] +
                         input->cpu_probes[7] +
                         input->cpu_probes[8] +
                         input->cpu_probes[9]) / 10;*/
    //int input->core_memory = M_LOAD ();
    //int input->core_proclist = P_LIST ();
    //int input->core_uptime = U_TIME ();
    //static int input->energy = 54000;
    //static bool input->once_plugged = false;
    //static int input->prev_bat_plug = input->core_battery_plugged;


    // updacenie debuga

    cout << "\033[2;22H" << input->current_probe+1 << " " << "\033[1;55H"
         << input->cpu_probes [0] << ' '
         << input->cpu_probes [1] << ' '
         << input->cpu_probes [2] << ' '
         << input->cpu_probes [3] << ' '
         << input->cpu_probes [4] << ' '
         << input->cpu_probes [5] << ' '
         << input->cpu_probes [6] << ' '
         << input->cpu_probes [7] << ' '
         << input->cpu_probes [8] << ' '
         << input->cpu_probes [9] << '\n';
    cout << "\033[3;22H" << input->current_probe_small+1 << " " << "\033[2;55H"
         << input->cpu_sector_small [0] << ' '
         << input->cpu_sector_small [1] << ' '
         << input->cpu_sector_small [2] << ' '
         << input->cpu_sector_small [3] << ' '
         << input->cpu_sector_small [4] << ' '
         << input->cpu_sector_small [5] << ' '
         << input->cpu_sector_small [6] << ' '
         << input->cpu_sector_small [7] << ' '
         << input->cpu_sector_small [8] << ' '
         << input->cpu_sector_small [9] << '\n';
    cout << "\033[3;53H" << input->core_cpu_load;
    cout << "\033[4;53H" << input->core_memory << "%"
         << "\033[5;53H" << input->core_proclist
         << "\033[6;53H" << input->core_uptime << " seconds" //<< "that is: " << input->core_uptime/3600 << " hours" << " and " << (input->core_uptime/60) - ((input->core_uptime/3600)*60) << "minutes"
         << "\033[4;92H" << input->core_day
         << "\033[5;92H" << input->core_dnum
         << "\033[6;92H" << input->core_month
         << "\033[7;92H" << input->core_year
         << "\033[8;92H" << input->core_time << " seconds that is:" << input->core_time/3600 << ":" << (input->core_time/60) - ((input->core_time/3600)*60) << ":" << input->core_time - ((input->core_time/60)*60)
         << "\033[10;17H" << ( input->core_battery_plugged == 1 ? "just pluged" : ( input->core_battery_plugged == 2 ? "just unpluged" : ( input->core_battery_plugged == 3 ? "pluged      " : "unpluged     " ) ) )
         << "\033[11;17H" << input->core_battery << "MAh   "
         << "\033[12;17H" << input->core_temperature << "ºC"
         << '\n';

    //--Proggramer Manual
    /*
     Bulwers manual:

       Bulwers classes:

        bulwers->special - runs special ocasion mode
        bulwers->level - true bulwers status eg. bulwers = 0 - everything is OK.
        bulwers->longev - long events appears on every bulwers - eg. low battery or high temperature.
        bulwers->happy - happyness modificator it has appear on some types of non-bad bulwers eg. 0-3



        bulwers->special list:
        0 - nothing special
        1 - weekand!
        2 - friday thirteenth !
        3 - start of school term
        4 - nicolas day
        5 - christmas
        6 - new year
        7 - wielkanoc
        8 - end of term

        bulwers->level list:
        0 - everything is OK
        1 - something goes wrong - speed looking
        2 - something goes wrong - shocked
        3 - something happend - surpriced looking
        4+ - hardware evenst eg. cpu load to high, or mem load(experimental), or longev appears

        bulwers->longev list: (in priority order)
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

    if (input->core_day == 7)
    {
        bulwers->special = 1;
    }
    if (input->core_day == 5)
    {
        if (input->core_dnum == 13)
        {
            bulwers->special = 2;
        }
    }
    if (input->core_month == 9)
    {
        if (input->core_day == 1)
        {
            bulwers->special = 3;
        }
    }
    if (input->core_month == 6)
    {
        if (input->core_day == 14)
        {
            bulwers->special = 8;
        }
    }
    if (input->core_month == 12)
    {
        if (input->core_day == 24)
        {
            bulwers->special = 5;
        }
        if (input->core_day == 31)
        {
            bulwers->special = 6;
        }
    }


    //--space for others events

    cout << "\033[10;46H" << bulwers->special << " ";

    _pos = 0;

    //--filling input->energy status, Wake up Neo!


if (wake_up)
{
    if (input->core_day != 7)
    {
        if (input->energy >= 10000)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/54000 << "%)  " << endl;
        if (input->energy < 10000 && input->energy >= 1000)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/54000 << "%)   " << endl;
        if (input->energy < 1000 && input->energy >= 100)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/54000 << "%)    " << endl;
        if (input->energy < 100 && input->energy >= 10)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/54000 << "%)     " << endl;
        if (input->energy < 10)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/54000 << "%)      " << endl;
    }

    else
    {
        if (input->energy >= 10000)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/72000 << "%)  " << endl;
        if (input->energy < 10000 && input->energy >= 1000)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/72000 << "%)   " << endl;
        if (input->energy < 1000 && input->energy >= 100)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/72000 << "%)    " << endl;
        if (input->energy < 100 && input->energy >= 10)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/72000 << "%)     " << endl;
        if (input->energy < 10)
            cout << "\033[13;17H" << input->energy << " units (" << (100*input->energy)/72000 << "%)      " << endl;
    }


    //--long events checking


    //static int input->battery_buffer = 0;
    if (input->core_battery_plugged == 2) input->battery_buffer = 0;
    if (input->core_battery_plugged == 4 && input->battery_buffer < 15) input->battery_buffer++;

    if (input->core_battery <= 2500 && bulwers->longev < 1 && input->core_battery_plugged == 4 && input->battery_buffer == 15)
    {
        bulwers->longev = 1;
        print_event ( "ev1 (battery <=2500 MAh)" );
    }

    if (input->core_battery <= 1500 && bulwers->longev < 2 && input->core_battery_plugged == 4 && input->battery_buffer == 15)
    {
        bulwers->longev = 2;
        print_event ( "ev2 (battery <= 1500 MAh)" );
    }

    if (input->core_time >= 75600 && bulwers->longev < 3)
    {
        bulwers->longev = 3;
        print_event ( "ev3 (it's late)" );
    }

    if (input->core_temperature >= 56 && bulwers->longev < 4)
    {
        bulwers->longev = 4;
        print_event ( "ev4 (temperature goes up to 56ºC)" );
    }

    if (input->core_time >= 82800 && bulwers->longev < 5)
    {
        bulwers->longev = 5;
        print_event ( "ev5 (it's very late)" );
    }


    if ((input->core_uptime >= 36000 || input->energy < 25200) && bulwers->longev < 6)
    {
        bulwers->longev = 6;
        print_event ( "ev6 (they're tired)" );
    }

    if (input->core_temperature >= 58)
    {
        input->energy -=1;

        if(bulwers->longev < 7)
            bulwers->longev = 7;

        print_event ( "ev7 (temperature goes up to 58ºC)" );
    }

    if (input->core_temperature >= 60)
    {
        input->energy -=2;

        if (bulwers->longev < 8)
            bulwers->longev = 8;

        print_event ( "ev8 (temperature goes up to 60ºC)" );
    }

    if ((input->core_uptime >= 28800 || input->energy < 14400) && bulwers->longev < 9)
    {
        bulwers->longev = 9;
        print_event ( "ev9 (they're very tired)" );
    }
    if (input->core_battery <= 500 && bulwers->longev < 10 && input->core_battery_plugged == 4 && input->battery_buffer == 15)
    {
        bulwers->longev = 10;

        if (bulwers->level < 5)
            bulwers->level = 5;

        print_event ( "ev10 (battery critical low)" );
    }
    if ((input->core_uptime >= 39600 || input->energy < 7200 )&& bulwers->longev < 11)
    {
        bulwers->longev = 11;

        if (bulwers->level < 6)
            bulwers->level = 6;

        print_event ( "ev11 (they've just slept...)" );
    }
    if (input->core_temperature >= 62)
    {
        input->energy -=3;

        if (bulwers->longev < 12)
            bulwers->longev = 12;

        if (bulwers->level < 10)
            bulwers->level = 10;

        print_event ( "ev12 (temperature goes up to 62ºC)" );
    }
    if (input->core_temperature >= 64)
    {
        input->energy -=4;

        if (bulwers->longev < 13)
            bulwers->longev = 13;

        if (bulwers->level < 14)
            bulwers->level = 14;

        print_event ( "ev13 (temperature goes up to 64ºC)" );
    }
    if ((input->core_uptime >= 50400 || input->energy < 3600) && bulwers->longev < 14)
    {
        bulwers->longev = 14;

        if (bulwers->level < 16)
            bulwers->level = 16;

        print_event ( "ev14 (they cant' stand)" );
    }
    if (input->energy <= 0)
    {
        bulwers->level = 17;
        print_event ( "idiot... <.:zzZ:.>" );
    }




    //if (input->core_battery <= 2500 && bulwers->longev < 1) bulwers->longev = 1;
    //if (input->core_battery <= 2500 && bulwers->longev < 2) bulwers->longev = 2;

    cout << "\033[11;46H" << bulwers->longev << " " << '\n';


    //--loads checking


    //----CPU----

    //----40

    if (input->core_cpu_load >= 40)
    {
        if(bulwers->level <= 4 && bulwers->longev < 9)
        {
            bulwers->level = 4;
            input->energy -=1;
            bulwers->step = 8;
        }

        if(bulwers->level <= 6 && bulwers->longev >= 9)
        {
            bulwers->level = 6;
            input->energy -=1;
            bulwers->step = 21;
        }

        if(bulwers->level <= 10 && bulwers->longev > 11)
        {
            bulwers->level = 10;
            input->energy -=2;
            bulwers->step = 144;
        }
    }



    if (input->core_cpu_load >= 50)
    {
        if (bulwers->level <= 6 && bulwers->longev < 8)
        {
            bulwers->level = 6;
            input->energy -=1;
            bulwers->step = 21;
        }

        if (bulwers->level <= 8 && bulwers->longev >= 8)
        {
            bulwers->level = 8;
            input->energy -=1;
            bulwers->step = 55;
        }

        if (bulwers->level <= 12 && bulwers->longev > 10)
        {
            bulwers->level = 12;
            input->energy -=2;
            bulwers->step = 377;
        }
    }



    if (input->core_cpu_load >= 60)
    {
        if (bulwers->level <= 8 && bulwers->longev < 7)
        {
            bulwers->level = 8;
            input->energy -=2;
            bulwers->step = 55;
        }

        if (bulwers->level <= 10 && bulwers->longev >= 7)
        {
            bulwers->level = 10;
            input->energy -=2;
            bulwers->step = 144;
        }

        if (bulwers->level <= 14 && bulwers->longev > 9)
        {
            bulwers->level = 14;
            input->energy -=3;
            bulwers->step = 987;
        }
    }



    if (input->core_cpu_load >= 70)
    {
        if(bulwers->level <= 10 && bulwers->longev < 6)
        {
            bulwers->level = 10;
            input->energy -=2;
            bulwers->step = 144;
        }

        if(bulwers->level <= 12 && bulwers->longev >= 6)
        {
            bulwers->level = 12;
            input->energy -=2;
            bulwers->step = 377;
        }

        if(bulwers->level <= 15 && bulwers->longev > 8)
        {
            bulwers->level = 15;
            input->energy -=3;
            bulwers->step = 1597;
        }
    }



    if (input->core_cpu_load >= 80)
    {
        if(bulwers->level <= 12 && bulwers->longev < 5)
        {
            bulwers->level = 12;
            input->energy -=3;
            bulwers->step = 377;
        }

        if(bulwers->level <= 14 && bulwers->longev >= 5)
        {
            bulwers->level = 14;
            input->energy -=3;
            bulwers->step = 987;
        }

        if(bulwers->level <= 16 && bulwers->longev > 7)
        {
            bulwers->level = 16;
            input->energy -=4;
            bulwers->step = 2584;
        }
    }



    if (input->core_cpu_load >= 90)
    {
        if(bulwers->level <= 14 && bulwers->longev < 4)
        {
            bulwers->level = 14;
            input->energy -=3;
            bulwers->step = 1024;
        }

        if(bulwers->level <= 15 && bulwers->longev >= 4)
        {
            bulwers->level = 15;
            input->energy -=3;
            bulwers->step = 2048;
        }

        if(bulwers->level <= 16 && bulwers->longev > 6)
        {
            bulwers->level = 16;
            input->energy -=4;
            bulwers->step = 4096;
        }
    }


    //----MEM----

    //----40

    if (input->core_memory >= 40)
    {
        if(bulwers->level <= 4 && bulwers->longev < 9)
        {
            bulwers->level = 4;
            bulwers->step = 8;
        }

        if(bulwers->level <= 5 && bulwers->longev >= 9)
        {
            bulwers->level = 5;
            bulwers->step = 13;
        }

        if(bulwers->level <= 7 && bulwers->longev > 11)
        {
            bulwers->level = 7;
            input->energy -=1;
            bulwers->step = 34;
        }
    }



    if (input->core_memory >= 50)
    {
        if(bulwers->level <= 5 && bulwers->longev < 8)
        {
            bulwers->level = 5;
            bulwers->step = 13;
        }

        if(bulwers->level <= 6 && bulwers->longev >= 8)
        {
            bulwers->level = 6;
            bulwers->step = 21;
        }

        if(bulwers->level <= 8 && bulwers->longev > 10)
        {
            bulwers->level = 8;
            input->energy -=1;
            bulwers->step = 55;
        }
    }



    if (input->core_memory >= 60)
    {
        if(bulwers->level <= 6 && bulwers->longev < 7)
        {
            bulwers->level = 6;
            input->energy -=1;
            bulwers->step = 21;
        }

        if(bulwers->level <= 7 && bulwers->longev >= 7)
        {
            bulwers->level = 7;
            input->energy -=1;
            bulwers->step = 34;
        }

        if(bulwers->level <= 9 && bulwers->longev > 9)
        {
            bulwers->level = 9;
            input->energy -=2;
            bulwers->step = 89;
        }
    }



    if (input->core_memory >= 70)
    {
        if(bulwers->level <= 6 && bulwers->longev < 6)
        {
            bulwers->level = 7;
            input->energy -=1;
            bulwers->step = 34;
        }

        if(bulwers->level <= 8 && bulwers->longev >= 6)
        {
            bulwers->level = 8;
            input->energy -=1;
            bulwers->step = 55;
        }

        if(bulwers->level <= 10 && bulwers->longev > 8)
        {
            bulwers->level = 10;
            input->energy -=2;
            bulwers->step = 144;
        }
    }



    if (input->core_memory >= 80)
    {
        if(bulwers->level <= 8 && bulwers->longev < 5)
        {
            bulwers->level = 8;
            input->energy -=2;
            bulwers->step = 55;
        }

        if(bulwers->level <= 9 && bulwers->longev >= 5)
        {
            bulwers->level = 9;
            input->energy -=2;
            bulwers->step = 89;
        }

        if(bulwers->level <= 11 && bulwers->longev > 7)
        {
            bulwers->level = 11;
            input->energy -=3;
            bulwers->step = 233;
        }
    }

    cout << "\033[13;46H" << bulwers->level << " " << '\n';


    //----events checking

    if (input->core_battery_plugged == 0)
    {
        pics.outline = 20;
        pics.eye = 10;
        print_event ("No battery !");
    }
    else if (pics.outline == 20)
        pics.outline = 0;

    bulwers->happy = input->prev_happy;

    if (input->once_plugged)
    {
        if (input->core_battery_plugged == 2 && input->prev_bat_plug != 2)
        {
            if (bulwers->level < 3)
            {
                bulwers->level = 3;
            }

            if (input->prev_bat_plug == 3)
            {
                bulwers->happy--;
            }

            if (input->prev_bat_plug == 1)
            {
                bulwers->happy-=2;
            }
        }
        if (input->core_battery_plugged == 1 && input->prev_bat_plug != 1)
        {
            bulwers->happy+=2;
        }
        if (input->core_battery_plugged == 3 && input->prev_bat_plug != 3)
        {
            bulwers->happy--;
        }
    }
    if (!input->once_plugged)
    {
        if (input->core_battery_plugged == 1)
        {
            input->once_plugged = true;
            bulwers->happy +=2;
        }
        if (input->core_battery_plugged == 3)
        {
            input->once_plugged = true;
            bulwers->happy ++;
        }
    }
    input->prev_bat_plug = input->core_battery_plugged;
    cout << "\033[12;46H" << bulwers->happy << '\n';
    input->prev_happy = bulwers->happy;


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
            if (bulwers->level <= 10)
                bulwers->level++;
            bulwers->step += 20;
            pics.hot = 2;
            last_pet = false;
            last_kill = false;
            last_plask = true;
        }
        if (ev.type == pet)
        {

            if (bulwers->level <= 10)
            {
                if (!last_pet && bulwers->level <= 10)
                {
                    bulwers->happy++;
                    pet_success = true;
                    if (bulwers->level <=6)
                        pics.outline = "to_happy 2";
                    else
                        pics.outline = "to_happy 1";
                }
            }

            if (bulwers->level > 10)
            {
                bulwers->step += 5;
                pics.outline = "to_bulwers 16";
            }


            last_plask = false;
            last_kill = false;
            last_pet = true;
        }
        if (last_pet && ev.type != pet && pet_success)
            bulwers->happy--;
    }

    else
    {

     last_kill = false;
     last_plask = false;
     last_pet = false;
     pet_success = false;
*/

    //---bulwers state
    if (bulwers->level == 1)
    {
        if (bulwers->step == 0)
        {
            bulwers->level = 0;
        }
        else
            bulwers->step--;
    }
    if (bulwers->level == 2)
    {
        if (bulwers->step == 0)
        {
            bulwers->level--;
            bulwers->step = 2;
        }
        else
            bulwers->step--;
    }
    if (bulwers->level == 3)
    {
        if (bulwers->step == 0)
        {
            bulwers->level--;
            bulwers->step = 3;
        }
        else
            bulwers->step--;
    }
    if (bulwers->level == 4)
    {
        if ((input->core_memory < 40 && bulwers->longev < 9) && (input->core_cpu_load < 40 && bulwers->longev < 9))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 5;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 5)
    {
        if (((input->core_memory < 40 && bulwers->longev >=9) || (input->core_memory < 50 && bulwers->longev < 8)) && bulwers->longev < 10)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 8;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 6)
    {
        if (((input->core_cpu_load < 40 && bulwers->longev >= 9) || (input->core_cpu_load < 50 && bulwers->longev < 8)) && ((input->core_memory < 50 && bulwers->longev >= 8) || (input->core_memory < 60 && bulwers->longev < 7)) && bulwers->longev < 11)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 13;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 7)
    {
        if ((input->core_memory < 40 && bulwers->longev > 11) || (input->core_memory < 60 && bulwers->longev >= 7) || (input->core_memory < 70 && bulwers->longev < 6))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 21;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 8)
    {
        if (((input->core_cpu_load < 50 && bulwers->longev >= 8) || (input->core_cpu_load < 60 && bulwers->longev < 7)) && ((input->core_memory < 50 && bulwers->longev > 10) || (input->core_memory < 70 && bulwers->longev >= 6) || (input->core_memory < 80 && bulwers->longev < 5)))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 34;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 9)
    {
        if ((input->core_memory < 60 && bulwers->longev > 9) || (input->core_memory < 80 && bulwers->longev >= 5))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 55;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 10)
    {
        if (((input->core_cpu_load < 40 && bulwers->longev > 11) || (input->core_cpu_load < 60 && bulwers->longev >= 7) || (input->core_cpu_load < 70 && bulwers->longev < 6)) && (input->core_memory < 70 && bulwers->longev > 8) && bulwers->longev < 12)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 89;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 11)
    {
        if (input->core_memory < 80 && bulwers->longev > 7)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 144;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 12)
    {
        if ((input->core_cpu_load < 50 && bulwers->longev > 10) || (input->core_cpu_load < 70 && bulwers->longev >= 6) || (input->core_cpu_load < 80 && bulwers->longev < 5))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 233;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 13)
    {
        if (1)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 377;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 14)
    {
        if (((input->core_cpu_load < 60 && bulwers->longev > 9) || (input->core_cpu_load < 80 && bulwers->longev >= 5) || (input->core_cpu_load < 90 && bulwers->longev < 4)) && bulwers->longev < 13)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 610;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 15)
    {
        if ((input->core_cpu_load < 70 && bulwers->longev > 8) || (input->core_cpu_load < 90 && bulwers->longev >= 4))
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 987;
            }
            else
                bulwers->step--;
        }
    }
    if (bulwers->level == 16)
    {
        if (((input->core_cpu_load < 80 && bulwers->longev > 7) || (input->core_cpu_load < 90 && bulwers->longev > 6)) && bulwers->longev < 14)
        {
            if (bulwers->step == 0)
            {
                bulwers->level--;
                bulwers->step = 1597;
            }
            else
                bulwers->step--;
        }
    }


    //---long events



    if (bulwers->longev == 1)
    {
        if (input->core_battery > 2500 || input->core_battery_plugged == 3)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 2)
    {
        if (input->core_battery > 1500 || input->core_battery_plugged == 3)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 3)
    {
        if (input->core_time < 75600)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 4)
    {
        if (input->core_temperature < 56)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 5)
    {
        if (input->core_time < 82800)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 6)
    {
        if (input->core_uptime < 21600 && input->energy >= 25200)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 7)
    {
        if (input->core_temperature < 58)
        {
            bulwers->longev--;
        }
    }

    if (bulwers->longev == 8)
    {
        if (input->core_temperature < 60)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 9)
    {
        if (input->core_uptime < 28800 && input->energy >= 14400)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 10)
    {
        if (input->core_battery > 500 || input->core_battery_plugged == 3)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 11)
    {
        if (input->core_uptime < 39600 && input->energy >= 7200 )
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 12)
    {
        if (input->core_temperature < 62)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 13)
    {
        if (input->core_temperature < 64)
        {
            bulwers->longev--;
        }
    }
    if (bulwers->longev == 14)
    {
        if (input->core_uptime < 50400 && input->energy >= 3600)
        {
            bulwers->longev--;
        }
    }

//---bulwers happyness modifications


    if (bulwers->happy == 1)
        if (bulwers->step >=1)
            bulwers->step--;

    if (bulwers->happy == 2)
        if (bulwers->step >=2)
            bulwers->step-=2;

    if (bulwers->happy == 3)
        if (bulwers->step >=5)
            bulwers->step-=5;

    if (bulwers->happy >= 4)
        if (bulwers->step >=10)
            bulwers->step -=10;


if (input->energy > 0)
    input->energy--;
}





/////////////////////////////////////////////////////

//----END OF BULWERS MANAGING

/////////////////////////////////////////////////////


if (pics.outline <=19 && bulwers->level != 0)
    pics.outline = bulwers->level + 3;
if (pics.outline != 20 && bulwers->level == 0)
    pics.outline = 0;

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

//if (input->core_month == 11 || input->core_month == 12 || input->core_month == 1)
if (pics.outline != 20)
{
    if (input->core_time < 6*3600 || input->core_time >= 22*3600)
        pics.eye = 1;
    if ((input->core_time >= 6*3600 && input->core_time < 7*3600) || (input->core_time >= 18*3600 && input->core_time < 22*3600))
        pics.eye = 2;
    if ((input->core_time >= 7*3600 && input->core_time < 8*3600) || (input->core_time >= 16*3600 && input->core_time < 18*3600))
        pics.eye = 3;
    if ((input->core_time >= 8*3600 && input->core_time < 9*3600) || (input->core_time >= 15*3600 && input->core_time < 16*3600))
        pics.eye = 4;
    if ((input->core_time >= 9*3600 && input->core_time < 11*3600) || (input->core_time >= 14*3600 && input->core_time < 15*3600))
        pics.eye = 5;
    if ((input->core_time >= 11*3600 && input->core_time < 14*3600))
        pics.eye = 6;
}



if (input->core_day != 7)
{
    if (input->core_time < 22800 || input->core_time > 75600 || input->energy < 18000)
    {
        pics.tired = 1;

        if (pics.outline <= 7)
            pics.outline = 1;

        if (pics.outline > 7 && pics.outline < 11)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    if (input->core_time < 21600 || input->core_time > 79200 || input->energy < 10800)
    {
        pics.tired = 2;
        if (pics.outline <= 8)
            pics.outline = 1;

        if (pics.outline > 8 && pics.outline < 12)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    if (input->core_time < 18000 || input->core_time > 82800 || input->energy < 3600 )
    {
        pics.tired = 3;
        if (pics.outline <= 9)
            pics.outline = 1;

        if (pics.outline > 9 && pics.outline < 13)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    else
    {
        if (pics.tired > 0)
            pics.tired--;
    }
}
else
{
    if (input->core_time < 36000 || input->core_time > 82800 || input->energy < 18000)
    {
        pics.tired = 1;
        if (pics.outline <= 7)
            pics.outline = 1;

        if (pics.outline > 7 && pics.outline < 11)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    if ((input->core_time < 28800 && input->core_time > 3600) || input->energy < 10800)
    {
        pics.tired = 2;
        if (pics.outline <= 8)
            pics.outline = 1;

        if (pics.outline > 8 && pics.outline < 12)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    if ((input->core_time < 25200 && input->core_time > 10800) || input->energy < 3600 )
    {
        pics.tired = 3;
        if (pics.outline <= 9)
            pics.outline = 1;

        if (pics.outline > 9 && pics.outline < 13)
        {
            if ((rand () % 1))
                pics.outline = 1;
        }
    }
    else
    {
        if (pics.tired > 0)
            pics.tired--;
        if (pics.outline <= 3 && pics.outline >= 1)
            pics.outline--;
    }
}
/*
56ºC
58ºC
60ºC
62ºC
64ºC
*/

//static int input->temp_t = 0;

if (input->core_temperature >= 56 && input->core_temperature < 58)
{
    pics.hot = 1;
    if (input->temp_t < 30)
        input->temp_t = 30;
    if (input->temp_t > 60)
        get_flu = true;
}
if (input->core_temperature >= 58 && input->core_temperature < 60)
{
    pics.hot = 2;
    if (input->temp_t < 60)
        input->temp_t = 60;
    if (input->temp_t > 80)
        get_flu = true;
}
if (input->core_temperature >= 60 && input->core_temperature < 62)
{
    pics.hot = 3;
    if (input->temp_t < 80)
        input->temp_t = 80;
    if (input->temp_t > 120)
        get_flu = true;
}
if (input->core_temperature >= 62 && input->core_temperature < 64)
{
    pics.hot = 4;
    if (input->temp_t < 120)
        input->temp_t = 120;
}
if (input->core_temperature >= 64)
{
    pics.hot = 5;
    if (input->temp_t < 180)
        input->temp_t = 180;
}

else
{
    if (pics.hot > 0)
        pics.hot--;
}

if (input->temp_t > 0)
    input->temp_t--;

if (get_flu)
{
    print_event ("GET FLU!");

   // static int input->flu_timer = 360;

    if (input->flu_timer > 0)
        input->flu_timer--;

    if (input->flu_timer <= 240)
    {
        pics.hot = 1;
        pics.shy = 1;
        pics.tired = 1;
        if (bulwers->level < 8)
            bulwers->level = 8;
    }
    if (input->flu_timer <= 120)
    {
        pics.hot = 2;
        pics.shy = 2;
        pics.tired = 2;
        if (bulwers->level < 8)
            bulwers->level = 8;
    }
    if (input->flu_timer == 0)
    {
        pics.hot = 3;
        pics.shy = 3;
        pics.tired = 3;
        if (bulwers->level < 10)
            bulwers->level = 10;
        input->energy--;
    }
}

/////////////////////////////////

// PICTURE MANAGING

/////////////////////////////////

/*
if (pics.eye == 1)
    eye = "eye_01_n";
if (pics.eye == 2)
    eye = "eye_02_n";
if (pics.eye == 3)
    eye = "eye_03_n";
if (pics.eye == 4)
    eye = "eye_04_n";
if (pics.eye == 5)
    eye = "eye_05_n";
if (pics.eye == 6)
    eye = "eye_06_n";
if (pics.eye == 7)
    eye = "eye_07_n";
if (pics.eye == 8)
    eye = "eye_08_n";
if (pics.eye == 9)
    eye = "eye_09_n";
if (pics.eye == 10)
    eye = "eye_10_n";

*/
if (pics.outline == 0)
{
    int tmp = rand () % 3;
    if (tmp == 0)
        set_face ("cusual_01");
    if (tmp == 1)
        set_face ("bul_01");
    if (tmp == 2)
        set_face ("bul_02");
    if (tmp == 3)
        set_face ("bul_03");
}
else
{
    if (pics.outline == 1)
    {
        int tmp = rand () % 3 + 1;
        if (tmp == 1)
            set_face ("slp_01");
        if (tmp == 2)
            set_face ("slp_02");
        if (tmp == 3)
            set_face ("slp_03");
        if (tmp == 4)
            set_face ("slp_04");
    }
    if (pics.outline == 2)
    {
        int tmp = rand () % 1 + 4;
        if (tmp == 4)
            set_face ("slp_04");
        if (tmp == 5)
            set_face ("slp_05");
    }
    if (pics.outline == 3)
    {
        int tmp = rand () % 5 + 1;
        if (tmp == 5)
            set_face ("slp_05");
        if (tmp == 6)
            set_face ("slp_06");
    }

    if (pics.outline == 4)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("cusual_01");
        if (tmp == 1)
            set_face ("bul_01");
        if (tmp == 2)
            set_face ("bul_02");
    }
    if (pics.outline == 5)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_01");
        if (tmp == 1)
            set_face ("bul_02");
        if (tmp == 2)
            set_face ("bul_03");
    }
    if (pics.outline == 6)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_02");
        if (tmp == 1)
            set_face ("bul_03");
        if (tmp == 2)
            set_face ("bul_04");
    }
    if (pics.outline == 7)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_03");
        if (tmp == 1)
            set_face ("bul_04");
        if (tmp == 2)
            set_face ("bul_05");
    }
    if (pics.outline == 8)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_04");
        if (tmp == 1)
            set_face ("bul_05");
        if (tmp == 2)
            set_face ("bul_06");
    }
    if (pics.outline == 9)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_05");
        if (tmp == 1)
            set_face ("bul_06");
        if (tmp == 2)
            set_face ("bul_07");
    }
    if (pics.outline == 10)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_06");
        if (tmp == 1)
            set_face ("bul_07");
        if (tmp == 2)
            set_face ("bul_08");
    }
    if (pics.outline == 11)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_07");
        if (tmp == 1)
            set_face ("bul_08");
        if (tmp == 2)
            set_face ("bul_09");
    }
    if (pics.outline == 12)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_08");
        if (tmp == 1)
            set_face ("bul_09");
        if (tmp == 2)
            set_face ("bul_10");
    }
    if (pics.outline == 13)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_09");
        if (tmp == 1)
            set_face ("bul_10");
        if (tmp == 2)
            set_face ("bul_11");
    }
    if (pics.outline == 14)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_10");
        if (tmp == 1)
            set_face ("bul_11");
        if (tmp == 2)
            set_face ("bul_12");
    }
    if (pics.outline == 15)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_11");
        if (tmp == 1)
            set_face ("bul_12");
        if (tmp == 2)
            set_face ("bul_13");
    }
    if (pics.outline == 16)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_12");
        if (tmp == 1)
            set_face ("bul_13");
        if (tmp == 2)
            set_face ("bul_14");
    }
    if (pics.outline == 17)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_13");
        if (tmp == 1)
            set_face ("bul_14");
        if (tmp == 2)
            set_face ("bul_15");
    }
    if (pics.outline == 18)
    {
        int tmp = rand () % 2;
        if (tmp == 0)
            set_face ("bul_14");
        if (tmp == 1)
            set_face ("bul_15");
        if (tmp == 2)
            set_face ("bul_16");
    }
    if (pics.outline == 19)
    {
        int tmp = rand () % 1;
        if (tmp == 0)
            set_face ("bul_15");
        if (tmp == 1)
            set_face ("bul_16");
    }

    if (pics.outline == 20 && prev_pics.outline != 20)
        set_face ("sh_02");

    if (pics.outline != 20 && prev_pics.outline == 20)
        set_face ("sh_01");

    if (pics.outline == 21)
        set_face ("slp_10");

}


if (identical (pics, prev_pics))
{}
else if (pics.outline != 20)
    update ();

prev_pics = pics;




//---sleepping

if (!wake_up)
{
    pics.outline = 21;
    if (input->core_day != 7)
    {
        if (input->core_time >= 22800 )
        {
            input->energy = 54000;
            wake_up = true;
            pics.outline = 3;
        }
        else
            input->energy = 43200;
    }
    else
    {

        if (input->core_time >= 36000 )
        {
            input->energy = 72000;
            wake_up = true;
            pics.outline = 3;
        }
        else
            input->energy = 43200;
    }
    cout << "\033[13;17H" << "Wake up Neo!" << endl;
}

cout << "\033[4;22H" << pics.outline;
}


bool identical (pict_layers el1, pict_layers el2)
{
    if (el1.eye == el2.eye && el1.hot == el2.hot && el1.layer2 == el2.layer2 && el1.layer3 == el2.layer3 && el1.layer4 == el2.layer4 && el1.outline == el2.outline && el1.shy == el2.shy && el1.tired == el2.tired)
        return true;
    else return false;
}


void core_main ()
{
        bulwers_init ();
        srand ( time(NULL) );
        event_now = false;
        core_step = 0;
        wake_up = false;
        cout << "\033[40m" << endl;
        cout << "\033[2J\033[0;0H";
        cout << "\033[32m" << endl;
        naglowek_in ("v.0.0.1a-01");
        print ( " Welcome in eyes project!" );
        sleep (1);
        naglowek_out ("v.0.0.1a-01");
        cout << "\033[2J\033[0;0H";
        cout << "\033[90A" << endl;
        cout << "\033[2J";
        print_gui ();
        cout << "\033[1;33m";
        core_stats input;
        initialization ( &input );
while (1)
{
    if ( is_finished )
    {
        cleanup ();
        return;
    }
    cout << "\033[0;22H" << core_step << '\n';
    reload_stats ( &input );
    eyes->update_bulwers ( &input );
    sleep (1);
    core_step++;
}





}


void initialization ( core_stats * input)
{
                input->core_day                 =get_time ().day;
                input->core_dnum                =get_time ().day_num;
                input->core_month               =get_time ().month;
                input->core_year                =get_time ().year;
                input->core_time                =get_time ().hour;
                input->core_battery_plugged     =bat_plugged ();
                input->core_temperature         =temperatura ();
                input->core_battery             =bateria();
                input->current_probe            =0;
                input->current_probe_small      =0;
                input->cpu_probes[0]            =20;
                input->cpu_probes[1]            =20;
                input->cpu_probes[2]            =20;
                input->cpu_probes[3]            =20;
                input->cpu_probes[4]            =20;
                input->cpu_probes[5]            =20;
                input->cpu_probes[6]            =20;
                input->cpu_probes[7]            =20;
                input->cpu_probes[8]            =20;
                input->cpu_probes[9]            =20;
                input->cpu_sector_small[0]      =20;
                input->cpu_sector_small[1]      =20;
                input->cpu_sector_small[2]      =20;
                input->cpu_sector_small[3]      =20;
                input->cpu_sector_small[4]      =20;
                input->cpu_sector_small[5]      =20;
                input->cpu_sector_small[6]      =20;
                input->cpu_sector_small[7]      =20;
                input->cpu_sector_small[8]      =20;
                input->cpu_sector_small[9]      =20;
                input->core_cpu_load            =20;
                input->core_memory              =M_LOAD ();
                input->core_proclist            =P_LIST ();
                input->core_uptime              =U_TIME ();
                input->energy                   =54000;
                input->prev_bat_plug            =input->core_battery_plugged;
                input->battery_buffer           =0;
                input->temp_t                   =0;
                input->flu_timer                =360;
                input->prev_happy               =0;
                input->once_plugged             =false;
}


void reload_stats ( core_stats * input )
{


                input->core_day                 =get_time ().day;
                input->core_dnum                =get_time ().day_num;
                input->core_month               =get_time ().month;
                input->core_year                =get_time ().year;
                input->core_time                =get_time ().hour;
                input->core_battery_plugged     =bat_plugged ();
                input->core_temperature         =temperatura ();
                input->core_battery             =bateria();
                input->current_probe_small      ++;
                if (input->current_probe_small == 10)
                    input->current_probe_small = 0;
                input->cpu_sector_small[input->current_probe_small] = C_LOAD ();
                if (input->cpu_sector_small[input->current_probe_small] > 100)
                    input->cpu_sector_small[input->current_probe_small] = 20;
                if (input->cpu_sector_small[input->current_probe_small] == 100)
                    input->cpu_sector_small[input->current_probe_small] = 99;
                input->cpu_probes[input->current_probe] = ((input->cpu_sector_small[0] +
                                                            input->cpu_sector_small[1] +
                                                            input->cpu_sector_small[2] +
                                                            input->cpu_sector_small[3] +
                                                            input->cpu_sector_small[4] +
                                                            input->cpu_sector_small[5] +
                                                            input->cpu_sector_small[6] +
                                                            input->cpu_sector_small[7] +
                                                            input->cpu_sector_small[8] +
                                                            input->cpu_sector_small[9]) / 10);
                if (core_step % 10 == 0)
                {
                input->current_probe            ++;
                input->core_cpu_load = (input->cpu_probes[0] +
                                        input->cpu_probes[1] +
                                        input->cpu_probes[2] +
                                        input->cpu_probes[3] +
                                        input->cpu_probes[4] +
                                        input->cpu_probes[5] +
                                        input->cpu_probes[6] +
                                        input->cpu_probes[7] +
                                        input->cpu_probes[8] +
                                        input->cpu_probes[9]) / 10;
                if (input->current_probe == 10)
                    input->current_probe = 0;
                }
                input->core_memory              =M_LOAD ();
                input->core_proclist            =P_LIST ();
                input->core_uptime              =U_TIME ();
}


//===========================

inline static void print ( char * str )
{
  print ( str, 0 );
}

static void print ( char * str, int _pos )
{
  cout << "\033[1;32m";
  int plus = _pos+1;
  for ( int i=0 ; i<strlen(str) ; i++ )
  {
    cout << str[i] << '\n';
    if ( str[i] == '\n' )
    {
      usleep ( 50000 );
      plus = _pos+1;
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
  print ( " stage:\ncurrent cpu probe:\ncureent buffer:\nreturned face:" );
  cout  << "\033[0;35H";
  print ( " cpu probes table:\ncpu buffer table", 35 );
  cout << "\033[3;40H";
  print ( " cpu:\nmemory:\nproclist:\nuptime:", 40 );
  cout << "\033[4;75H";
  print ( " day of week:\nday of month:\nmonth:\nyear:\ntime:\n", 75 );
  cout << "\033[10;0H";
  print ( " batery state:\nbatery power:\ntemperature:\ninput->energy:" );
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

int pulsetext (char * text, int delay, int repeat, int _position)
{
    while (repeat>0)
    {
        if (repeat%2 == 1)
        cout << text << endl;
        else
        cout << "                    " << endl;

     cout << "\033[1A" << "\033[" << _position << "C";
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

//pict_layers bulwers_init()




//Version 0.0.1a-01
//Chiliński Damian
//GBS
